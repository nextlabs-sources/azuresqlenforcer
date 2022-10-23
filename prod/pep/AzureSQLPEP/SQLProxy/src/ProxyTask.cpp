#include "ProxyTask.h"
#include "CommonFunc.h"
#include "Config.h"
#include "TCPFrame.h"
#include "ProxyManager.h"
#include "ProxyDataAnalysis.h"
#include "CriticalSectionLock.h"
#include "TDS.h"
#include "Log.h"
#include "Base64.h"

#include <credssp.h>
#include <ntsecapi.h>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

extern HMODULE g_hThisModule;

ProxyTask::ProxyTask(void* tcpsocket)
    :m_tcpSocket(tcpsocket)
    ,m_IsServerMsg(false)
    ,m_bCompleteTask(FALSE)
    ,m_TdsTaskReq(NULL)
    ,m_bUseNewListTdsPacket(FALSE)
    ,m_bDispatchReady(FALSE)
    ,m_session_id(SESSION_ID_IF_NO_SMP)
{

}

ProxyTask::~ProxyTask()
{
    if (IsServerMessage()) {
        m_TdsTaskReq = nullptr;
    }

    std::list<uint8_t*>::iterator itData = m_lstTdsPacket.begin();
    for (;itData != m_lstTdsPacket.end(); itData++)
    {
        delete[] (*itData);
    }

    std::list<uint8_t*>::iterator itDataNew = m_lstTdsPacketNew.begin();
    for (;itDataNew != m_lstTdsPacketNew.end(); itDataNew++)
    {
        delete[] (*itDataNew);
    }

    m_lstTdsPacket.clear();
    m_lstTdsPacketNew.clear();
}

void ProxyTask::AddedTdsPacket(PBYTE pTdsPacket)
{
    if (pTdsPacket[0] == SMP_PACKET_FLAG) {
        SMPHeader smp;
        smp.Parse(pTdsPacket);

        if (smp.m_flags & SMP_DATA)
        {
            if (!m_IsServerMsg)
            {
                uint32_t ack_wndw = m_ProxyChannel->GetSessionAckWndwToClient(m_session_id);
                if (smp.m_seqnum >= ack_wndw - 2)
                {
                    ack_wndw += 4;
                    m_ProxyChannel->ReplySMPAckToClient(m_session_id, ack_wndw);
                    m_ProxyChannel->SetSessionAckWndwToClient(m_session_id, ack_wndw);
                }
            }
            else if (m_IsServerMsg)
            {
                uint32_t ack_wndw = m_ProxyChannel->GetSessionAckWndwToSever(m_session_id);
                if (smp.m_seqnum >= ack_wndw - 2)
                {
                    ack_wndw += 4;
                    m_ProxyChannel->ReplySMPAckToServer(m_session_id, ack_wndw);
                    m_ProxyChannel->SetSessionAckWndwToSever(m_session_id, ack_wndw);
                }
            }
        }
    }
    m_lstTdsPacket.push_back(pTdsPacket);

    if (theConfig.NeedPrintSocketData())
    {
        uint32_t pack_len = 0;
        if (pTdsPacket[0] == SMP_PACKET_FLAG)
            pack_len = SMP::GetSMPPacketLen(pTdsPacket);
        else
            pack_len = TDS::GetTdsPacketLength(pTdsPacket);

        SYSTEMTIME st;
        GetLocalTime(&st);
        char msg[100] = {0};
        sprintf_s(msg, "%04d-%02d-%02d %02d:%02d:%02d.%d channel: %p\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, m_ProxyChannel.get());
        std::string log_str(msg);
        log_str.append(ProxyCommon::BufferToHexView(pTdsPacket, pack_len, true, 16));
        log_str.append("\n\n");
        
        ProxyCommon::PrintSocketData(log_str);
    }
}

void ProxyTask::Execute()
{
    if ((m_ProxyChannel.get() && m_ProxyChannel->IsChannelClose()) || m_lstTdsPacket.size() == 0)
        return;

    if (!m_IsServerMsg)
    {
        ProcessClientMessage();
    }
    else
    {
        ProcessServerMessage();
    }
}

void ProxyTask::ProcessClientMessage()
{
    CHANNEL_STATUS channelStatus = m_ProxyChannel->GetChannelClientStatus();
    if (CHANNEL_STATUS_Wait_Client_First_Prelogin == channelStatus)
    {
        boost::system::error_code errorcode;
        // send first prelogin package to server
        BYTE *pPacket = *m_lstTdsPacket.begin();
        theTCPFrame->BlockSendData(m_ProxyChannel->GetServerSocket(), pPacket, TDS::GetTdsPacketLength(pPacket), errorcode);
        m_ProxyChannel->SetChannelClientStatus(CHANNEL_STATUS_TLS_Handle_Shake);
        m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_Wait_Server_First_Prelogin_Response);
        m_ProxyChannel->CachePreloginPackets(m_lstTdsPacket);
    }
    else if (CHANNEL_STATUS_TLS_Handle_Shake == channelStatus)
    {//process tls handshake with client
        SECURITY_STATUS status = TLSHandShakeWithClient();
        if (status == SEC_E_OK) {
            m_ProxyChannel->SetChannelClientStatus(CHANNEL_STATUS_Proxy_Success);
            m_ProxyChannel->SetClientTLSFinished(TRUE);
            m_ProxyChannel->ProcessCachedTLSData(CACHED_TLS_CLIENT_DATA);


            //get TLS channel property
            SecPkgContext_StreamSizes TLSSizesToClient;            // unsigned long cbBuffer;    // Size of the buffer, in bytes
            ProxyManager::GetInstance()->SecurityFunTable->QueryContextAttributes(m_ProxyChannel->GetServerTLSCtx(), SECPKG_ATTR_STREAM_SIZES, &TLSSizesToClient);
            PROXYLOG(CELOG_DEBUG, L"SECPKG_ATTR_STREAM_SIZES for Client,Sizes.cbHeader:%d, Sizes.cbMaximumMessage:%d,Sizes.cbTrailer=%d\n",
                TLSSizesToClient.cbHeader, TLSSizesToClient.cbMaximumMessage, TLSSizesToClient.cbTrailer);
            m_ProxyChannel->SetClientTLSSizeProperty(&TLSSizesToClient);

            //init TLS handshake with remote server
            TLSHandShakeWithServer(TRUE);
        }
    }
    else if (CHANNEL_STATUS_Proxy_Success == channelStatus)
    {
        bool bSmpPack = false;
        TDS_MSG_TYPE tdsType = TDS_Unkown;
        uint8_t* pack = m_lstTdsPacket.front();
        if (pack[0] == SMP_PACKET_FLAG) {
            
            bSmpPack = true;

            if (SMP::GetSMPPacketLen(pack) == 16) {
                SetUseNewListTdsPacket(false);
                SetDispatchReady(true);
                m_ProxyChannel->DispatchReq(m_session_id);
                return;
            }
            else if (SMP::GetSMPPacketLen(pack) > 16)
                tdsType = TDS::GetTdsPacketType(pack + 16);
        }
        else
            tdsType = TDS::GetTdsPacketType(pack);
        //PROXYLOG(CELOG_DEBUG, L"Client Request type: %s", TDS::GetTdsPacketTypeString(tdsType));
        if (tdsType == TDS_LOGIN7)
        {
            ProcessLogin7Request();
        }
        else if (tdsType == TDS_FEDAUTH)
        {
            ProcessFedauthRequest();
        }
        else if (tdsType == TDS_SSPI_LOGIN7)
        {
            ProcessSSPIRequest();
        }
        else
        {
            SetUseNewListTdsPacket(false);

            if (ProxyDataAnalysis::GetInstance()->IsInited())
            {
                // fill enforce params
                EnforceParams eparams;
                if (m_ProxyChannel->GetClientSocket()) {
                    boost::system::error_code errcode;
                    auto ip = m_ProxyChannel->GetClientSocket()->socket().remote_endpoint(errcode);
                    if (!errcode) {
                        eparams.user_ip = ip.address().to_string();
                    }
                }

                eparams.sql_user = m_ProxyChannel->GetLoginUser();
                eparams.domain_name = m_ProxyChannel->GetLoginDomain();
                eparams.computer_name = m_ProxyChannel->GetLoginHostname();
                eparams.database = m_ProxyChannel->GetLoginDefaultDatabase();
                eparams.appname = m_ProxyChannel->GetLoginAppname();
                eparams.smp_last_request_seqnum = m_ProxyChannel->GetSessionRequsetSeqnum(m_session_id);

                ProxyDataAnalysis::GetInstance()->ProcessClientPacket(m_lstTdsPacket, m_lstTdsPacketNew, m_ProxyChannel->GetNegotiatePacketSize(), eparams);

                if (m_lstTdsPacketNew.size() > 0) {
                    SetUseNewListTdsPacket(true);
                }

                if (bSmpPack && (SMP::GetSMPFlag(m_lstTdsPacket.front()) & SMP_DATA))
                {
                    uint32_t wndw = m_ProxyChannel->GetSessionWndwToServer(m_session_id);
                    if (m_lstTdsPacketNew.size() > 0)
                    {
                        for (auto pack : m_lstTdsPacketNew)
                        {
                            *(uint32_t*)(pack + 12) = wndw++;
                        }
                        m_ProxyChannel->SetSessionWndwToServer(m_session_id, wndw);
                    }
                    else
                    {
                        for (auto pack : m_lstTdsPacket)
                        {
                            *(uint32_t*)(pack + 12) = wndw++;
                        }
                        m_ProxyChannel->SetSessionWndwToServer(m_session_id, wndw);
                    }
                }

                if (!eparams.database.empty() && _stricmp(m_ProxyChannel->GetLoginDefaultDatabase().c_str(), eparams.database.c_str()) != 0)
                    m_ProxyChannel->SetLoginDefaultDatabase(eparams.database);
            }
        }

        SetDispatchReady(true);
        m_ProxyChannel->DispatchReq(m_session_id);
    }
}

