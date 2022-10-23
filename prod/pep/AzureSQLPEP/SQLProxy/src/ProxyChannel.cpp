#include "ProxyChannel.h"
#include "ProxyManager.h"
#include "CertificateHelper.h"
#include "TDS.h"
#include "ProxyTask.h"
#include "Config.h"

ProxyChannel::ProxyChannel(void):m_ClientSocket(NULL),
	m_ServerSocket(NULL),
    m_tlsSvrCred({0,0}),
    m_tlsCltCred({0,0}),
    m_krbSvrCred({0,0}),
    m_krbCltCred({0,0}),
	m_bClientTLSFinished(FALSE),
	m_bServerTLSFinished(FALSE),
	m_bTLSCtxServerInited(FALSE),
	m_bTLSCtxClientInited(FALSE),
	m_dwInCompleteTLSBufServerLen(0),
	m_dwInCompleteTLSBufClientLen(0),
	m_dwTLSCacheType(0),
	//m_pIncompleteTask(NULL),
	m_finalRemoteServer(theConfig.RemoteServer()),
	m_finalRemoteServerPort(theConfig.RemoteServerPort()),
	m_channelCloseCause(CHANNEL_CLOSE_CAUSE_NONE),
    m_bChannelClose(false),
	m_dwNegotiatePacketSize(4096),
    m_encryptOption(1)  // default to open encryption
{
	m_ChannelClientStatus = CHANNEL_STATUS_Wait_Client_First_Prelogin;
	m_ChannelServerStatus = CHANNEL_STATUS_Wait_Server_First_Prelogin_Response;

	memset(&m_TLSCtxClient, 0, sizeof(m_TLSCtxClient));
	memset(&m_TLSCtxServer, 0, sizeof(m_TLSCtxServer));
    memset(&m_TLSSizesToClient, 0, sizeof(m_TLSSizesToClient));
    memset(&m_TLSSizesToServer, 0, sizeof(m_TLSSizesToServer));

	m_hEventTLSSuccessWithRemoteServer = CreateEventW(NULL, FALSE, FALSE, NULL);

	InitializeCriticalSection(&m_csListReqNeedDispatch);
	InitializeCriticalSection(&m_csListRespNeedDispatch);
	InitializeCriticalSection(&m_csListReqDispatched);
    InitializeCriticalSection(&m_cs_default_db);
    InitializeCriticalSection(&m_csSession);
}


ProxyChannel::~ProxyChannel(void)
{
	for (auto it : m_cachedLogin7Packets)
		delete[] it;
	m_cachedLogin7Packets.clear();
	for (auto it : m_cachedPreloginPackets)
		delete[] it;
	m_cachedPreloginPackets.clear();

    for (auto it : m_cachedSSPIPackets)
        delete[] it;
    m_cachedSSPIPackets.clear();

    for (auto it : m_mapReqDispatched) {
        it.second.clear();
    }
    m_mapReqDispatched.clear();

    for (auto it : m_mapReqNeedDispatch) {
        it.second.clear();
    }
    m_mapReqNeedDispatch.clear();

    for (auto it : m_mapRespNeedDispatch) {
        it.second.clear();
    }
    m_mapRespNeedDispatch.clear();

    m_mapIncompleteTaskClient.clear();
    m_mapIncompleteTaskServer.clear();

    for (auto it : m_mapSession) {
        CloseHandle(it.second.dispatch_event);
    }
    m_mapSession.clear();

    if (m_hEventTLSSuccessWithRemoteServer) {
        CloseHandle(m_hEventTLSSuccessWithRemoteServer);
        m_hEventTLSSuccessWithRemoteServer = NULL;
    }

	DeleteCriticalSection(&m_csListReqNeedDispatch);
	DeleteCriticalSection(&m_csListRespNeedDispatch);
	DeleteCriticalSection(&m_csListReqDispatched);
    DeleteCriticalSection(&m_cs_default_db);
    DeleteCriticalSection(&m_csSession);

    ProxyManager::GetInstance()->SecurityFunTable->DeleteSecurityContext(&m_TLSCtxClient);
    ProxyManager::GetInstance()->SecurityFunTable->DeleteSecurityContext(&m_TLSCtxServer);
    memset(&m_TLSCtxClient, 0, sizeof(m_TLSCtxClient));
    memset(&m_TLSCtxServer, 0, sizeof(m_TLSCtxServer));

    ProxyManager::GetInstance()->SecurityFunTable->FreeCredentialsHandle(&m_tlsSvrCred);
    ProxyManager::GetInstance()->SecurityFunTable->FreeCredentialsHandle(&m_tlsCltCred);
    ProxyManager::GetInstance()->SecurityFunTable->FreeCredentialsHandle(&m_krbSvrCred);
    ProxyManager::GetInstance()->SecurityFunTable->FreeCredentialsHandle(&m_krbCltCred);
    memset(&m_tlsSvrCred, 0, sizeof(m_tlsSvrCred));
    memset(&m_tlsCltCred, 0, sizeof(m_tlsCltCred));
    memset(&m_krbSvrCred, 0, sizeof(m_krbSvrCred));
    memset(&m_krbCltCred, 0, sizeof(m_krbCltCred));
}

bool ProxyChannel::InitTlsServerCred()
{
    if (ProxyManager::GetInstance()->GetTlsCredContext() == NULL)
        return false;

    SECURITY_STATUS status = CertificateHelper::CreateCredentialsFromCertificate(&m_tlsSvrCred, ProxyManager::GetInstance()->GetTlsCredContext());
    if (FAILED(status))
    {
        PROXYLOG(CELOG_ERR, "ProxyChannel::InitTlsServerCred CreateCredentialsFromCertificate failed! result: 0x%X", status);
        return false;
    }

    return true;
}

