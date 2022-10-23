#include "Config.h"
#include "CommonFunc.h"
#include "DAEEncryptTool.h"
#include "INIReader.h"
#include "Log.h"
#include <string.h>
#include <boost\thread\thread.hpp>
#include <boost\algorithm\string.hpp>

Config theConfig;
extern HMODULE g_hThisModule;

const char KerberosSection[] = "Kerberos";
const char windowsAuthUsername[] = "windows_auth_username";
const char windowsAuthPassword[] = "windows_auth_password";
const char domain[] = "domain";
const char krbSwitch[] = "kerberos_switch";

const char TDSProxyCommon[] = "TDSProxyCommon";
const char Level[] = "proxy_log_level";
const char subject[] = "cert_subject";
const char TaskThreadpool[] = "task_thread_pool";
const char SocketDataTraceLogOpen[] = "socket_data_trace_on";

const char SvrRemoteServer[] = "remote_server";
const char SvrRemoteServerPort[] = "remote_server_port";
const char SvrRemoteServerInstance[] = "remote_server_instance";
const char SvrLocalProxyPort[] = "local_proxy_port";
const char SvrSPN[] = "database_spn";


Config::Config()
    :m_bPrintSocketData(false)
    , m_LogLevel(4)
    , m_krbAuthOpen(false)
{
    
}


Config::~Config()
{
}


bool Config::ReadConfig()
{
    char path[300] = { 0 };
    GetModuleFileNameA(NULL, path, 300);
    char* p = strrchr(path, '\\');
    if (p) *p = 0;
    strcat_s(path, "\\config\\config.ini");

    const std::string& worker_name = ProxyCommon::GetWorkerNameA();
    std::string strValue;
    INIReader iniread(path);

    // Tds common
    m_LogLevel = iniread.GetInteger(TDSProxyCommon, Level, 4);

    m_TaskThreadpool = iniread.GetInteger(TDSProxyCommon, TaskThreadpool, 50);
    if (m_TaskThreadpool < boost::thread::hardware_concurrency())
        m_TaskThreadpool = boost::thread::hardware_concurrency();

    strValue = iniread.GetString(TDSProxyCommon, subject, "");
    boost::trim(strValue);
    strCertificateSubject = ProxyCommon::StringToWString(strValue);

    m_bPrintSocketData = (iniread.GetInteger(TDSProxyCommon, SocketDataTraceLogOpen, 0) == 1);

    // kerberos
    m_krbAuthOpen = iniread.GetInteger(KerberosSection, krbSwitch, 0) == 1;
    strKrbUsername = ProxyCommon::UTF8ToUnicode(iniread.GetString(KerberosSection, windowsAuthUsername, ""));
    boost::trim(strKrbUsername);
    strKrbDomain = ProxyCommon::UTF8ToUnicode(iniread.GetString(KerberosSection, domain, ""));
    boost::trim(strKrbDomain);
    strValue = iniread.GetString(KerberosSection, windowsAuthPassword, "");
    if (!strValue.empty())
    {
        boost::trim(strValue);
        AesEncryptor aes;
        strKrbPassword = ProxyCommon::UTF8ToUnicode(aes.DecryptString(strValue));
    }
    
    strValue = iniread.GetString(worker_name.c_str(), SvrRemoteServer, "");
    boost::trim(strValue);
    strRemoteServer = ProxyCommon::StringToWString(strValue);
    
    strValue = iniread.GetString(worker_name.c_str(), SvrRemoteServerPort, "");
    boost::trim(strValue);
    strRemoteServerPort = ProxyCommon::StringToWString(strValue);
    
    strValue = iniread.GetString(worker_name.c_str(), SvrRemoteServerInstance, "");
    boost::trim(strValue);
    strRemoteServerInstance = ProxyCommon::StringToWString(strValue);

    strValue = iniread.GetString(worker_name.c_str(), SvrLocalProxyPort, "1433");
    boost::trim(strValue);
    strLocalProxyPort = ProxyCommon::StringToWString(strValue);

    strDatabaseSPN = ProxyCommon::UTF8ToUnicode(iniread.GetString(worker_name, SvrSPN, ""));
    boost::trim(strDatabaseSPN);

	return true;
}