void ProxyTask::ProcessLogin7Request()
{
    DWORD dwWaitResult = m_ProxyChannel->WaitTLSHandShakeFinishWithRemoteServer();
    if (dwWaitResult == WAIT_OBJECT_0)
    {
        //parse login request
        tdsLogin7 tdsLogin(m_lstTdsPacket);

        PROXYLOG(CELOG_DEBUG, L"\nLogin7 Info:\n"
                             L"      HostName: %s\n"
                             L"      UserName: %s\n"
                             L"       AppName: %s\n"
                             L"    ServerName: %s\n"
                             L"      Language: %s\n"
                             L"      DataBase: %s\n"
                             L"    CltIntName: %s\n"
                             L"        cbSSPI: %u\n"
                             L"      SSPILong: %u\n"
                             L"       CientID: %s\n"
                             L"  AttachDBFile: %s\n"
                             ,tdsLogin.GetHostName()
                             ,tdsLogin.GetUserUniqueName()
                             ,tdsLogin.GetAppName()
                             ,tdsLogin.GetServerName()
                             ,tdsLogin.GetLanguage()
                             ,tdsLogin.GetDataBase()
                             ,tdsLogin.GetCltIntName()
                             ,tdsLogin.GetcbSSPI()
                             ,tdsLogin.GetSSPILong()
                             ,tdsLogin.ClientID2String().c_str()
                             ,tdsLogin.GetAtachDBFile());

        m_ProxyChannel->SetLoginUser(ProxyCommon::UnicodeToUTF8(tdsLogin.GetUserUniqueName()));
        m_ProxyChannel->SetLoginHostname(ProxyCommon::UnicodeToUTF8(tdsLogin.GetHostName()));
        m_ProxyChannel->SetLoginAppname(ProxyCommon::UnicodeToUTF8(tdsLogin.GetAppName()));
        m_ProxyChannel->SetLoginDefaultDatabase(ProxyCommon::UnicodeToUTF8(tdsLogin.GetDataBase()));

        wchar_t szTargetServer[1024];
        wsprintfW(szTargetServer, L"tcp:%s,%s", theConfig.RemoteServer().c_str(), theConfig.RemoteServerPort().c_str());
        tdsLogin.SetServerName(szTargetServer);

        if (tdsLogin.IsKerberosAuth() && theConfig.kerberosAuthOpen())
        {
            PROXYLOG(CELOG_NOTICE, "This is a kerberos authentication.");
            std::vector<BYTE> ticket;
            tdsLogin.GetKerberosTicket(ticket);

            if (ticket.size() > 0)
            {
                CtxtHandle clientCtx = { 0 };
                CtxtHandle serverCtx = { 0 };

                do 
                {
                    if (!CheckClientKrbTicket(ticket, &clientCtx))
                    {
                        PROXYLOG(CELOG_WARNING, "Check client kerberos ticket failed");
                        break;
                    }

                    SecPkgContext_NamesW pkb = {0};
                    SECURITY_STATUS query_status = ProxyManager::GetInstance()->SecurityFunTable->QueryContextAttributesW(&clientCtx, SECPKG_ATTR_NAMES, &pkb);
                    if (query_status == SEC_E_OK)
                    {
                        PROXYLOG(CELOG_NOTICE, L"Kerberos logon user: %s", pkb.sUserName);
                        m_ProxyChannel->SetLoginUser(ProxyCommon::UnicodeToUTF8(pkb.sUserName));
                    }
                    else
                    {
                        if (pkb.sUserName)
                            ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(pkb.sUserName);
                        
                        PROXYLOG(CELOG_ERR, "Try to query kerberos auth info failed. result: %#x", query_status);
                        break;
                    }

                    if (pkb.sUserName)
                        ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(pkb.sUserName);

                    if (!CreateKrbAuthTicketForServer(ticket, &serverCtx))
                    {
                        PROXYLOG(CELOG_WARNING, "Create kerberos ticket for server failed");
                        break;
                    }

                    tdsLogin.SetKerberosTicket(ticket);

                } while (0);

                if (clientCtx.dwLower != 0 || clientCtx.dwUpper != 0)
                    ProxyManager::GetInstance()->SecurityFunTable->DeleteSecurityContext(&clientCtx);
                if (serverCtx.dwLower != 0 || serverCtx.dwUpper != 0)
                    ProxyManager::GetInstance()->SecurityFunTable->DeleteSecurityContext(&serverCtx);
            }
            else
            {
                PROXYLOG(CELOG_WARNING, "Can not get ticket data from kerberos authentication");
            }
        }

        //PBYTE SerializeBuf = new BYTE[6000];
        //DWORD dwSendBufLen;
        //tdsLogin.Serialize(SerializeBuf, 6000, &dwSendBufLen);
        //m_lstTdsPacketNew.push_back(SerializeBuf);
        tdsLogin.Serialize(m_lstTdsPacketNew);
        SetUseNewListTdsPacket(true);

        m_ProxyChannel->CacheLogin7Packets(m_lstTdsPacket);
    }
    else
    {
        PROXYLOG(CELOG_WARNING, "Login wait for TLS handshake success time out");
    }
}