bool ProxyChannel::InitTlsClientCred()
{
    SECURITY_STATUS status = CertificateHelper::CreateCredentialsForClient(NULL, &m_tlsCltCred);
    if (FAILED(status))
    {
        PROXYLOG(CELOG_ERR, "ProxyChannel::InitTlsClientCred CreateCredentialsForClient failed! result: 0x%X", status);
        return false;
    }

    return true;
}

bool ProxyChannel::InitKrbServerCred()
{
    BOOL ret = TRUE;

    SECURITY_STATUS status = CertificateHelper::CreateKrbCredForServer(&m_krbSvrCred);
    if (FAILED(status))
    {
        PROXYLOG(CELOG_EMERG, "ProxyChannel::InitKrbServerCred Create kerberos server cred failed!");
        ret = FALSE;
    }

    return ret;
}

bool ProxyChannel::InitKrbClientCred()
{
    BOOL ret = TRUE;

    SECURITY_STATUS status = CertificateHelper::CreateKrbCredForClient(&m_krbCltCred);
    if (FAILED(status))
    {
        PROXYLOG(CELOG_EMERG, "ProxyChannel::InitKrbClientCred Create kerberos client cred failed!");
        ret = FALSE;
    }

    return ret;
}

void ProxyChannel::SetIncompleteClientTask(uint32_t session_id, boost::shared_ptr<ProxyTask> pTask)
{
    m_mapIncompleteTaskClient[session_id] = pTask;
}

void ProxyChannel::SetIncompleteServerTask(uint32_t session_id, boost::shared_ptr<ProxyTask> pTask)
{
    m_mapIncompleteTaskServer[session_id] = pTask;
}

boost::shared_ptr<ProxyTask> ProxyChannel::GetIncompleteClientTask(uint32_t session_id)
{
    if (m_mapIncompleteTaskClient.find(session_id) != m_mapIncompleteTaskClient.end())
    {
        return m_mapIncompleteTaskClient[session_id];
    }

    return nullptr;
}

boost::shared_ptr<ProxyTask> ProxyChannel::GetIncompleteServerTask(uint32_t session_id)
{
    if (m_mapIncompleteTaskServer.find(session_id) != m_mapIncompleteTaskServer.end())
    {
        return m_mapIncompleteTaskServer[session_id];
    }

    return nullptr;
}

void ProxyChannel::RemoveIncompleteClientTask(uint32_t session_id)
{
    if (m_mapIncompleteTaskClient.find(session_id) != m_mapIncompleteTaskClient.end())
    {
        m_mapIncompleteTaskClient.erase(session_id);
    }
}

void ProxyChannel::RemoveIncompleteServerTask(uint32_t session_id)
{
    if (m_mapIncompleteTaskServer.find(session_id) != m_mapIncompleteTaskServer.end())
    {
        m_mapIncompleteTaskServer.erase(session_id);
    }
}

uint32_t ProxyChannel::GetSessionInitWndw(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].syn_wndw;

    return 0;
}

void ProxyChannel::SetSessionInitWndw(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end()) {
        m_mapSession[sid].syn_wndw = wndw;
        m_mapSession[sid].ack_wndw_from_client = wndw;
        m_mapSession[sid].ack_wndw_from_server = wndw;
        m_mapSession[sid].ack_wndw_to_client = wndw;
        m_mapSession[sid].ack_wndw_to_server = wndw;
        m_mapSession[sid].wndw_to_client = wndw + 1;
        m_mapSession[sid].wndw_to_server = wndw;
        m_mapSession[sid].request_seqnum = 0;
    }
    else
    {
        SessionInChannel sic;
        sic.syn_wndw = wndw;
        sic.ack_wndw_from_client = wndw;
        sic.ack_wndw_from_server = wndw;
        sic.ack_wndw_to_client = wndw;
        sic.ack_wndw_to_server = wndw;
        sic.wndw_to_client = wndw + 1;
        sic.wndw_to_server = wndw;
        sic.dispatch_event = CreateEvent(NULL, FALSE, FALSE, NULL);
        m_mapSession.insert(std::make_pair(sid, sic));
    }
}

uint32_t ProxyChannel::GetSessionAckWndwFromClient(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].ack_wndw_from_client;

    return 0;
}

void ProxyChannel::SetSessionAckWndwFromClient(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        m_mapSession[sid].ack_wndw_from_client = wndw;
}

uint32_t ProxyChannel::GetSessionAckWndwFromServer(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].ack_wndw_from_server;

    return 0;
}

void ProxyChannel::SetSessionAckWndwFromServer(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        m_mapSession[sid].ack_wndw_from_server = wndw;
}

uint32_t ProxyChannel::GetSessionAckWndwToClient(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].ack_wndw_to_client;

    return 0;
}

void ProxyChannel::SetSessionAckWndwToClient(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        m_mapSession[sid].ack_wndw_to_client = wndw;
}

uint32_t ProxyChannel::GetSessionAckWndwToSever(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].ack_wndw_to_server;

    return 0;
}

void ProxyChannel::SetSessionAckWndwToSever(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    session_map::iterator it = m_mapSession.find(sid);
    if (it != m_mapSession.end())
    {
        it->second.ack_wndw_to_server = wndw;
        it->second.wndw_to_server = wndw + 1;
    }
}

uint32_t ProxyChannel::GetSessionRequsetSeqnum(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].request_seqnum;

    return 0;
}

void ProxyChannel::SetSessionRequestSeqnum(uint32_t sid, uint32_t seqnum)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        m_mapSession[sid].request_seqnum = seqnum;
}

