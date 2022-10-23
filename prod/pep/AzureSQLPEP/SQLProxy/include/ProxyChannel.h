#ifndef PROXY_CHANNEL_H
#define PROXY_CHANNEL_H
#include <windows.h>
#include "MemoryCache.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "TCPFrame.h"
#include <memory.h>
#include <atomic>
#include <map>
class ProxyTask;
class ProxyChannel;

#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif 
#include <Sspi.h>

#define INCOMPLETE_TLS_MSG_LEN 32*1024

typedef std::shared_ptr<ProxyChannel> ProxyChannelPtr;

enum CHANNEL_STATUS
{
	CHANNEL_STATUS_Wait_Client_First_Prelogin = 0x01,
	//CHANNEL_STATUS_Wait_Client_Login=0x02,

	CHANNEL_STATUS_Wait_Server_First_Prelogin_Response = 0x11,
	//CHANNEL_STATUS_Wait_Server_Login_Response = 0x12,

	CHANNEL_STATUS_TLS_Handle_Shake = 0x21,
	CHANNEL_STATUS_Proxy_Success = 0x06,

	CHANNEL_STATUS_Wait_Server_First_Prelogin_Response_Routing,
	CHANNEL_STATUS_TLS_Handle_Shake_Routing,
	CHANNEL_STATUS_Proxy_Success_Routing,
};

enum CHANNEL_CLOSE_CAUSE
{
	CHANNEL_CLOSE_CAUSE_NONE,
	CHANNEL_CLOSE_CAUSE_CLIENT_DISCONNECT,
	CHANNEL_CLOSE_CAUSE_SERVER_DISCONNECT,
	CHANNEL_CLOSE_CAUSE_SERVER_ROUTING,
};

#define CACHED_TLS_CLIENT_DATA  0x01   //client send TLS encrypted data, before our proxy established the TLS ctx;
#define CACHED_TLS_SERVER_DATA  0x02

#define SESSION_ID_IF_NO_SMP    -1

struct SessionInChannel
{
    uint32_t request_seqnum;
    uint32_t response_seqnum;
    uint32_t syn_wndw;
    uint32_t ack_wndw_from_client;
    uint32_t ack_wndw_from_server;
    uint32_t ack_wndw_to_client;
    uint32_t ack_wndw_to_server;
    uint32_t wndw_to_client;
    uint32_t wndw_to_server;
    HANDLE dispatch_event;

    SessionInChannel()
    {
        request_seqnum = 0;
        response_seqnum = 0;
        syn_wndw = 0;
        ack_wndw_from_client = 0;
        ack_wndw_from_server = 0;
        ack_wndw_to_client = 0;
        ack_wndw_to_server = 0;
        wndw_to_client = 0;
        wndw_to_server = 0;
        dispatch_event = NULL;
    }
};

typedef std::map<uint32_t, std::list<boost::shared_ptr<ProxyTask>>> session_task_map;
typedef std::map<uint32_t, boost::shared_ptr<ProxyTask>> session_uncomplete_task_map;
typedef std::map<uint32_t, SessionInChannel> session_map;

class ProxyChannel
{
public:
	ProxyChannel(void);
	~ProxyChannel(void);

public:
    bool InitTlsServerCred();
    bool InitTlsClientCred();
    bool InitKrbServerCred();
    bool InitKrbClientCred();

    PCredHandle GetTlsServerCred() { return &m_tlsSvrCred; }
    PCredHandle GetTlsClientCred() { return &m_tlsCltCred; }
    PCredHandle GetKrbServerCred() { return &m_krbSvrCred; }
    PCredHandle GetKrbClientCred() { return &m_krbCltCred; }

	void SetClientSocket(TcpSocketPtr clientSock) { m_ClientSocket = clientSock;}
	boost::shared_ptr<TcpSocket> GetClientSocket() {return m_ClientSocket; }
	BOOL IsClientSocket(void* sock) { return m_ClientSocket.get()==sock; }
	void SetServerSocket(TcpSocketPtr serverSock) { m_ServerSocket =  serverSock;}
	boost::shared_ptr<TcpSocket> GetServerSocket() {return m_ServerSocket; }
	BOOL IsServerSocket(void* sock) { return m_ServerSocket.get()==sock; }
	void CloseClient() { theTCPFrame->Close(GetClientSocket()); }