void ProxyTask::ProcessFedauthRequest()
{
    try
    {
        tdsFedauth fedauth;
        fedauth.Parse(m_lstTdsPacket.front());
        std::wstring in((WCHAR*)fedauth.FedAuthToken.GetPValue(), (WCHAR*)(fedauth.FedAuthToken.GetPValue() + fedauth.FedAuthToken.GetCount()));
        std::string a = ProxyCommon::WStringToString(in);

        // this is a jwt( Header(base64). Payload(base64). Signature)
        std::vector<std::string> fields;
        boost::split(fields, a, boost::is_any_of("."));
        if (fields.size() < 2)
            return;
        //auto f0 = base64_decode(fields[0]);
        auto t = base64_decode(fields[1]);	// get Payload
        //auto f2 = base64_decode(fields[2]);
        
        //std::string str0(f0.begin(), f0.end());
        std::string jsonStr(t.begin(), t.end());
        //std::string str2(f2.begin(), f2.end());

        boost::property_tree::ptree pt;
        std::stringstream ss(jsonStr);
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, pt);
        std::string name;
        //if (pt.to_iterator(pt.find("unique_name")) != pt.end())
            name = pt.get<std::string>("unique_name");	// read unique_name
        //else if (pt.to_iterator(pt.find("sub")) != pt.end())
        //    name = pt.get<std::string>("sub");

        if (!name.empty())
            m_ProxyChannel->SetLoginUser(name);
    }
    catch (std::exception e)
    {
        PROXYLOG(CELOG_WARNING, "get unique_name from FedauthRequest packet exception: %s", e.what());
    }
}

void ProxyTask::ProcessSSPIRequest()
{
    tdsSSPI sspi;
    sspi.Parse(m_lstTdsPacket.front());

    PROXYLOG(CELOG_DEBUG, L"\nSSPI Info:\n"
                          L"     Domain name: %s\n"
                          L"       User name: %s\n"
                          L"       Host name: %s\n"
                          ,sspi.GetDomain()
                          ,sspi.GetUsername()
                          ,sspi.GetHostname());

    if (sspi.GetUsername() && wcslen(sspi.GetUsername()) > 0)
        m_ProxyChannel->SetLoginUser(ProxyCommon::UnicodeToUTF8(sspi.GetUsername()));
    if (sspi.GetDomain() && wcslen(sspi.GetDomain()) > 0)
        m_ProxyChannel->SetLoginDomain(ProxyCommon::UnicodeToUTF8(sspi.GetDomain()));
    if (sspi.GetHostname() && wcslen(sspi.GetHostname()) > 0)
        m_ProxyChannel->SetLoginHostname(ProxyCommon::UnicodeToUTF8(sspi.GetHostname()));

    m_ProxyChannel->CacheSSPIPackets(m_lstTdsPacket);
}