uint32_t ProxyChannel::GetSessionResponseSeqnum(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        return m_mapSession[sid].response_seqnum;

    return 0;
}

void ProxyChannel::SetSessionResponseSeqnum(uint32_t sid, uint32_t seqnum)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end())
        m_mapSession[sid].response_seqnum = seqnum;
}

uint32_t ProxyChannel::GetSessionWndwToClient(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    session_map::iterator it = m_mapSession.find(sid);
    if (it != m_mapSession.end())
        return it->second.wndw_to_client;

    return 0;
}

void ProxyChannel::SetSessionWndwToClient(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    session_map::iterator it = m_mapSession.find(sid);
    if (it != m_mapSession.end())
        it->second.wndw_to_client = wndw;
}

uint32_t ProxyChannel::GetSessionWndwToServer(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    session_map::iterator it = m_mapSession.find(sid);
    if (it != m_mapSession.end())
        return it->second.wndw_to_server;

    return 0;
}

void ProxyChannel::SetSessionWndwToServer(uint32_t sid, uint32_t wndw)
{
    CriticalSectionLock cs(&m_csSession);
    session_map::iterator it = m_mapSession.find(sid);
    if (it != m_mapSession.end())
        it->second.wndw_to_server = wndw;
}

void ProxyChannel::SetEventForSessionDispEvent(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end()) {
        SetEvent(m_mapSession[sid].dispatch_event);
        //PROXYLOG(CELOG_DEBUG, "set event session[%d] handle: %#x", sid, m_mapSession[sid].dispatch_event);
    }
}

void ProxyChannel::WaitForSessionDispEvent(uint32_t sid, uint32_t timeout)
{
    HANDLE hEvent = NULL;
    {
        CriticalSectionLock cs(&m_csSession);
        if (m_mapSession.find(sid) != m_mapSession.end())
            hEvent = m_mapSession[sid].dispatch_event;
    }

    if (hEvent != NULL) {
        DWORD dwWait = WaitForSingleObject(hEvent, timeout);
        if (dwWait == WAIT_TIMEOUT)
            PROXYLOG(CELOG_WARNING, "Wait for session[%d] event time out.", sid);
    }
}

void ProxyChannel::EndSession(uint32_t sid)
{
    CriticalSectionLock cs(&m_csSession);
    if (m_mapSession.find(sid) != m_mapSession.end()) {
        CloseHandle(m_mapSession[sid].dispatch_event);
        m_mapSession.erase(sid);
    }
}

DWORD ProxyChannel::DecryptClientMessage(PBYTE pBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, PBYTE*  ppAllocateBuf)
{
	PBYTE pInputData = pBuf;
	DWORD dwInputLen = dwBufLen;

	if (m_dwInCompleteTLSBufClientLen>0)
	{
		//we must combine the previous incomplete message first
		//for allocate the new buffer here, refer to DecryptServerMessage
		pInputData = new BYTE[dwBufLen + m_dwInCompleteTLSBufClientLen];
		dwInputLen = dwBufLen + m_dwInCompleteTLSBufClientLen;

		memcpy(pInputData, m_inCompleteTLSBufClient, m_dwInCompleteTLSBufClientLen);
		memcpy(pInputData + m_dwInCompleteTLSBufClientLen, pBuf, dwBufLen);
		
		m_dwInCompleteTLSBufClientLen = 0;
		*ppAllocateBuf = pInputData;
	}


	SecBuffer extraData={0, SECBUFFER_EMPTY, NULL};
	DWORD cbData = DecryptMessage(GetServerTLSCtx(), pInputData, dwInputLen, tdsData, &extraData );

	if (extraData.cbBuffer>0 && extraData.BufferType==SECBUFFER_EXTRA)
	{
		//Save extra data
		memcpy(m_inCompleteTLSBufClient, extraData.pvBuffer, extraData.cbBuffer);
		m_dwInCompleteTLSBufClientLen = extraData.cbBuffer;
		assert(m_dwInCompleteTLSBufClientLen<=INCOMPLETE_TLS_MSG_LEN);
	}

	return 0;
}

DWORD ProxyChannel::DecryptServerMessage(PBYTE pBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, PBYTE*  ppAllocateBuf)
{
	PBYTE pInputData = pBuf;
	DWORD dwInputLen = dwBufLen;

	if (m_dwInCompleteTLSBufServerLen>0)
	{
		//we must combine the previous incomplete message first
		//we can't just copy the pBuf to m_inCompleteTLSBufServer and must allocate a new buffer for decrypt, because it is in-place decrypt.
		//and after decrypt it may have extra data, if we don't allocate the new buf, the extra data will cover the decrypted data.!!!11
		pInputData = new BYTE[dwBufLen + m_dwInCompleteTLSBufServerLen];
		dwInputLen = dwBufLen + m_dwInCompleteTLSBufServerLen;

		memcpy(pInputData, m_inCompleteTLSBufServer, m_dwInCompleteTLSBufServerLen);
		memcpy(pInputData + m_dwInCompleteTLSBufServerLen, pBuf, dwBufLen);

		m_dwInCompleteTLSBufServerLen = 0;
		*ppAllocateBuf = pInputData;	
	}

	SecBuffer extraData={0, SECBUFFER_EMPTY, NULL};
	DWORD cbData = DecryptMessage(GetClientTLSCtx(), pInputData, dwInputLen, tdsData, &extraData );

	if (extraData.cbBuffer>0 && extraData.BufferType==SECBUFFER_EXTRA)
	{
		//Save extra data
		memcpy(m_inCompleteTLSBufServer, extraData.pvBuffer, extraData.cbBuffer);
		m_dwInCompleteTLSBufServerLen = extraData.cbBuffer;
		assert(m_dwInCompleteTLSBufServerLen<=INCOMPLETE_TLS_MSG_LEN);
	}

	return 0;
}

