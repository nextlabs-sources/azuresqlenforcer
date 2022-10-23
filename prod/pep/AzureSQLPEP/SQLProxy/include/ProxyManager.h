#ifndef PROXY_MANAGER_H
#define PROXY_MANAGER_H

#include <windows.h>
#include <list>
#include <map>
#include "ProxyChannel.h"
#include "Log.h"
#include <boost/system/error_code.hpp>

#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif 
#include <Security.h>
#include <schannel.h>
#pragma comment(lib, "Crypt32.Lib")

class ProxyManager
{
public:
    static ProxyManager* GetInstance();
    static void Release();

public:
	void Init();

	void ServerStartEvent(TcpSocketPtr tcpSocket);
	void EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);

public:
	ProxyChannelPtr GetProxyChannel(void* tcpSocket);
	void AddSocketChannelMap(void* tcpSocket, ProxyChannelPtr pChannel);
	DWORD GetTLSProtocolVersion() { return m_TLSProtocol; }

    PCCERT_CONTEXT GetTlsCredContext() { return m_certContext; }

    void SetInstancePort(USHORT pt) { m_instancePort = pt; }
    USHORT GetInstancePort() { return m_instancePort; }

private:
    ProxyManager(void);
    ~ProxyManager(void);

    BOOL InitCertContext();
	BOOL LoadSecurityLibrary( void );

public:
	PSecurityFunctionTable SecurityFunTable;


private:
    static ProxyManager* m_pThis;

    bool m_bInitComplete;

	std::map<void*,ProxyChannelPtr>  m_ProxyChannels;  
    std::list<ProxyChannelPtr>       m_lstProxyNeedClean;
	CRITICAL_SECTION m_csProxyChannel;
    CRITICAL_SECTION m_csChannelClean;
    USHORT m_instancePort;
	
	std::list<boost::shared_ptr<TcpSocket>> m_lstEndSockets;
	CRITICAL_SECTION m_csEndSockets;
	HANDLE  m_hEventHaveEndSocket;
	HANDLE m_hSocketCleanThread;

	HANDLE m_hSocketDataReadyEvent;
	HANDLE m_hTdsDataReadyEvent;

	HANDLE m_hTdsTaskDispatchThread;

	//TLS credtical
    PCCERT_CONTEXT m_certContext;
	const DWORD m_TLSProtocol;
};

#endif 

