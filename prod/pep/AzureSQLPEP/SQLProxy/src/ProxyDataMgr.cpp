#include <assert.h>

#include "ProxyManager.h"
#include "ProxyDataMgr.h"
#include "ProxyChannel.h"
#include "CriticalSectionLock.h"
#include "Config.h"
#include "CommonFunc.h"

#include "tdsPacket.h"
#include "TDSHelper.h"
#include "SMP.h"

ProxyDataMgr* theProxyDataMgr = NULL;

ProxyDataMgr::ProxyDataMgr(void)
{
    m_hThreadCacheSockData = NULL;
	m_hThreadDecryptData = NULL;
	m_hTdsDataReadyEvent = NULL;
	m_hSocketDataReadyEvent = NULL;

	InitializeCriticalSection(&m_csTdsData);
	InitializeCriticalSection(&m_csDataSocket);
}


ProxyDataMgr::~ProxyDataMgr(void)
{
}

void ProxyDataMgr::Init(HANDLE hTdsDataReadyEvent, HANDLE hSocketDataReadyEvent)
{
	m_hTdsDataReadyEvent = hTdsDataReadyEvent;
	m_hSocketDataReadyEvent = hSocketDataReadyEvent;
}

void ProxyDataMgr::ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length)
{
    ProxyChannelPtr pProxyChannel = ProxyManager::GetInstance()->GetProxyChannel(tcpSocket.get());
    if (pProxyChannel.get() == nullptr)
    {
        PROXYLOG(CELOG_EMERG, L"ProxyDataMgr::ReceiveDataEvent [pProxyChannel is null]. Close socket");
        theTCPFrame->Close(tcpSocket);
        return;
    }

    if (pProxyChannel->IsChannelClose())
        return;

    std::vector<uint8_t> decrypted_data;
    uint32_t decrypted_len = 0;
    if (data[0] == 0x17)
    {
        std::list<SecBuffer> lstTdsData;
        PBYTE pAllocateBuf = NULL;

        pProxyChannel->IsClientSocket(tcpSocket.get()) ? pProxyChannel->DecryptClientMessage(data, length, lstTdsData, &pAllocateBuf) :
                                                         pProxyChannel->DecryptServerMessage(data, length, lstTdsData, &pAllocateBuf) ;

        for (auto itData : lstTdsData)
        {
            decrypted_data.resize(decrypted_len + itData.cbBuffer);
            memcpy(&decrypted_data[decrypted_len], itData.pvBuffer, itData.cbBuffer);
            decrypted_len += itData.cbBuffer;
        }

        data = &decrypted_data[0];
        length = decrypted_len;

        // free buf
        if (NULL != pAllocateBuf)
        {
            delete[] pAllocateBuf;
            pAllocateBuf = NULL;
        }
    }

    boost::shared_ptr<ProxyTask> pTask = GetTdsTask(tcpSocket.get(), pProxyChannel, data, length);
    if (pTask.get() && pTask->IsCompleteTask())
    {
        if (pTask->IsServerMessage() && pProxyChannel->GetChannelServerStatus() == CHANNEL_STATUS_Proxy_Success)
        {
            pProxyChannel->PushBackRespNeedDispatch(pTask);
        }
        else if (!pTask->IsServerMessage() && pProxyChannel->GetChannelClientStatus() == CHANNEL_STATUS_Proxy_Success)
        {
            pProxyChannel->PushBackReqNeedDispatch(pTask);
        }

        pTask->Execute();
    }
}

uint32_t ProxyDataMgr::TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len)
{
    if (pBuff == nullptr || len == 0)
        return 0;

    if (pBuff[0] == 0x17)
    {
        if (len >= 5) {
            uint32_t tlslen = ntohs(*(uint16_t*)&(pBuff[3]));
            return tlslen + 5;
        }
        else
            return -1;
    }
    else if (pBuff[0] == SMP_PACKET_FLAG)
    {
        if (len >= 16)
            return SMP::GetSMPPacketLen(pBuff);
        else
            return -1;
    }
    else if (TDS::GetTdsPacketType(pBuff) != TDS_Unkown)
    {
        if (len >= 8)
            return TDS::GetTdsPacketLength(pBuff);
        else
            return -1;
    }
    else
    {
        // unknow packet!
        PROXYLOG(CELOG_WARNING, "Unkown packet! Can not calculate packet length! pack flag: 0x%X", pBuff[0]);
        return 0;
    }
}