DWORD ProxyChannel::DecryptMessage(CtxtHandle* tlsCtx, PBYTE pInBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, SecBuffer* extraData)
{

	SECURITY_STATUS    scRet;               // unsigned long cbBuffer;    // Size of the buffer, in bytes
	SecBufferDesc      Message;             // unsigned long BufferType;  // Type of the buffer (below)
    SecBuffer          Buffers[4] = {0};    // void    SEC_FAR * pvBuffer;   // Pointer to the buffer

	PVOID pCipherData = pInBuf;
	DWORD dwCipherDataLen = dwBufLen;
    int nTryMax = 5;
	for (int nTry = 1; nTry <= nTryMax; nTry++)
	{
		Buffers[0].pvBuffer     = pCipherData;
		Buffers[0].cbBuffer     = dwCipherDataLen;
		Buffers[0].BufferType   = SECBUFFER_DATA;  // Initial Type of the buffer 1
		Buffers[1].BufferType   = SECBUFFER_EMPTY; // Initial Type of the buffer 2 
		Buffers[2].BufferType   = SECBUFFER_EMPTY; // Initial Type of the buffer 3 
		Buffers[3].BufferType   = SECBUFFER_EMPTY; // Initial Type of the buffer 4 

		Message.ulVersion       = SECBUFFER_VERSION;    // Version number
		Message.cBuffers        = 4;                    // Number of buffers - must contain four SecBuffer structures.
		Message.pBuffers        = Buffers;              // Pointer to array of buffers

		scRet = ProxyManager::GetInstance()->SecurityFunTable->DecryptMessage(tlsCtx, &Message, 0, NULL);

		if (scRet==SEC_E_OK){
			//success to decrypt message
			SecBuffer* pDataBuffer  = NULL;
			SecBuffer* pExtraBuffer = NULL;
			for(int i = 1; i < 4; i++){
				if( pDataBuffer  == NULL && Buffers[i].BufferType == SECBUFFER_DATA  ) pDataBuffer  = &Buffers[i];
				if( pExtraBuffer == NULL && Buffers[i].BufferType == SECBUFFER_EXTRA ) pExtraBuffer = &Buffers[i];
			}

			if (pDataBuffer){
				tdsData.push_back(*pDataBuffer);
			}
			
			if (pExtraBuffer){
				//need to Decrypt once again
				pCipherData = pExtraBuffer->pvBuffer;
				dwCipherDataLen = pExtraBuffer->cbBuffer;
				continue;
			}
			else {
				//no extra data, the decrypt is finished
				break;
			}

		}
		else if (scRet == SEC_E_INCOMPLETE_MESSAGE){

			extraData->BufferType = SECBUFFER_EXTRA;
			extraData->cbBuffer = dwCipherDataLen;
			extraData->pvBuffer = pCipherData;
			break;
		}
		else {
			//failed
			assert(FALSE);
		}

	}
	
	return 0;
}

void ProxyChannel::EncryptClientMessage(std::list<PBYTE>& lstData, std::list<SecBuffer>& lstEncryptData)
{
    for (auto pack : lstData)
    {
        DWORD dwLen = 0;
        if (pack[0] == SMP_PACKET_FLAG)
            dwLen = SMP::GetSMPPacketLen(pack);
        else
            dwLen = TDS::GetTdsPacketLength(pack);

        EncryptMessage(GetClientTLSCtx(), &m_TLSSizesToServer, pack, dwLen, lstEncryptData);
    }
}

void ProxyChannel::EncryptClientMessage(PBYTE pData, DWORD dwDataLen, std::list<SecBuffer>& lstEncryptData)
{
	EncryptMessage(GetClientTLSCtx(), &m_TLSSizesToServer, pData, dwDataLen, lstEncryptData);
}

void ProxyChannel::EncryptServerMessage(std::list<PBYTE>& lstData, std::list<SecBuffer>& lstEncryptData)
{
    for (auto pack : lstData)
    {
        DWORD dwLen = 0;
        if (pack[0] == SMP_PACKET_FLAG)
            dwLen = SMP::GetSMPPacketLen(pack);
        else
            dwLen = TDS::GetTdsPacketLength(pack);

        EncryptMessage(GetServerTLSCtx(), &m_TLSSizesToClient, pack, dwLen, lstEncryptData);
    }
}

void ProxyChannel::EncryptServerMessage(PBYTE pData, DWORD dwDataLen, std::list<SecBuffer>& lstEncryptData)
{
    EncryptMessage(GetServerTLSCtx(), &m_TLSSizesToServer, pData, dwDataLen, lstEncryptData);
}

