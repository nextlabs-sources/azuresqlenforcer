#include "ProxyManager.h"
#include "TCPFrame.h"
#include "ProxyDataMgr.h"
#include "ProxyTask.h"
#include "CriticalSectionLock.h"
#include "CertificateHelper.h"
#include "Log.h"
#include "Config.h"
#include "CycleTaskManager.h"
#include "ProxyDataAnalysis.h"
#include "CommonFunc.h"

ProxyManager* ProxyManager::m_pThis = NULL;

ProxyManager* ProxyManager::GetInstance()
{
    if (m_pThis == NULL)
    {
        m_pThis = new ProxyManager;
    }

    return m_pThis;
}

void ProxyManager::Release()
{
    if (m_pThis)
    {
        delete m_pThis;
        m_pThis = NULL;
    }
}

ProxyManager::ProxyManager(void)
    : m_TLSProtocol(SP_PROT_TLS1_2)
	, m_hTdsTaskDispatchThread(NULL)
	, m_hSocketCleanThread(NULL)
    , m_bInitComplete(false)
    , m_instancePort(0)
    , m_certContext(NULL)
{
   InitializeCriticalSection(&m_csProxyChannel);
   InitializeCriticalSection(&m_csEndSockets);
   InitializeCriticalSection(&m_csChannelClean);

	//init notify event
	m_hSocketDataReadyEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	m_hTdsDataReadyEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	m_hEventHaveEndSocket = CreateEventW(NULL, FALSE, FALSE, NULL);
}


ProxyManager::~ProxyManager(void)
{
}

void ProxyManager::Init()
{
    if (m_bInitComplete)
        return;

    //Log init
    theLog.Init(4, LOG_POLICY_StdErr | LOG_POLICY_File);

	//read config
	try
	{
		theConfig.ReadConfig();
        theLog.SetLevel(theConfig.GetLogLevel());
	}
	catch (std::exception& ex)
	{
        PROXYLOG(CELOG_EMERG, "ReadConfig exception: %s", ex.what());
	}

    // load SQLTask.dll
    if (ProxyDataAnalysis::GetInstance()->Init() == false)
    {
        PROXYLOG(CELOG_EMERG, L"ProxyDataAnalysis init failed!");
    }

	//load interface within frame.dll
	theTCPFrame = TCPFrame::GetInstance();
	if(!theTCPFrame->LoadTCPFrame())
	{
        PROXYLOG(CELOG_EMERG, L"Load Frame.dll failed. exit!");
	}

	//Load Security library
	if (!LoadSecurityLibrary())
	{
        PROXYLOG(CELOG_EMERG, L"SQLProxy:LoadSecurityLibrary failed. exit.");
	}

    // Init certificate context
    InitCertContext();

	//init tds data manager
	theProxyDataMgr  = ProxyDataMgr::GetInstance();
    theProxyDataMgr->Init(m_hTdsDataReadyEvent, m_hSocketDataReadyEvent);

    m_bInitComplete = true;

    PROXYLOG(CELOG_NOTICE, "All initialization works are done");
    PROXYLOG(CELOG_NOTICE, L"DAE for SQL started.");
}

BOOL ProxyManager::InitCertContext()
{
    SECURITY_STATUS status = CertificateHelper::CertFindCertificateByName(m_certContext, theConfig.CertificateSubject().c_str());
    if (FAILED(status))
    {
        PROXYLOG(CELOG_EMERG, L"Error can't find a certificate");
        return FALSE;
    }

    return TRUE;
}

void ProxyManager::ServerStartEvent(TcpSocketPtr tcpSocket)
{
    //connect to server
    boost::shared_ptr<TcpSocket> svrSocket;
    boost::system::error_code errorcode;
    std::string port;

    if (!theConfig.RemoteServerInstance().empty() && ProxyManager::GetInstance()->GetInstancePort() != 0) {
        port = std::to_string(ProxyManager::GetInstance()->GetInstancePort());
    }
    else {
        port = ProxyCommon::UnicodeToUTF8(theConfig.RemoteServerPort());
    }

    bool bConnect = theTCPFrame->BlockConnect(
        (char*)ProxyCommon::UnicodeToUTF8(theConfig.RemoteServer()).c_str(),
        (char*)port.c_str(),
        svrSocket, errorcode);

    if (bConnect)
    {
        ProxyChannelPtr pProxyChannel(new ProxyChannel());
        if (!pProxyChannel->InitTlsServerCred())
        {
            theTCPFrame->Close(tcpSocket);
            theTCPFrame->Close(svrSocket);
            return;
        }

        if (!pProxyChannel->InitTlsClientCred())
        {
            theTCPFrame->Close(tcpSocket);
            theTCPFrame->Close(svrSocket);
            return;
        }

        if (theConfig.kerberosAuthOpen())
        {
            pProxyChannel->InitKrbServerCred();
            pProxyChannel->InitKrbClientCred();
        }

        pProxyChannel->SetClientSocket(tcpSocket);
        pProxyChannel->SetServerSocket(svrSocket);

        CriticalSectionLock lockChannels(&m_csProxyChannel);
        m_ProxyChannels[tcpSocket.get()] = pProxyChannel;
        m_ProxyChannels[svrSocket.get()] = pProxyChannel;
    }
    else
    {
        //close client socket
        PROXYLOG(CELOG_EMERG, L"Connect to remote server failed [server: %s][port: %s][error_msg: %s].\n",
            theConfig.RemoteServer().c_str(), theConfig.RemoteServerPort().c_str(), ProxyCommon::StringToWString(errorcode.message()).c_str());

        theTCPFrame->Close(tcpSocket);
    }
}