	CHANNEL_STATUS GetChannelClientStatus() { return m_ChannelClientStatus; }
	void SetChannelClientStatus(CHANNEL_STATUS status) { m_ChannelClientStatus=status;}

	CHANNEL_STATUS GetChannelServerStatus() { return m_ChannelServerStatus; }
	void SetChannelServerStatus(CHANNEL_STATUS status) { m_ChannelServerStatus = status; }

	DWORD DecryptClientMessage(PBYTE pBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, PBYTE*  ppAllocateBuf);
	DWORD DecryptServerMessage(PBYTE pBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, PBYTE*  ppAllocateBuf);

	void EncryptClientMessage(std::list<PBYTE>& lstData, std::list<SecBuffer>& lstEncryptData);
	void EncryptClientMessage(PBYTE pData, DWORD dwDataLen, std::list<SecBuffer>& lstEncryptData);
	void EncryptServerMessage(std::list<PBYTE>& lstData, std::list<SecBuffer>& lstEncryptData);
    void EncryptServerMessage(PBYTE pData, DWORD dwDataLen, std::list<SecBuffer>& lstEncryptData);

	DWORD WaitTLSHandShakeFinishWithRemoteServer() {return WaitForSingleObject(m_hEventTLSSuccessWithRemoteServer, 10*1000);}
	void SetTLSHandShakeFinishWithRemoteServer() { SetEvent(m_hEventTLSSuccessWithRemoteServer); }

	void SetServerTLSSizeProperty(SecPkgContext_StreamSizes* TLSSizes){ CopyTLSSize(&m_TLSSizesToServer, TLSSizes);	}
	void SetClientTLSSizeProperty(SecPkgContext_StreamSizes* TLSSizes){ CopyTLSSize(&m_TLSSizesToClient, TLSSizes);	}

	void SetTLSCacheType(DWORD dwCacheType) { m_dwTLSCacheType= dwCacheType; }
	DWORD GetTLSCacheType(){ return m_dwTLSCacheType; }

	void ProcessCachedTLSData(DWORD dwCacheType); 

	void SetClientTLSFinished(BOOL bFin) { m_bClientTLSFinished = bFin;}
	BOOL ClientTLSFinished() { return m_bClientTLSFinished; }
	void SetServerTLSFinished(BOOL bFin) { m_bServerTLSFinished = bFin;}
	BOOL ServerTLSFinished() { return m_bServerTLSFinished; }

	CtxtHandle* GetClientTLSCtx() { return &m_TLSCtxClient; }
	CtxtHandle* GetServerTLSCtx() { return &m_TLSCtxServer; }

	void SetTLSCtxClientInited(BOOL bInit) { m_bTLSCtxClientInited= bInit;}
	BOOL IsTLSCtxClientInited() { return m_bTLSCtxClientInited;}

	void SetTLSCtxServerInited(BOOL bInit) { m_bTLSCtxServerInited = bInit; }
	BOOL IsTLSCtxServerInited() { return m_bTLSCtxServerInited; }

    void SetIncompleteClientTask(uint32_t session_id, boost::shared_ptr<ProxyTask> pTask);
    void SetIncompleteServerTask(uint32_t session_id, boost::shared_ptr<ProxyTask> pTask);
    boost::shared_ptr<ProxyTask> GetIncompleteClientTask(uint32_t session_id);
    boost::shared_ptr<ProxyTask> GetIncompleteServerTask(uint32_t session_id);
    void RemoveIncompleteClientTask(uint32_t session_id);
    void RemoveIncompleteServerTask(uint32_t session_id);