//Note： lstEncryptData must be freed after used.
DWORD ProxyChannel::EncryptMessage(CtxtHandle* tlsCtx, SecPkgContext_StreamSizes* tlsSize, PBYTE pInBuf, DWORD dwBufLen, std::list<SecBuffer>& lstEncryptData)
{
	SECURITY_STATUS scRet;         // unsigned long cbBuffer;    // Size of the buffer, in bytes
	SecBufferDesc   Message;       // unsigned long BufferType;  // Type of the buffer (below)
	SecBuffer       Buffers[4];    // void    SEC_FAR * pvBuffer;   // Pointer to the buffer
	DWORD           cbMessage;
	PBYTE           pbMessage;
	PBYTE           pbRemainData = pInBuf;
	DWORD           cbRemainDataLen = dwBufLen;
	BOOL            bRes = TRUE;

	while (cbRemainDataLen>0)
	{
		cbMessage = (cbRemainDataLen > tlsSize->cbMaximumMessage) ? tlsSize->cbMaximumMessage : cbRemainDataLen;

		//Allocate TLS message buffer
		DWORD cbIoBufferLength = tlsSize->cbHeader  +  cbMessage  +  tlsSize->cbTrailer;
		PBYTE pbIoBuffer       = (PBYTE) LocalAlloc(LMEM_FIXED, cbIoBufferLength);

		//fill content
		memcpy(pbIoBuffer+tlsSize->cbHeader, pbRemainData, cbMessage); 

		pbMessage = pbIoBuffer + tlsSize->cbHeader; // Offset by "header size"

		// Encrypt the HTTP request.
		Buffers[0].pvBuffer     = pbIoBuffer;                 // Pointer to buffer 1
		Buffers[0].cbBuffer     = tlsSize->cbHeader;          // length of header
		Buffers[0].BufferType   = SECBUFFER_STREAM_HEADER;    // Type of the buffer 

		Buffers[1].pvBuffer     = pbMessage;                  // Pointer to buffer 2
		Buffers[1].cbBuffer     = cbMessage;                  // length of the message
		Buffers[1].BufferType   = SECBUFFER_DATA;             // Type of the buffer 

		Buffers[2].pvBuffer     = pbMessage + cbMessage;      // Pointer to buffer 3
		Buffers[2].cbBuffer     = tlsSize->cbTrailer;         // length of the trailor
		Buffers[2].BufferType   = SECBUFFER_STREAM_TRAILER;   // Type of the buffer 

		Buffers[3].pvBuffer     = SECBUFFER_EMPTY;            // Pointer to buffer 4
		Buffers[3].cbBuffer     = SECBUFFER_EMPTY;            // length of buffer 4
		Buffers[3].BufferType   = SECBUFFER_EMPTY;            // Type of the buffer 4 

		Message.ulVersion       = SECBUFFER_VERSION;          // Version number
		Message.cBuffers        = 4;                          // Number of buffers - must contain four SecBuffer structures.
		Message.pBuffers        = Buffers;                    // Pointer to array of buffers
		
        scRet = ProxyManager::GetInstance()->SecurityFunTable->EncryptMessage(tlsCtx, 0, &Message, 0); // must contain four SecBuffer structures.
		if(FAILED(scRet)) 
		{
            PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by EncryptMessage\n", scRet);
			bRes = FALSE;
			if (pbIoBuffer){
				LocalFree(pbIoBuffer);
				pbIoBuffer = NULL;
			}
			goto CLEAN_UP;
		}

		//
		{
			SecBuffer dataBuf = { Buffers[0].cbBuffer + Buffers[1].cbBuffer + Buffers[2].cbBuffer, SECBUFFER_DATA, pbIoBuffer };
			lstEncryptData.push_back( dataBuf );
		}


		//update
		cbRemainDataLen -= cbMessage;
		pbRemainData += cbMessage;
	}

CLEAN_UP:
	return 0;
}

void ProxyChannel::CopyTLSSize(SecPkgContext_StreamSizes* dstSize, SecPkgContext_StreamSizes* srcSize)
{
	memcpy(dstSize, srcSize, sizeof(SecPkgContext_StreamSizes) );
}

void ProxyChannel::ProcessCachedTLSData(DWORD dwCacheType)
{
	/*if (dwCacheType&CACHED_TLS_CLIENT_DATA && m_dwTLSCacheType&CACHED_TLS_CLIENT_DATA){
       theSocketDataMgr->ProcessCachedData(GetClientSocket().get());
	}
	else if (dwCacheType&CACHED_TLS_SERVER_DATA && m_dwTLSCacheType&CACHED_TLS_SERVER_DATA){
		theSocketDataMgr->ProcessCachedData(GetServerSocket().get());
	}*/
}

// by jie 2018.05.21
bool ProxyChannel::PushBackReqNeedDispatch(boost::shared_ptr<ProxyTask> req)
{
	if (req->IsServerMessage() || IsChannelClose())
		return false;

	CriticalSectionLock lock(&m_csListReqNeedDispatch);
    if (IsChannelClose())
        return false;

    if (m_mapReqNeedDispatch.find(req->GetSessionID()) != m_mapReqNeedDispatch.end())
    {
        m_mapReqNeedDispatch[req->GetSessionID()].push_back(req);
    }
    else
    {
        std::list<boost::shared_ptr<ProxyTask>> tasks;
        tasks.push_back(req);
        m_mapReqNeedDispatch.insert(std::make_pair(req->GetSessionID(), tasks));
    }
	return true;
}