void ProxyManager::EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{
    theProxyDataMgr->CleanSocket(tcpSocket.get());

	ProxyChannelPtr pChannel = GetProxyChannel(tcpSocket.get());
	if (pChannel){
		
		if (CHANNEL_CLOSE_CAUSE_SERVER_ROUTING == pChannel->GetChannelCloseCause())
		{
            {
                CriticalSectionLock lockChannels(&m_csProxyChannel);
                this->m_ProxyChannels.erase(tcpSocket.get());
                pChannel->SetChannelCloseCause(CHANNEL_CLOSE_CAUSE_NONE);
            }

			return;
		}

        pChannel->SetChannelClose(true);

        if (pChannel->IsClientSocket(tcpSocket.get()))
        {
            // PROXYLOG(CELOG_EMERG, L"Normal close client socket: %#x, this channel server socket: %#x", tcpSocket, pChannel->GetServerSocket().get());
            theTCPFrame->Close(pChannel->GetClientSocket());
            pChannel->SetClientSocket(NULL);
            theTCPFrame->Close(pChannel->GetServerSocket());
        }
        else if (pChannel->IsServerSocket(tcpSocket.get()))
        {
            // PROXYLOG(CELOG_EMERG, L"Normal close server socket: %#x, this channel client socket: %#x", tcpSocket, pChannel->GetClientSocket().get());
            theTCPFrame->Close(pChannel->GetServerSocket());
            pChannel->SetServerSocket(NULL);
            theTCPFrame->Close(pChannel->GetClientSocket());
        }
        else
        {
            theTCPFrame->Close(tcpSocket);
            theTCPFrame->Close(pChannel->GetClientSocket());
            theTCPFrame->Close(pChannel->GetServerSocket());
        }

        {
            CriticalSectionLock cs(&(GetInstance()->m_csProxyChannel));
            GetInstance()->m_ProxyChannels.erase(tcpSocket.get());
        }
    }
    else
    {
        //PROXYLOG(CELOG_ERR, "SocketCleanThread socket can not get channel. socket: %p", tcpSocket.get());
        theTCPFrame->Close(tcpSocket);
    }
}


ProxyChannelPtr ProxyManager::GetProxyChannel(void* tcpSocket)
{
	CriticalSectionLock lockChannels(&m_csProxyChannel);

	std::map<void*, ProxyChannelPtr>::iterator itChannel = m_ProxyChannels.find(tcpSocket);
	if (itChannel != m_ProxyChannels.end())
	{
		return itChannel->second;
	}

	return NULL;
}

void ProxyManager::AddSocketChannelMap(void* tcpSocket, ProxyChannelPtr pChannel)
{
	CriticalSectionLock lockChannels(&m_csProxyChannel);
	m_ProxyChannels[tcpSocket] = pChannel;

}

BOOL ProxyManager::LoadSecurityLibrary( void ) // load SSPI.DLL, set up a special table - PSecurityFunctionTable
{
	INIT_SECURITY_INTERFACE pInitSecurityInterface;
	//  QUERY_CREDENTIALS_ATTRIBUTES_FN pQueryCredentialsAttributes;
    OSVERSIONINFO VerInfo = {0};
    CHAR lpszDLL[MAX_PATH] = {0};

	//  Find out which security DLL to use, depending on
	//  whether we are on Win2K, NT or Win9x
	VerInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if ( !GetVersionEx (&VerInfo) ) return FALSE;

	if ( VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT  &&  VerInfo.dwMajorVersion == 4 )
	{
		strcpy (lpszDLL, "Security.dll" ); 
	}
	else if ( VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ||
		VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		strcpy(lpszDLL, "Secur32.dll"); 
	}
	else 
	{
        PROXYLOG(CELOG_EMERG, L"Not supported system platform. Can't load SSPI interface. PlatformId: %d MajorVer: %d MinorVer: %d", VerInfo.dwPlatformId, VerInfo.dwMajorVersion, VerInfo.dwMinorVersion);
        assert(false);
        return FALSE;
	}

	//  Load Security DLL
	HMODULE hSecurity = LoadLibraryA(lpszDLL);
	if(hSecurity == NULL) 
	{
        PROXYLOG(CELOG_EMERG, "Error 0x%x loading %s.", GetLastError(), lpszDLL);
        assert(false);
		return FALSE;
	}

	pInitSecurityInterface = (INIT_SECURITY_INTERFACE)GetProcAddress( hSecurity, "InitSecurityInterfaceW" );
	if(pInitSecurityInterface == NULL) 
	{
        PROXYLOG(CELOG_EMERG, L"Error 0x%x reading InitSecurityInterface entry point.", GetLastError());
        assert(false);
		return FALSE;
	}

	SecurityFunTable = pInitSecurityInterface(); 
	if(SecurityFunTable == NULL)
	{
        PROXYLOG(CELOG_EMERG, L"Error 0x%x reading security interface.", GetLastError());
        assert(false);
		return FALSE;
	}

	return TRUE;
}