void ProxyTask::ProcessServerMessage()
{
    CHANNEL_STATUS channelStatus = m_ProxyChannel->GetChannelServerStatus();
    if (CHANNEL_STATUS_Wait_Server_First_Prelogin_Response == channelStatus)
    {//send the first prelogin response back to client
        boost::system::error_code errorcode;
        BYTE* pPacket = *m_lstTdsPacket.begin();

        TdsPreLogin prelogin;
        prelogin.Parse(pPacket);
        m_ProxyChannel->SetEncryptionOption(prelogin.GetEncryptFlag());

        theTCPFrame->BlockSendData(m_ProxyChannel->GetClientSocket(), pPacket, TDS::GetTdsPacketLength(pPacket), errorcode);

        //
        m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_TLS_Handle_Shake);
    }
    else if (CHANNEL_STATUS_Wait_Server_First_Prelogin_Response_Routing == channelStatus)
    {
        m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_TLS_Handle_Shake_Routing);
        bool ret = TLSHandShakeWithServer(TRUE);
    }
    else if (CHANNEL_STATUS_TLS_Handle_Shake_Routing == channelStatus)
    {
        SECURITY_STATUS sec = TLSHandShakeWithServer(FALSE);
        if (SEC_E_OK == sec) {
            m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_Proxy_Success);
            m_ProxyChannel->SetServerTLSFinished(TRUE);
            m_ProxyChannel->ProcessCachedTLSData(CACHED_TLS_SERVER_DATA);

            //get TLS channel property
            SecPkgContext_StreamSizes TLSSizesToServer;            // unsigned long cbBuffer;    // Size of the buffer, in bytes
            ProxyManager::GetInstance()->SecurityFunTable->QueryContextAttributes(m_ProxyChannel->GetClientTLSCtx(), SECPKG_ATTR_STREAM_SIZES, &TLSSizesToServer);
            PROXYLOG(CELOG_DEBUG, L"SECPKG_ATTR_STREAM_SIZES for Server,Sizes.cbHeader:%d, Sizes.cbMaximumMessage:%d,Sizes.cbTrailer=%d\n",
                TLSSizesToServer.cbHeader, TLSSizesToServer.cbMaximumMessage, TLSSizesToServer.cbTrailer);
            m_ProxyChannel->SetServerTLSSizeProperty(&TLSSizesToServer);

            //notify
            //m_ProxyChannel->SetTLSHandShakeFinishWithRemoteServer();

            //send
            tdsLogin7 tdsLogin(m_ProxyChannel->GetCachedLogin7Packets());
            wchar_t szTargetServer[1024];

            wsprintfW(szTargetServer, L"tcp:%s,%s", m_ProxyChannel->GetFinalRemoteServer().c_str(), m_ProxyChannel->GetFinalRemoteServerPort().c_str());
            tdsLogin.SetServerName(szTargetServer);

            tdsLogin.Serialize(m_lstTdsPacketNew);
            SetUseNewListTdsPacket(true);

            boost::system::error_code errorcode;
            std::list<PBYTE>& tdsPackets = m_lstTdsPacketNew;

            std::list<SecBuffer> lstEncryptedPacket;
            m_ProxyChannel->EncryptClientMessage(tdsPackets, lstEncryptedPacket);

            std::list<SecBuffer>::iterator itData = lstEncryptedPacket.begin();
            for (; itData != lstEncryptedPacket.end(); itData++)
            {
                theTCPFrame->BlockSendData(m_ProxyChannel->GetServerSocket(), (PBYTE)itData->pvBuffer, itData->cbBuffer, errorcode);

                LocalFree(itData->pvBuffer);
                itData->pvBuffer = NULL;
                itData->cbBuffer = 0;
            }
        }
    }
    else if (CHANNEL_STATUS_TLS_Handle_Shake == channelStatus)
    {
        SECURITY_STATUS sec = TLSHandShakeWithServer(FALSE);
        if (SEC_E_OK == sec) {
            m_ProxyChannel->SetServerTLSFinished(TRUE);
            if (CHANNEL_STATUS_TLS_Handle_Shake == channelStatus)
                m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_Proxy_Success);
            else if (CHANNEL_STATUS_TLS_Handle_Shake_Routing == channelStatus)
            {
                m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_Proxy_Success_Routing);
            }
            m_ProxyChannel->ProcessCachedTLSData(CACHED_TLS_SERVER_DATA);

            //get TLS channel property
            SecPkgContext_StreamSizes TLSSizesToServer;            // unsigned long cbBuffer;    // Size of the buffer, in bytes
            ProxyManager::GetInstance()->SecurityFunTable->QueryContextAttributes(m_ProxyChannel->GetClientTLSCtx(), SECPKG_ATTR_STREAM_SIZES, &TLSSizesToServer);

            PROXYLOG(CELOG_DEBUG, L"SECPKG_ATTR_STREAM_SIZES for Server,Sizes.cbHeader:%d, Sizes.cbMaximumMessage:%d,Sizes.cbTrailer=%d\n",
                TLSSizesToServer.cbHeader, TLSSizesToServer.cbMaximumMessage, TLSSizesToServer.cbTrailer);
            
            m_ProxyChannel->SetServerTLSSizeProperty(&TLSSizesToServer);

            //notify
            m_ProxyChannel->SetTLSHandShakeFinishWithRemoteServer();
        }
    }
    else if (CHANNEL_STATUS_Proxy_Success == channelStatus)
    {
        TDS_MSG_TYPE tdsType = TDS_Unkown;
        uint8_t* pack = m_lstTdsPacket.front();
        if (pack[0] == SMP_PACKET_FLAG) {
            if (SMP::GetSMPPacketLen(pack) == 16) {
                SetUseNewListTdsPacket(false);
                SetDispatchReady(true);
                m_ProxyChannel->DispatchResp(m_session_id);
                return;
            }
            else if (SMP::GetSMPPacketLen(pack) > 16) {
                tdsType = TDS::GetTdsPacketType(pack + 16);
            }
        }
        else {
            tdsType = TDS::GetTdsPacketType(*m_lstTdsPacket.begin());
        }

        if (tdsType == TDS_RESPONSE && GetReq())
        {
            TDS_MSG_TYPE tdsReqType = GetReq()->GetTdsPacketType();
            if (tdsReqType == TDS_LOGIN7 || tdsReqType == TDS_SSPI_LOGIN7 || tdsReqType == TDS_FEDAUTH)
            {
                ProcessServerResponse_TDS_LOGIN7();
            }
            else
            {
                SetUseNewListTdsPacket(false);

                //if (tdsReqType == TDS_SQLBATCH || tdsReqType == TDS_RPC)
                {
                    for (auto pack : m_lstTdsPacket)
                    {
                        if (pack[0] == SMP_PACKET_FLAG)
                        {
                            *(uint32_t*)(pack + 12) = m_ProxyChannel->GetSessionAckWndwToClient(m_session_id) + 1;
                        }
                    }
                }
            }
        }

        SetDispatchReady(true);
        m_ProxyChannel->DispatchResp(m_session_id);
    }
}