void ProxyChannel::DispatchReq(uint32_t sid)
{
    CriticalSectionLock lock(&m_csListReqNeedDispatch);
    if (m_mapReqNeedDispatch.find(sid) == m_mapReqNeedDispatch.end())
        return;

    std::list<boost::shared_ptr<ProxyTask>>& tasks = m_mapReqNeedDispatch[sid];
    std::list<boost::shared_ptr<ProxyTask>>::iterator it = tasks.begin();
    while (tasks.size() > 0 && it != tasks.end() && !IsChannelClose())
    {
        if (!((*it)->GetDispatchReady())) {
            it++;
            break;
        }

        boost::system::error_code errorcode;
        std::list<PBYTE>& tdsPackets = (*it)->GetTdsPacketList();
        if (tdsPackets.size() == 0) {
            it = tasks.erase(it);
            continue;
        }

        PBYTE pack = tdsPackets.front();
        if (pack[0] == SMP_PACKET_FLAG)
        {
            if (pack[1] & SMP_DATA) {
                SetSessionRequestSeqnum(sid, SMP::GetSMPSeqnum(tdsPackets.back()));
            }

            if (SMP::GetSMPPacketLen(pack) == 16)
            {
                if (m_encryptOption == 1 || m_encryptOption == 3)
                    TransferEncryptedRequest((*it).get(), tdsPackets, errorcode);
                else
                    TransferOriginalRequest((*it).get(), tdsPackets, errorcode);
            }
            else if (SMP::GetSMPPacketLen(pack) > 16) 
            {
                if (TDS::GetTdsPacketType(pack + 16) == TDS_LOGIN7) {
                    TransferEncryptedRequest((*it).get(), tdsPackets, errorcode);
                }
                else {
                    if (m_encryptOption == 1 || m_encryptOption == 3)
                        TransferEncryptedRequest((*it).get(), tdsPackets, errorcode);
                    else
                        TransferOriginalRequest((*it).get(), tdsPackets, errorcode);
                }
            }
        }
        else
        {
            if (TDS::GetTdsPacketType(tdsPackets.front()) == TDS_LOGIN7) {
                TransferEncryptedRequest((*it).get(), tdsPackets, errorcode);
            }
            else {
                if (m_encryptOption == 1 || m_encryptOption == 3)
                    TransferEncryptedRequest((*it).get(), tdsPackets, errorcode);
                else
                    TransferOriginalRequest((*it).get(), tdsPackets, errorcode);
            }
        }

        if (!errorcode)
        {
            
            if (pack[0] == SMP_PACKET_FLAG && (pack[1] == SMP_SYN || pack[1] == SMP_FIN))
            {

            }
            else
            {
                CriticalSectionLock lock(&m_csListReqDispatched);
                if (m_mapReqDispatched.find(sid) != m_mapReqDispatched.end())
                {
                    m_mapReqDispatched[sid].push_back(*it);
                }
                else
                {
                    std::list<boost::shared_ptr<ProxyTask>> ltasks;
                    ltasks.push_back(*it);
                    m_mapReqDispatched.insert(std::make_pair(sid, ltasks));
                }
            }
        }
        else
        {
            PROXYLOG(CELOG_ERR, "Request send failed! err: %s", errorcode.message().c_str());
        }

        it = tasks.erase(it);
    }
}

void ProxyChannel::TransferEncryptedRequest(ProxyTask* task, std::list<PBYTE>& packets, boost::system::error_code& err)
{
    if (packets.front()[0] == SMP_PACKET_FLAG)
    {
        uint32_t wndw = GetSessionAckWndwFromServer(task->GetSessionID());
        for (auto it : packets)
        {
            if (it[1] & SMP_DATA)
            {
                if (SMP::GetSMPSeqnum(it) > wndw)
                {
                    //WaitForSessionDispEvent(task->GetSessionID(), 5000);
                    wndw = GetSessionAckWndwFromServer(task->GetSessionID());
                }
            }

            std::list<SecBuffer> lstEncPacks;
            EncryptClientMessage(it, SMP::GetSMPPacketLen(it), lstEncPacks);

            for (auto& enc_data : lstEncPacks)
            {
                theTCPFrame->BlockSendData(GetServerSocket(), (PBYTE)enc_data.pvBuffer, enc_data.cbBuffer, err);

                LocalFree(enc_data.pvBuffer);
                enc_data.pvBuffer = NULL;
                enc_data.cbBuffer = 0;
            }
        }
    }
    else
    {
        std::list<SecBuffer> lstEncryptedPacket;
        EncryptClientMessage(packets, lstEncryptedPacket);

        std::list<SecBuffer>::iterator itData = lstEncryptedPacket.begin();
        while (itData != lstEncryptedPacket.end())
        {
            theTCPFrame->BlockSendData(GetServerSocket(), (PBYTE)itData->pvBuffer, itData->cbBuffer, err);

            LocalFree(itData->pvBuffer);
            itData->pvBuffer = NULL;
            itData->cbBuffer = 0;

            itData++;
        }
    }
}

void ProxyChannel::TransferOriginalRequest(ProxyTask* task, std::list<PBYTE>& packets, boost::system::error_code& err)
{
    uint32_t wndw = GetSessionAckWndwFromServer(task->GetSessionID());
    for (auto itPack : packets)
    {
        uint32_t len = 0;
        if (itPack[0] == SMP_PACKET_FLAG) {
            len = SMP::GetSMPPacketLen(itPack);
            if (itPack[1] & SMP_DATA) {
                if (SMP::GetSMPSeqnum(itPack) > wndw) {
                    //PROXYLOG(CELOG_DEBUG, "TransferOriginalRequest need to wait ack [%d >= %d]", SMP::GetSMPSeqnum(itPack), wndw);
                    //WaitForSessionDispEvent(task->GetSessionID(), 5000);
                    wndw = GetSessionAckWndwFromServer(task->GetSessionID());
                }
            }
        }
        else
            len = TDS::GetTdsPacketLength(itPack);

        theTCPFrame->BlockSendData(GetServerSocket(), itPack, len, err);
    }
}