void* ProxyDataMgr::GetTdsDataSocket()
{
	void* dataSocket = NULL;
	CriticalSectionLock lockDataSocket(&m_csDataSocket);
	if (m_lstDataSockets.size()){
		dataSocket = *m_lstDataSockets.begin();
		m_lstDataSockets.pop_front();
	}

	return dataSocket;
}

boost::shared_ptr<ProxyTask> ProxyDataMgr::GetTdsTask(void* tcpSocket, ProxyChannelPtr channel, uint8_t* data, uint32_t len)
{
    boost::shared_ptr<ProxyTask> pTdsTask(NULL);

    if (data == nullptr || len == 0 || tcpSocket == nullptr || channel.get() == nullptr)
        return pTdsTask;

    uint32_t session_id = SESSION_ID_IF_NO_SMP;
    //uint32_t smp_ack_wndw = 0;
    if (data[0] == SMP_PACKET_FLAG)
    {
        SMPHeader smp;
        smp.Parse(data);
        session_id = smp.m_sid;
        if (smp.m_flags == SMP_SYN)
        {
            channel->SetSessionInitWndw(session_id, smp.m_wndw);
        }
        else if (smp.m_flags == SMP_FIN)
        {
            channel->EndSession(session_id);
        }
        else if (smp.m_flags == SMP_ACK)
        {
            if (tcpSocket == channel->GetClientSocket().get())
                channel->SetSessionAckWndwFromClient(session_id, smp.m_wndw);
            else if (tcpSocket == channel->GetServerSocket().get())
                channel->SetSessionAckWndwFromServer(session_id, smp.m_wndw);

            channel->SetEventForSessionDispEvent(session_id);

            return pTdsTask;
            // do not add ack packet to task
        }
    }

    if (channel->IsClientSocket(tcpSocket))
        pTdsTask = channel->GetIncompleteClientTask(session_id);
    else if (channel->IsServerSocket(tcpSocket))
        pTdsTask = channel->GetIncompleteServerTask(session_id);
    else
    {
        return pTdsTask;
    }

    if (pTdsTask.get() == nullptr) {
        pTdsTask = boost::shared_ptr<ProxyTask>(new ProxyTask(tcpSocket));
        pTdsTask->SetSessionID(session_id);
        pTdsTask->SetChannel(channel);
        pTdsTask->SetIsServerMessage(channel->IsServerSocket(tcpSocket));

        if (channel->IsClientSocket(tcpSocket))
            channel->SetIncompleteClientTask(session_id, pTdsTask);
        else if (channel->IsServerSocket(tcpSocket))
            channel->SetIncompleteServerTask(session_id, pTdsTask);
        else
        {
            return boost::shared_ptr<ProxyTask>(NULL);
        }
    }

    PBYTE task_buff = new BYTE[len];
    memcpy(task_buff, data, len);
    pTdsTask->AddedTdsPacket(task_buff);

    if (data[0] == SMP_PACKET_FLAG) {
        if (len == 16) {
            pTdsTask->SetCompleteTask(TRUE);

            if (channel->IsClientSocket(tcpSocket))
                channel->RemoveIncompleteClientTask(session_id);
            else if (channel->IsServerSocket(tcpSocket))
                channel->RemoveIncompleteServerTask(session_id);
        }
        else if (len > 16 && TDS::IsEndOfMsg(data + 16)) {
            pTdsTask->SetCompleteTask(TRUE);

            if (channel->IsClientSocket(tcpSocket))
                channel->RemoveIncompleteClientTask(session_id);
            else if (channel->IsServerSocket(tcpSocket))
                channel->RemoveIncompleteServerTask(session_id);
        }
    }
    else {
        if (TDS::IsEndOfMsg(data)) {
            pTdsTask->SetCompleteTask(TRUE);

            if (channel->IsClientSocket(tcpSocket))
                channel->RemoveIncompleteClientTask(session_id);
            else if (channel->IsServerSocket(tcpSocket))
                channel->RemoveIncompleteServerTask(session_id);
        }
    }

	return pTdsTask;
}

void ProxyDataMgr::CleanSocket(void* tcpSocket)
{
	const int nTryTimes = 3;
	int nTryTime = 0;
	do
	{
		nTryTime++;
		CriticalSectionLock lockSocketData(&m_csTdsData);

		std::map<void*,MemoryCache*>::iterator itData =  m_tdsDatas.find(tcpSocket);
		if (itData != m_tdsDatas.end()){
			MemoryCache* pCache = itData->second;

			if (pCache->CBSize() == 0 || (nTryTimes <= nTryTime)){
				delete pCache;
				m_tdsDatas.erase(tcpSocket);
				break;
			}
		}
		else{
			break;
		}

        Sleep(500);

	}while (TRUE);
}