void ProxyTask::ProcessServerResponse_TDS_LOGIN7()
{
    tdsTokenStream tokenStream;
    if (0 == tokenStream.Parse(m_lstTdsPacket))
    {
        return;
    }
    else
    {
        // NegotiatePacketSize
        DWORD size = 0;
        if (tokenStream.GetNegotiatePacketSize(size))
            m_ProxyChannel->SetNegotiatePacketSize(size);

        std::wstring db;
        if (tokenStream.GetDefaultDatabase(db))
            m_ProxyChannel->SetLoginDefaultDatabase(ProxyCommon::WStringToString(db));

        std::wstring server = L"";
        USHORT port = 0;
        if (tokenStream.GetRoutingInfo(server, port))
        {
            // close server socket, routing new server
            m_ProxyChannel->SetChannelCloseCause(CHANNEL_CLOSE_CAUSE_SERVER_ROUTING);
            theTCPFrame->Close(m_ProxyChannel->GetServerSocket());
            
            m_ProxyChannel->SetFinalRemoteServer(server);
            m_ProxyChannel->SetFinalRemoteServerPort(std::to_wstring(port));
            m_ProxyChannel->ResetBeforeRouting();
            //connect to server
            boost::shared_ptr<TcpSocket> tcpSocket;
            boost::system::error_code errorcode;
            bool bConnect = theTCPFrame->BlockConnect((char*)ProxyCommon::WStringToString(m_ProxyChannel->GetFinalRemoteServer()).c_str(),
                                                      (char*)ProxyCommon::WStringToString(m_ProxyChannel->GetFinalRemoteServerPort()).c_str(),
                                                      tcpSocket, errorcode);

            if (bConnect) {
                m_ProxyChannel->SetServerSocket(tcpSocket);
                ProxyManager::GetInstance()->AddSocketChannelMap(tcpSocket.get(), m_ProxyChannel);

                //send first prelogin package to server
                BYTE* pPacket = *m_ProxyChannel->GetCachedPreloginPackets().begin();
                theTCPFrame->BlockSendData(tcpSocket, pPacket, TDS::GetTdsPacketLength(pPacket), errorcode);
                m_ProxyChannel->SetChannelServerStatus(CHANNEL_STATUS_Wait_Server_First_Prelogin_Response_Routing);
                PROXYLOG(CELOG_INFO, L"[Routing Send Prelogin][Server: %s][Port: %d]", server.c_str(), port);
            }
            else {
                //close client socket
                PROXYLOG(CELOG_EMERG, L"[Routing Send Prelogin Failed][Server: %s][Port: %d][errorcode: %d]", server.c_str(), port, errorcode.value());
            }
            SetUseNewListTdsPacket(true);
        }
    }
}

SECURITY_STATUS ProxyTask::TLSHandShakeWithClient()
{
    TimeStamp            tsExpiry;
    SecBufferDesc        InBuffer;
    SecBufferDesc        OutBuffer;
    SecBuffer            InBuffers[2];
    SecBuffer            OutBuffers[1];

    DWORD dwSSPIOutFlags = 0;
    DWORD dwSSPIFlags = ASC_REQ_REPLAY_DETECT |
        ASC_REQ_CONFIDENTIALITY |
        ASC_REQ_EXTENDED_ERROR |
        ASC_REQ_ALLOCATE_MEMORY |
        ASC_REQ_STREAM;

    //BYTE* pTLSData = *m_lstTdsPacket.begin() + TDS_HEADER_LEN;
    if (m_lstTdsPacket.size() > 1)
    {
        PROXYLOG(CELOG_INFO, L"TdsTask::TLSHandShakeWithClient [m_lstTdsPacket size %d]", m_lstTdsPacket.size());
    }
    TdsBufferList bufList(m_lstTdsPacket);
    BYTE* pTLSData = bufList.Get() + TDS_HEADER_LEN;

    DWORD cbTLSData = bufList.Len() - TDS_HEADER_LEN;//TDS::GetTdsPacketLength(*m_lstTdsPacket.begin()) - TDS_HEADER_LEN;

    InBuffers[0].pvBuffer = pTLSData;
    InBuffers[0].cbBuffer = cbTLSData;
    InBuffers[0].BufferType = SECBUFFER_TOKEN;

    InBuffers[1].pvBuffer = NULL;
    InBuffers[1].cbBuffer = 0;
    InBuffers[1].BufferType = SECBUFFER_EMPTY;

    InBuffer.cBuffers = 2;
    InBuffer.pBuffers = InBuffers;
    InBuffer.ulVersion = SECBUFFER_VERSION;

    //
    // Set up the output buffers. These are initialized to NULL
    // so as to make it less likely we'll attempt to free random
    // garbage later.
    //

    OutBuffers[0].pvBuffer = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer = 0;

    OutBuffer.cBuffers = 1;
    OutBuffer.pBuffers = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    SECURITY_STATUS scRet = ProxyManager::GetInstance()->SecurityFunTable->AcceptSecurityContext(
        m_ProxyChannel->GetTlsServerCred(),								// Which certificate to use, already established
        m_ProxyChannel->IsTLSCtxServerInited() ? m_ProxyChannel->GetServerTLSCtx() : NULL,	// The context handle if we have one, ask to make one if this is first call
        &InBuffer,										// Input buffer list
        dwSSPIFlags,									// What we require of the connection
        0,												// Data representation, not used 
        m_ProxyChannel->IsTLSCtxServerInited() ? NULL : m_ProxyChannel->GetServerTLSCtx(),	// If we don't yet have a context handle, it is returned here
        &OutBuffer,										// [out] The output buffer, for messages to be sent to the other end
        &dwSSPIOutFlags,								// [out] The flags associated with the negotiated connection
        &tsExpiry);										// [out] Receives context expiration time

    m_ProxyChannel->SetTLSCtxServerInited(TRUE);

    if (scRet == SEC_E_OK || scRet == SEC_I_CONTINUE_NEEDED
        || (FAILED(scRet) && (0 != (dwSSPIOutFlags & ASC_RET_EXTENDED_ERROR))))
    {
        if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
        {
            //send TDS prelogin header
            unsigned char* sendBuf = new unsigned char[TDS_HEADER_LEN + OutBuffers[0].cbBuffer];
            TDS::FillTDSHeader(sendBuf, TDS_PRELOGIN, 1, OutBuffers[0].cbBuffer + TDS_HEADER_LEN);
            memcpy(sendBuf + TDS_HEADER_LEN, OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer);
            
            boost::system::error_code errcode;
            theTCPFrame->BlockSendData(m_ProxyChannel->GetClientSocket(), sendBuf, OutBuffers[0].cbBuffer + TDS_HEADER_LEN, errcode);
            if (NULL != sendBuf)
            {
                delete[] sendBuf;
                sendBuf = NULL;
            }

            ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(OutBuffers[0].pvBuffer);
            OutBuffers[0].pvBuffer = NULL;
        }
    }

    // At this point, we've read and checked a message (giving scRet) and maybe sent a response (giving err)
    // as far as the client is concerned, the SSL handshake may be done, but we still have checks to make.
    if (scRet == SEC_E_OK)
    {	// The termination case, the handshake worked and is completed, this could as easily be outside the loop


        // Now deal with the possibility that there were some data bytes tacked on to the end of the handshake
#if 0
        if (InBuffers[1].BufferType == SECBUFFER_EXTRA)
        {
            readPtr = readBuffer + (readBufferBytes - InBuffers[1].cbBuffer);
            readBufferBytes = InBuffers[1].cbBuffer;
            DebugMsg("Handshake worked, but received %d extra bytes", readBufferBytes);
        }
        else
        {
            readBufferBytes = 0;
            readPtr = readBuffer;
            DebugMsg("Handshake worked, no extra bytes received");
        }
#endif ///// !!!!!!!!!!!!!!

        return scRet; // The normal exit
    }
    else if (scRet == SEC_E_INCOMPLETE_MESSAGE)
    {
        PROXYLOG(CELOG_EMERG, L"AcceptSecurityContext got a partial message and is requesting more be read\n");
    }
    else if (scRet == SEC_E_INCOMPLETE_CREDENTIALS)
    {
        PROXYLOG(CELOG_EMERG, L"AcceptSecurityContext got SEC_E_INCOMPLETE_CREDENTIALS, it shouldn't but we'll treat it like a partial message\n");
    }
    else if (FAILED(scRet))
    {
        if (scRet == SEC_E_INVALID_TOKEN)
            PROXYLOG(CELOG_EMERG, L"AcceptSecurityContext detected an invalid token, maybe the client rejected our certificate\n");
        else
            PROXYLOG(CELOG_EMERG, L"AcceptSecurityContext Failed with error code %#x\n", scRet);
        //return false;
    }
    else
    {  // We won't be appending to the message data already in the buffer, so store a reference to any extra data in case it is useful
#if 0
        if (InBuffers[1].BufferType == SECBUFFER_EXTRA)
        {
            //readPtr = readBuffer + (readBufferBytes - InBuffers[1].cbBuffer);
            readBufferBytes = InBuffers[1].cbBuffer;
            printf("Handshake working so far but received %d extra bytes we can't handle\n", readBufferBytes);
            //m_LastError = WSASYSCALLFAILURE;
            return false;
        }
        else
        {
            //readPtr = readBuffer;
            readBufferBytes = 0; // prepare for next receive
            printf("Handshake working so far, more packets required\n");
        }
#endif 
    }

    // Something is wrong, we exited the loop abnormally

    return scRet;
}