bool ProxyChannel::PushBackRespNeedDispatch(boost::shared_ptr<ProxyTask> resp)
{
	if (!resp->IsServerMessage() || IsChannelClose())
		return false;

	{
		CriticalSectionLock lock(&m_csListReqDispatched);
        if (IsChannelClose())
            return false;

        if (m_mapReqDispatched.find(resp->GetSessionID()) != m_mapReqDispatched.end())
        {
            std::list<boost::shared_ptr<ProxyTask>>& tasks = m_mapReqDispatched[resp->GetSessionID()];
            if (tasks.size() > 0)
            {
                resp->SetReq(tasks.front());
                tasks.pop_front();
            }
        }
	}

	{
		CriticalSectionLock lock(&m_csListRespNeedDispatch);
        if (IsChannelClose())
            return false;

        if (m_mapRespNeedDispatch.find(resp->GetSessionID()) != m_mapRespNeedDispatch.end())
        {
            m_mapRespNeedDispatch[resp->GetSessionID()].push_back(resp);
        }
        else
        {
            std::list<boost::shared_ptr<ProxyTask>> tasks;
            tasks.push_back(resp);
            m_mapRespNeedDispatch.insert(std::make_pair(resp->GetSessionID(), tasks));
        }
	}
	return true;
}

void ProxyChannel::DispatchResp(uint32_t sid)
{
    if (m_encryptOption == 1 || m_encryptOption == 3)
        TransferEncryptedResponse(sid);
    else
        TransferOriginalResponse(sid);
}

void ProxyChannel::TransferEncryptedResponse(uint32_t sid)
{
    CriticalSectionLock lock(&m_csListRespNeedDispatch);
    if (m_mapRespNeedDispatch.find(sid) == m_mapRespNeedDispatch.end())
        return;

    std::list<boost::shared_ptr<ProxyTask>>& tasks = m_mapRespNeedDispatch[sid];
    std::list<boost::shared_ptr<ProxyTask>>::iterator it = tasks.begin();
    while (it != tasks.end() && (*it)->GetDispatchReady() && !IsChannelClose())
    {
        boost::system::error_code errorcode;
        std::list<PBYTE>& tdsPackets = (*it)->GetTdsPacketList();
        if (tdsPackets.size() == 0) {
            it = tasks.erase(it);
            continue;
        }
        
        PBYTE pack = tdsPackets.front();
        if (pack[0] == SMP_PACKET_FLAG && (pack[1] & SMP_DATA))
        {
            SetSessionResponseSeqnum(sid, SMP::GetSMPSeqnum(tdsPackets.back()));
        }

        if (pack[0] == SMP_PACKET_FLAG)
        {
            uint32_t wndw = GetSessionAckWndwFromClient(sid);
            for (auto pack : tdsPackets)
            {
                if (pack[1] & SMP_DATA)
                {
                    if (SMP::GetSMPSeqnum(pack) > wndw)
                    {
                        //WaitForSessionDispEvent(sid, 5000);
                        wndw = GetSessionAckWndwFromClient(sid);
                    }
                }

                std::list<SecBuffer> lstEncPacks;
                EncryptServerMessage(pack, SMP::GetSMPPacketLen(pack), lstEncPacks);

                for (auto& enc_data : lstEncPacks)
                {
                    theTCPFrame->BlockSendData(GetClientSocket(), (PBYTE)enc_data.pvBuffer, enc_data.cbBuffer, errorcode);

                    LocalFree(enc_data.pvBuffer);
                    enc_data.pvBuffer = NULL;
                    enc_data.cbBuffer = 0;
                }
            }
        }
        else
        {
            std::list<SecBuffer> lstEncryptedPacket;
            EncryptServerMessage(tdsPackets, lstEncryptedPacket);
            
            std::list<SecBuffer>::iterator itData = lstEncryptedPacket.begin();
            while (itData != lstEncryptedPacket.end())
            {
                theTCPFrame->BlockSendData(GetClientSocket(), (PBYTE)itData->pvBuffer, itData->cbBuffer, errorcode);

                LocalFree(itData->pvBuffer);
                itData->pvBuffer = NULL;
                itData->cbBuffer = 0;

                itData++;
            }
        }

        it = tasks.erase(it);
    }
}

void ProxyChannel::TransferOriginalResponse(uint32_t sid)
{
    CriticalSectionLock lock(&m_csListRespNeedDispatch);
    if (m_mapRespNeedDispatch.find(sid) == m_mapRespNeedDispatch.end())
        return;

    std::list<boost::shared_ptr<ProxyTask>>& tasks = m_mapRespNeedDispatch[sid];
    std::list<boost::shared_ptr<ProxyTask>>::iterator it = tasks.begin();
    while (it != tasks.end() && (*it)->GetDispatchReady() && !IsChannelClose())
    {
        boost::system::error_code errorcode;
        std::list<PBYTE>& tdsPackets = (*it)->GetTdsPacketList();
        if (tdsPackets.size() == 0) {
            it = tasks.erase(it);
            continue;
        }

        PBYTE pack = tdsPackets.front();
        if (pack[0] == SMP_PACKET_FLAG && (pack[1] & SMP_DATA))
            SetSessionResponseSeqnum(sid, SMP::GetSMPSeqnum(tdsPackets.back()));

        uint32_t wndw = GetSessionAckWndwFromClient(sid);
        for (auto itPack : tdsPackets)
        {
            uint32_t len = 0;
            if (itPack[0] == SMP_PACKET_FLAG) {
                SMPHeader smp;
                smp.Parse(itPack);
                len = smp.m_length;
                if (smp.m_flags & SMP_DATA) {
                    if (smp.m_seqnum > wndw) {
                        //PROXYLOG(CELOG_DEBUG, "TransferOriginalResponse need to wait ack [%d >= %d]", smp.m_seqnum, wndw);
                        //WaitForSessionDispEvent(sid, 5000);
                        wndw = GetSessionAckWndwFromClient(sid);
                    }
                }
            }
            else
                len = TDS::GetTdsPacketLength(itPack);

            theTCPFrame->BlockSendData(GetClientSocket(), itPack, len, errorcode);
        }

        it = tasks.erase(it);
    }
}

