#ifndef SQL_PROXY_CONFIG_H
#define SQL_PROXY_CONFIG_H
#include <stdint.h>
#include <string>

class Config
{
public:
	Config();
	~Config();

public:
	bool ReadConfig();

public:
    std::wstring& RemoteServer() { return strRemoteServer; }
    std::wstring& RemoteServerPort() { return strRemoteServerPort;  }
    std::wstring& RemoteServerInstance() { return strRemoteServerInstance; }
	std::wstring& CertificateSubject() { return strCertificateSubject;  }
    std::wstring& LocalProxyPort() { return strLocalProxyPort; }
    std::wstring& DatabaseSPN() { return strDatabaseSPN; }
    bool NeedPrintSocketData() { return m_bPrintSocketData; }
    uint32_t GetLogLevel() { return m_LogLevel; }
    uint32_t GetTaskThreadpool() { return m_TaskThreadpool; }

    // kerberos
    bool kerberosAuthOpen() { return m_krbAuthOpen; }
    std::wstring& GetKrbUsername() { return strKrbUsername; }
    std::wstring& GetKrbPassword() { return strKrbPassword; }
    std::wstring& GetKrbDomain() { return strKrbDomain; }

private:
	std::wstring strRemoteServer;
	std::wstring strRemoteServerPort;
    std::wstring strRemoteServerInstance;
    std::wstring strLocalProxyPort;
    std::wstring strDatabaseSPN;
	std::wstring strCertificateSubject;
    bool m_bPrintSocketData;

    // Log
    uint32_t m_LogLevel;
    uint32_t m_TaskThreadpool;

    // kerberos
    bool m_krbAuthOpen;
    std::wstring strKrbUsername;
    std::wstring strKrbPassword;
    std::wstring strKrbDomain;
};


extern Config theConfig;

#endif 