SECURITY_STATUS ProxyTask::TLSHandShakeWithServer(BOOL bInit)
{
    SecBufferDesc   OutBuffer, InBuffer;
    SecBuffer       InBuffers[2], OutBuffers[1];
    DWORD           dwSSPIFlags, dwSSPIOutFlags;
    TimeStamp       tsExpiry;
    SECURITY_STATUS scRet;


    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY |
        ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;

    TdsBufferList bufList(m_lstTdsPacket);

    if (!bInit)
    {
        //PBYTE pInputData = *m_lstTdsPacket.begin() + TDS_HEADER_LEN;
        if (m_lstTdsPacket.size() > 1)
        {
            PROXYLOG(CELOG_INFO, L"TdsTask::TLSHandShakeWithServer [m_lstTdsPacket size %d]", m_lstTdsPacket.size());
        }

        BYTE* pInputData = bufList.Get() + TDS_HEADER_LEN;

        //DWORD cbInputLen = TDS::GetTdsPacketLength(*m_lstTdsPacket.begin()) - TDS_HEADER_LEN;
        DWORD cbInputLen = bufList.Len() - TDS_HEADER_LEN;

        InBuffers[0].pvBuffer = pInputData;
        InBuffers[0].cbBuffer = cbInputLen;
        InBuffers[0].BufferType = SECBUFFER_TOKEN;

        InBuffers[1].pvBuffer = NULL;
        InBuffers[1].cbBuffer = 0;
        InBuffers[1].BufferType = SECBUFFER_EMPTY;

        InBuffer.cBuffers = 2;
        InBuffer.pBuffers = InBuffers;
        InBuffer.ulVersion = SECBUFFER_VERSION;
    }

    // Set up the output buffers. These are initialized to NULL
    // so as to make it less likely we'll attempt to free random
    // garbage later.
    OutBuffers[0].pvBuffer = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer = 0;

    OutBuffer.cBuffers = 1;
    OutBuffer.pBuffers = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    // Call InitializeSecurityContext.
    scRet = ProxyManager::GetInstance()->SecurityFunTable->InitializeSecurityContext(
        m_ProxyChannel->GetTlsClientCred(),
        bInit ? NULL : m_ProxyChannel->GetClientTLSCtx(),
        NULL,
        dwSSPIFlags,
        0,
        SECURITY_NATIVE_DREP,
        bInit ? NULL : &InBuffer,
        0,
        bInit ? m_ProxyChannel->GetClientTLSCtx() : NULL,
        &OutBuffer,
        &dwSSPIOutFlags,
        &tsExpiry);

    m_ProxyChannel->SetTLSCtxClientInited(TRUE);

    // If InitializeSecurityContext was successful (or if the error was 
    // one of the special extended ones), send the contends of the output
    // buffer to the server.
    if (scRet == SEC_E_OK ||
        scRet == SEC_I_CONTINUE_NEEDED ||
        FAILED(scRet) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))
    {
        if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
        {
            //send TDS prelogin header
            unsigned char* sendBuf = new unsigned char[TDS_HEADER_LEN + OutBuffers[0].cbBuffer];
            TDS::FillTDSHeader(sendBuf, TDS_PRELOGIN, 1, OutBuffers[0].cbBuffer + TDS_HEADER_LEN);
            memcpy(sendBuf + TDS_HEADER_LEN, OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer);

            boost::system::error_code errcode;
            theTCPFrame->BlockSendData(m_ProxyChannel->GetServerSocket(), sendBuf, OutBuffers[0].cbBuffer + TDS_HEADER_LEN, errcode);
            if (NULL != sendBuf)
            {
                delete[] sendBuf;
                sendBuf = NULL;
            }

            // Free output buffer.
            ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(OutBuffers[0].pvBuffer);
            OutBuffers[0].pvBuffer = NULL;
        }
    }

    // If InitializeSecurityContext returned SEC_E_INCOMPLETE_MESSAGE,
    // then we need to read more data from the server and try again.
    if (scRet == SEC_E_INCOMPLETE_MESSAGE) {
        PROXYLOG(CELOG_EMERG, L"AcceptSecurityContext got a partial message and is requesting more be read");
        assert(FALSE);
    }

    // If InitializeSecurityContext returned SEC_E_OK, then the 
    // handshake completed successfully.
    if (scRet == SEC_E_OK)
    {
        // If the "extra" buffer contains data, this is encrypted application
        // protocol layer stuff. It needs to be saved. The application layer
        // will later decrypt it with DecryptMessage.
        PROXYLOG(CELOG_NOTICE, L"Handshake was successful\n");
#if 0
        if (InBuffers[1].BufferType == SECBUFFER_EXTRA)
        {
            pExtraData->pvBuffer = LocalAlloc(LMEM_FIXED, InBuffers[1].cbBuffer);
            if (pExtraData->pvBuffer == NULL) { printf("**** Out of memory (2)\n"); return SEC_E_INTERNAL_ERROR; }

            MoveMemory(pExtraData->pvBuffer,
                IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer),
                InBuffers[1].cbBuffer);

            pExtraData->cbBuffer = InBuffers[1].cbBuffer;
            pExtraData->BufferType = SECBUFFER_TOKEN;

            printf("%d bytes of app data was bundled with handshake data\n", pExtraData->cbBuffer);
        }
        else
        {
            pExtraData->pvBuffer = NULL;
            pExtraData->cbBuffer = 0;
            pExtraData->BufferType = SECBUFFER_EMPTY;
        }