bool ProxyChannel::HasReqTask()
{
	CriticalSectionLock lock(&m_csListRespNeedDispatch);
    for (auto it : m_mapReqNeedDispatch)
    {
        if (it.second.size() > 0)
            return true;
    }

    return false;
}

bool ProxyChannel::HasRespTask()
{
	CriticalSectionLock lock(&m_csListReqNeedDispatch);
    for (auto it : m_mapRespNeedDispatch)
    {
        if (it.second.size() > 0)
            return true;
    }

    return false;
}

void ProxyChannel::ReplySMPAckToClient(uint32_t sid, uint32_t wndw)
{
    uint8_t smp_head[16] = { 0 };
    SMP::SerializeSMPHeader(smp_head, SMP_ACK, sid, 16, GetSessionResponseSeqnum(sid), wndw);

    if (m_encryptOption == 1 || m_encryptOption == 3)
    {
        std::list<SecBuffer> lstEncryptedPacket;
        std::list<PBYTE> tdsPackets;
        tdsPackets.push_back(smp_head);
        EncryptServerMessage(tdsPackets, lstEncryptedPacket);

        boost::system::error_code errorcode;
        std::list<SecBuffer>::iterator itData = lstEncryptedPacket.begin();
        while (itData != lstEncryptedPacket.end())
        {
            theTCPFrame->BlockSendData(GetClientSocket(), (PBYTE)itData->pvBuffer, itData->cbBuffer, errorcode);

            LocalFree(itData->pvBuffer);
            itData->pvBuffer = NULL;
            itData->cbBuffer = 0;

            itData++;
        }
    }
    else
    {
        boost::system::error_code errcode;
        theTCPFrame->BlockSendData(GetClientSocket(), smp_head, 16, errcode);
    }
}

void ProxyChannel::ReplySMPAckToServer(uint32_t sid, uint32_t wndw)
{
    uint8_t smp_head[16] = { 0 };
    SMP::SerializeSMPHeader(smp_head, SMP_ACK, sid, 16, GetSessionRequsetSeqnum(sid), wndw);

    if (m_encryptOption == 1 || m_encryptOption == 3)
    {
        std::list<SecBuffer> lstEncryptedPacket;
        std::list<PBYTE> tdsPackets;
        tdsPackets.push_back(smp_head);
        EncryptClientMessage(tdsPackets, lstEncryptedPacket);

        boost::system::error_code errorcode;
        std::list<SecBuffer>::iterator itData = lstEncryptedPacket.begin();
        while (itData != lstEncryptedPacket.end())
        {
            theTCPFrame->BlockSendData(GetServerSocket(), (PBYTE)itData->pvBuffer, itData->cbBuffer, errorcode);

            LocalFree(itData->pvBuffer);
            itData->pvBuffer = NULL;
            itData->cbBuffer = 0;

            itData++;
        }
    }
    else
    {
        boost::system::error_code errcode;
        theTCPFrame->BlockSendData(GetServerSocket(), smp_head, 16, errcode);
    }
}

void ProxyChannel::CachePreloginPackets(const std::list<PBYTE>& src)
{
	for (auto it : src)
	{
		USHORT length = TDS::GetTdsPacketLength(it);
		PBYTE buf = new BYTE[length];
		memcpy(buf, it, length);
		m_cachedPreloginPackets.push_back(buf);
	}
}

void ProxyChannel::CacheLogin7Packets(const std::list<PBYTE>& src)
{
	for (auto it : src)
	{
		USHORT length = TDS::GetTdsPacketLength(it);
		PBYTE buf = new BYTE[length];
		memcpy(buf, it, length);
		m_cachedLogin7Packets.push_back(buf);
	}
}

void ProxyChannel::CacheSSPIPackets(const std::list<PBYTE>& src)
{
    for (auto it : src)
    {
        USHORT length = TDS::GetTdsPacketLength(it);
        PBYTE buf = new BYTE[length];
        memcpy(buf, it, length);
        m_cachedSSPIPackets.push_back(buf);
    }
}

void ProxyChannel::ResetBeforeRouting()
{
	m_ServerSocket = NULL;
	m_bServerTLSFinished = false;
	m_bTLSCtxClientInited = false;
	m_dwInCompleteTLSBufServerLen = 0;
	m_dwTLSCacheType = 0;
	memset(&m_TLSCtxClient, 0, sizeof(m_TLSCtxClient));
}

BOOL ProxyChannel::ValidClientTdsPackage(PBYTE pBuf, DWORD dwLen)
{
	if (CHANNEL_STATUS_Wait_Client_First_Prelogin == GetChannelClientStatus())
	{
		if (pBuf[0]!=TDS_PRELOGIN){
			return false;
		}
	}

	return true;
}

void ProxyChannel::SetChannelClose(bool b)
{
    m_bChannelClose = b;

    if (b)
    {
        {
            CriticalSectionLock lock(&m_csListReqNeedDispatch);
            for (auto itSession : m_mapReqNeedDispatch)
            {
                itSession.second.clear();
            }
            m_mapReqNeedDispatch.clear();
        }

        {
            CriticalSectionLock lock(&m_csListRespNeedDispatch);
            for (auto itSession : m_mapRespNeedDispatch)
            {
                itSession.second.clear();
            }
            m_mapRespNeedDispatch.clear();
        }

        {
            CriticalSectionLock lock(&m_csListReqDispatched);
            for (auto itSession : m_mapReqDispatched)
            {
                itSession.second.clear();
            }
            m_mapReqDispatched.clear();
        }
    }
}