    uint32_t GetSessionInitWndw(uint32_t sid);
    void SetSessionInitWndw(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionAckWndwFromClient(uint32_t sid);
    void SetSessionAckWndwFromClient(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionAckWndwFromServer(uint32_t sid);
    void SetSessionAckWndwFromServer(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionAckWndwToClient(uint32_t sid);
    void SetSessionAckWndwToClient(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionAckWndwToSever(uint32_t sid);
    void SetSessionAckWndwToSever(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionRequsetSeqnum(uint32_t sid);
    void SetSessionRequestSeqnum(uint32_t sid, uint32_t seqnum);

    uint32_t GetSessionResponseSeqnum(uint32_t sid);
    void SetSessionResponseSeqnum(uint32_t sid, uint32_t seqnum);

    uint32_t GetSessionWndwToClient(uint32_t sid);
    void SetSessionWndwToClient(uint32_t sid, uint32_t wndw);

    uint32_t GetSessionWndwToServer(uint32_t sid);
    void SetSessionWndwToServer(uint32_t sid, uint32_t wndw);

    void SetEventForSessionDispEvent(uint32_t sid);
    void WaitForSessionDispEvent(uint32_t sid, uint32_t timeout);

    void EndSession(uint32_t sid);

	BOOL ValidClientTdsPackage(PBYTE pBuf, DWORD dwLen);

public:
	bool PushBackReqNeedDispatch(boost::shared_ptr<ProxyTask> req);
	void DispatchReq(uint32_t sid);
	bool PushBackRespNeedDispatch(boost::shared_ptr<ProxyTask> resp);
	void DispatchResp(uint32_t sid);
	bool HasReqTask();
	bool HasRespTask();

    void ReplySMPAckToClient(uint32_t sid, uint32_t wndw);
    void ReplySMPAckToServer(uint32_t sid, uint32_t wndw);

private:
    void TransferEncryptedRequest(ProxyTask*, std::list<PBYTE>& packets, boost::system::error_code& err);
    void TransferOriginalRequest(ProxyTask*, std::list<PBYTE>& packets, boost::system::error_code& err);
    void TransferEncryptedResponse(uint32_t sid);
    void TransferOriginalResponse(uint32_t sid);

	DWORD DecryptMessage(CtxtHandle* tlsCtx, PBYTE pInBuf, DWORD dwBufLen, std::list<SecBuffer>& tdsData, SecBuffer* extraData);
	DWORD EncryptMessage(CtxtHandle* tlsCtx, SecPkgContext_StreamSizes* tlsSize, PBYTE pInBuf, DWORD dwBufLen, std::list<SecBuffer>& lstEncryptData);

	void CopyTLSSize(SecPkgContext_StreamSizes* dstSize, SecPkgContext_StreamSizes* srcSize);

private:
	boost::shared_ptr<TcpSocket> m_ClientSocket;
	boost::shared_ptr<TcpSocket>  m_ServerSocket;

	std::atomic<CHANNEL_STATUS> m_ChannelClientStatus;
	std::atomic<CHANNEL_STATUS> m_ChannelServerStatus;

	std::atomic<BOOL> m_bClientTLSFinished;
	std::atomic<BOOL> m_bServerTLSFinished;

    CredHandle  m_tlsSvrCred;
    CredHandle  m_tlsCltCred;
    CredHandle  m_krbSvrCred;
    CredHandle  m_krbCltCred;

	CtxtHandle  m_TLSCtxClient;
	BOOL m_bTLSCtxClientInited;

	CtxtHandle  m_TLSCtxServer;
	BOOL m_bTLSCtxServerInited;

	HANDLE m_hEventTLSSuccessWithRemoteServer;

	//when decrypt, it may be have incomplete data
	BYTE m_inCompleteTLSBufClient[INCOMPLETE_TLS_MSG_LEN];
	DWORD m_dwInCompleteTLSBufClientLen;
	BYTE m_inCompleteTLSBufServer[INCOMPLETE_TLS_MSG_LEN];
	DWORD m_dwInCompleteTLSBufServerLen;

	SecPkgContext_StreamSizes m_TLSSizesToServer; 
	SecPkgContext_StreamSizes m_TLSSizesToClient; 

	DWORD m_dwTLSCacheType;

    //cached task
	session_uncomplete_task_map m_mapIncompleteTaskClient;
    session_uncomplete_task_map m_mapIncompleteTaskServer;

	// by jie 2018.05.21
private:
	session_task_map m_mapReqNeedDispatch;
	session_task_map m_mapRespNeedDispatch;
	session_task_map m_mapReqDispatched;
	CRITICAL_SECTION m_csListReqNeedDispatch;
	CRITICAL_SECTION m_csListRespNeedDispatch;
	CRITICAL_SECTION m_csListReqDispatched;

    session_map      m_mapSession;
    CRITICAL_SECTION m_csSession;

public:
	const std::wstring& GetDbInfo() const { return m_dbInfo; }
	bool SetDbInfo(const WCHAR* db)
	{
		if (0 == wcslen(db))
			return false;
		m_dbInfo = db;
		return true;
	}
    void SetLoginUser(const std::string& user) { m_login_user = user; }
    const std::string& GetLoginUser() { return m_login_user; }
    void SetLoginDomain(const std::string& domain) { m_login_domain = domain; }
    const std::string& GetLoginDomain() { return m_login_domain; }
    void SetLoginHostname(const std::string& host) { m_login_hostname = host; }
    const std::string& GetLoginHostname() { return m_login_hostname; }
	void SetLoginAppname(const std::string& app) { m_login_appname = app; }
	const std::string& GetLoginAppname() { return m_login_appname; }
    void SetLoginDefaultDatabase(const std::string& db) { CriticalSectionLock l(&m_cs_default_db); m_login_default_database = db; }
    const std::string GetLoginDefaultDatabase() { CriticalSectionLock l(&m_cs_default_db); return m_login_default_database; }

public:
    void CachePreloginPackets(const std::list<PBYTE>& src);
    void CacheLogin7Packets(const std::list<PBYTE>& src);
    void CacheSSPIPackets(const std::list<PBYTE>& src);
    void ResetBeforeRouting();

	const std::wstring& GetFinalRemoteServer() const { return m_finalRemoteServer; }
	const std::wstring& GetFinalRemoteServerPort() const { return m_finalRemoteServerPort; }
	void SetFinalRemoteServer(const std::wstring& server) { m_finalRemoteServer = server; }
	void SetFinalRemoteServerPort(const std::wstring& port) { m_finalRemoteServerPort = port; }
	const std::list<PBYTE>& GetCachedPreloginPackets() const { return m_cachedPreloginPackets; }
	const std::list<PBYTE>& GetCachedLogin7Packets() const { return m_cachedLogin7Packets; }
    const std::list<PBYTE>& GetCachedSSPIPackets() const { return m_cachedSSPIPackets; }
	CHANNEL_CLOSE_CAUSE GetChannelCloseCause() const { return m_channelCloseCause; }
	void SetChannelCloseCause(CHANNEL_CLOSE_CAUSE flag) { m_channelCloseCause = flag; }
    void SetChannelClose(bool b);
    bool IsChannelClose() { return m_bChannelClose; }

public:
	DWORD GetNegotiatePacketSize() const { return m_dwNegotiatePacketSize; }
	void SetNegotiatePacketSize(DWORD sz){ m_dwNegotiatePacketSize = sz; }
    BYTE GetEncryptionOption() const { return m_encryptOption; }
    void SetEncryptionOption(BYTE eo) { m_encryptOption = eo; }

private:
	std::wstring m_dbInfo;
    std::string  m_login_user;
    std::string  m_login_domain;
    std::string  m_login_hostname;
    std::string  m_login_default_database;
	std::string  m_login_appname;
    CRITICAL_SECTION m_cs_default_db;

private:
	std::wstring m_finalRemoteServer;
	std::wstring m_finalRemoteServerPort;
	std::list<PBYTE> m_cachedPreloginPackets;
	std::list<PBYTE> m_cachedLogin7Packets;
    std::list<PBYTE> m_cachedSSPIPackets;
private:
	CHANNEL_CLOSE_CAUSE m_channelCloseCause;
    std::atomic_bool    m_bChannelClose;

private:
	DWORD              m_dwNegotiatePacketSize;
    std::atomic<BYTE>  m_encryptOption;
};

#endif 