#endif 
        return scRet;
    }
    else if (FAILED(scRet))
    {
        PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by InitializeSecurityContext (2)\n", scRet);
    }

    // If InitializeSecurityContext returned SEC_I_INCOMPLETE_CREDENTIALS,
    // then the server just requested client authentication.

    if (scRet == SEC_I_INCOMPLETE_CREDENTIALS)
    {
        // Busted. The server has requested client authentication and
        // the credential we supplied didn't contain a client certificate.
        // This function will read the list of trusted certificate
        // authorities ("issuers") that was received from the server
        // and attempt to find a suitable client certificate that
        // was issued by one of these. If this function is successful, 
        // then we will connect using the new certificate. Otherwise,
        // we will attempt to connect anonymously (using our current credentials).
        // 		GetNewClientCredentials(phCreds, phContext);
        // 
        // 		// Go around again.
        // 		fDoRead = FALSE;
        // 		scRet = SEC_I_CONTINUE_NEEDED;
        // 		continue;
        assert(FALSE);
    }


    // Copy any leftover data from the "extra" buffer, and go around again.
    if (InBuffers[1].BufferType == SECBUFFER_EXTRA)
    {
        //MoveMemory( IoBuffer, IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer), InBuffers[1].cbBuffer );
        //cbIoBuffer = InBuffers[1].cbBuffer;
        assert(FALSE);
    }

    return scRet;
}

TDS_MSG_TYPE ProxyTask::GetTdsPacketType() const
{
    uint8_t* pack = m_lstTdsPacket.front();
    if (pack[0] == SMP_PACKET_FLAG) {
        if (SMP::GetSMPPacketLen(pack) <= 16)
            return TDS_Unkown;
        else if (SMP::GetSMPPacketLen(pack) > 16)
            return TDS::GetTdsPacketType(pack + 16);
    }
    else
        return TDS::GetTdsPacketType(pack);
}

BOOL ProxyTask::CheckClientKrbTicket(std::vector<BYTE>& data, PCtxtHandle pctx)
{
    BOOL                 bRet = FALSE;
    TimeStamp            tsExpiry;
    SecBufferDesc        InBuffer;
    SecBufferDesc        OutBuffer;
    SecBuffer            InBuffers[2];
    SecBuffer            OutBuffers[1];

    DWORD dwSSPIOutFlags = 0;
    DWORD dwSSPIFlags = ASC_REQ_REPLAY_DETECT |
        ASC_REQ_CONFIDENTIALITY |
        ASC_REQ_EXTENDED_ERROR |
        ASC_REQ_ALLOCATE_MEMORY |
        ASC_REQ_STREAM;

    InBuffers[0].pvBuffer = &data[0];
    InBuffers[0].cbBuffer = data.size();
    InBuffers[0].BufferType = SECBUFFER_TOKEN;

    InBuffers[1].pvBuffer = NULL;
    InBuffers[1].cbBuffer = 0;
    InBuffers[1].BufferType = SECBUFFER_EMPTY;

    InBuffer.cBuffers = 2;
    InBuffer.pBuffers = InBuffers;
    InBuffer.ulVersion = SECBUFFER_VERSION;

    //
    // Set up the output buffers. These are initialized to NULL
    // so as to make it less likely we'll attempt to free random
    // garbage later.
    //

    OutBuffers[0].pvBuffer = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer = 0;

    OutBuffer.cBuffers = 1;
    OutBuffer.pBuffers = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;

    SECURITY_STATUS scRet = ProxyManager::GetInstance()->SecurityFunTable->AcceptSecurityContext(
        m_ProxyChannel->GetKrbServerCred(),								// Which certificate to use, already established
        NULL,	                                        // The context handle if we have one, ask to make one if this is first call
        &InBuffer,										// Input buffer list
        dwSSPIFlags,									// What we require of the connection
        0,												// Data representation, not used 
        pctx,	                                    // If we don't yet have a context handle, it is returned here
        &OutBuffer,										// [out] The output buffer, for messages to be sent to the other end
        &dwSSPIOutFlags,								// [out] The flags associated with the negotiated connection
        &tsExpiry);										// [out] Receives context expiration time

    if (scRet == SEC_E_OK || scRet == SEC_I_CONTINUE_NEEDED
        || (FAILED(scRet) && (0 != (dwSSPIOutFlags & ASC_RET_EXTENDED_ERROR))))
    {
        if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
        {
            //send Kerberos response token
            unsigned char* sendBuf = new unsigned char[TDS_HEADER_LEN + OutBuffers[0].cbBuffer + 3];
            TDS::FillTDSHeader(sendBuf, TDS_RESPONSE, 1, OutBuffers[0].cbBuffer + TDS_HEADER_LEN + 3);
            sendBuf[TDS_HEADER_LEN] = 0xed; // token sspi
            *(uint16_t*)(sendBuf + TDS_HEADER_LEN + 1) = (uint16_t)(OutBuffers[0].cbBuffer);
            memcpy(sendBuf + TDS_HEADER_LEN + 3, OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer);

            if (m_ProxyChannel->GetEncryptionOption() == 1 || m_ProxyChannel->GetEncryptionOption() == 3)
            {
                boost::system::error_code errorcode;
                std::list<SecBuffer> lstEncPacks;
                m_ProxyChannel->EncryptServerMessage(sendBuf, OutBuffers[0].cbBuffer + TDS_HEADER_LEN + 3, lstEncPacks);

                for (auto& enc_data : lstEncPacks)
                {
                    theTCPFrame->BlockSendData(m_ProxyChannel->GetClientSocket(), (PBYTE)enc_data.pvBuffer, enc_data.cbBuffer, errorcode);

                    LocalFree(enc_data.pvBuffer);
                    enc_data.pvBuffer = NULL;
                    enc_data.cbBuffer = 0;
                }
            }
            else
            {
                boost::system::error_code errcode;
                theTCPFrame->BlockSendData(m_ProxyChannel->GetClientSocket(), sendBuf, OutBuffers[0].cbBuffer + TDS_HEADER_LEN + 3, errcode);
            }
            
            if (NULL != sendBuf)
            {
                delete[] sendBuf;
                sendBuf = NULL;
            }

            bRet = TRUE;
        }
    }

    if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
    {
        ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(OutBuffers[0].pvBuffer);
        OutBuffers[0].pvBuffer = NULL;
    }

    // At this point, we've read and checked a message (giving scRet) and maybe sent a response (giving err)
    // as far as the client is concerned, the SSL handshake may be done, but we still have checks to make.
    if (scRet == SEC_E_OK)
    {
        return TRUE; // The normal exit
    }
    else if (scRet == SEC_E_INCOMPLETE_MESSAGE)
    {
        PROXYLOG(CELOG_EMERG, L"CheckClientKrbTicket AcceptSecurityContext got a partial message and is requesting more be read");
    }
    else if (scRet == SEC_E_INCOMPLETE_CREDENTIALS)
    {
        PROXYLOG(CELOG_EMERG, L"CheckClientKrbTicket AcceptSecurityContext got SEC_E_INCOMPLETE_CREDENTIALS, it shouldn't but we'll treat it like a partial message");
    }
    else if (FAILED(scRet))
    {
        if (scRet == SEC_E_INVALID_TOKEN)
            PROXYLOG(CELOG_EMERG, L"CheckClientKrbTicket AcceptSecurityContext detected an invalid token, maybe the client login user or target is wrong.");
        else
            PROXYLOG(CELOG_EMERG, L"CheckClientKrbTicket AcceptSecurityContext Failed with error code %#x\n", scRet);
        //return false;
    }

    // Something is wrong, we exited the loop abnormally

    return bRet;
}

BOOL ProxyTask::CreateKrbAuthTicketForServer(std::vector<BYTE>& data, PCtxtHandle pctx)
{
    SecBufferDesc   OutBuffer;
    SecBuffer       OutBuffers[1];
    DWORD           dwSSPIFlags = 0, dwSSPIOutFlags = 0;
    TimeStamp       tsExpiry;
    SECURITY_STATUS scRet;
    BOOL            bRet = FALSE;

    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY |
        ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;
    

    // Set up the output buffers. These are initialized to NULL
    // so as to make it less likely we'll attempt to free random
    // garbage later.
    OutBuffers[0].pvBuffer = NULL;
    OutBuffers[0].BufferType = SECBUFFER_TOKEN;
    OutBuffers[0].cbBuffer = 0;

    OutBuffer.cBuffers = 1;
    OutBuffer.pBuffers = OutBuffers;
    OutBuffer.ulVersion = SECBUFFER_VERSION;
    
    // Call InitializeSecurityContext.
    scRet = ProxyManager::GetInstance()->SecurityFunTable->InitializeSecurityContext(
        m_ProxyChannel->GetKrbClientCred(),
        NULL,
        //L"MSSQLSvc/W19-SQL19.qapf1.qalab01.nextlabs.com:1433",
        (WCHAR*)theConfig.DatabaseSPN().c_str(),
        dwSSPIFlags,
        0,
        SECURITY_NATIVE_DREP,
        NULL,
        0,
        pctx,
        &OutBuffer,
        &dwSSPIOutFlags,
        &tsExpiry);

    // If InitializeSecurityContext was successful (or if the error was 
    // one of the special extended ones), send the contends of the output
    // buffer to the server.
    if (scRet == SEC_E_OK ||
        scRet == SEC_I_CONTINUE_NEEDED ||
        FAILED(scRet) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))
    {
        if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
        {
            data.resize(OutBuffers[0].cbBuffer, 0);
            memcpy(&data[0], OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer);

            bRet = TRUE;
            PROXYLOG(CELOG_NOTICE, "Create kerberos auth ticket for server success. Length: %d", OutBuffers[0].cbBuffer);
        }
    }

    if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
    {
        // Free output buffer.
        ProxyManager::GetInstance()->SecurityFunTable->FreeContextBuffer(OutBuffers[0].pvBuffer);
        OutBuffers[0].pvBuffer = NULL;
    }

    // If InitializeSecurityContext returned SEC_E_INCOMPLETE_MESSAGE,
    // then we need to read more data from the server and try again.
    if (scRet == SEC_E_INCOMPLETE_MESSAGE) {
        PROXYLOG(CELOG_ERR, L"CreateKrbAuthTicketForServer InitializeSecurityContext got a partial message and is requesting more be read");
        assert(FALSE);
    }

    // If InitializeSecurityContext returned SEC_E_OK, then the 
    // handshake completed successfully.
    if (scRet == SEC_E_OK)
    {
        // If the "extra" buffer contains data, this is encrypted application
        // protocol layer stuff. It needs to be saved. The application layer
        // will later decrypt it with DecryptMessage.

        return bRet;
    }
    else if (FAILED(scRet))
    {
        PROXYLOG(CELOG_ERR, L"CreateKrbAuthTicketForServer InitializeSecurityContext Error: 0x%X", scRet);
    }

    // If InitializeSecurityContext returned SEC_I_INCOMPLETE_CREDENTIALS,
    // then the server just requested client authentication.

    if (scRet == SEC_I_INCOMPLETE_CREDENTIALS)
    {
        // Busted. The server has requested client authentication and
        // the credential we supplied didn't contain a client certificate.
        // This function will read the list of trusted certificate
        // authorities ("issuers") that was received from the server
        // and attempt to find a suitable client certificate that
        // was issued by one of these. If this function is successful, 
        // then we will connect using the new certificate. Otherwise,
        // we will attempt to connect anonymously (using our current credentials).
        // 		GetNewClientCredentials(phCreds, phContext);
        // 
        // 		// Go around again.
        // 		fDoRead = FALSE;
        // 		scRet = SEC_I_CONTINUE_NEEDED;
        // 		continue;
        assert(FALSE);
    }

    return bRet;
}