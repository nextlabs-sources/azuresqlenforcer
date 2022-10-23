#include <boost/asio.hpp>

#include "TDSConfigCheck.h"

#include <stdint.h>
#include "commfun.h"
#include "INIReader.h"
#include "CertificateHelper.h"
#include "SQLServerInstanceHelper.h"
#include "logger_class.h"
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/regex.hpp>

bool CheckSQLServerHost(const std::string& host, const uint16_t& port)
{
    daebootstrap::Logger::Instance().Info("start to check MSSQL Server host........");

    bool res = false;

    boost::system::error_code error;
    boost::asio::io_service io_service;
    boost::asio::io_context io_context;

	boost::asio::ip::tcp::resolver resolver(io_service);  
	boost::asio::ip::tcp::resolver::query query(host.c_str(), std::to_string(port).c_str());
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query, error);
    boost::asio::ip::tcp::socket sock(io_context);
	
	boost::asio::connect(sock, iterator, error);
    if (!error)
        res = true;

    daebootstrap::Logger::Instance().Info("check MSSQL Server host end, result: %s........\n", res ? "success" : "failed");
    return res;
}

bool CheckSQLServerInstance(const std::string& instance, const std::string& host)
{
    daebootstrap::Logger::Instance().Info("start to check MSSQL Server instance........");

    bool res = TryGetInstancePort(instance.c_str(), host.c_str());

    daebootstrap::Logger::Instance().Info("check MSSQL Server instance end, result: %s........\n", res ? "success" : "failed");
    return res;
}

bool CheckLocalPortForProxy(const uint16_t& port)
{
    daebootstrap::Logger::Instance().Info("start to check local port for proxy........");

    bool res = false;

    daebootstrap::Logger::Instance().Info("check local port for proxy end, result: %s........\n", res ? "success" : "failed");
    return res;
}

bool CheckCertification(const std::string& cert)
{
    daebootstrap::Logger::Instance().Info("start to check certification........");

    bool res = false;

    do
    {
        if (!LoadSecurityLibrary())
            break;
        
        if (CertFindCertificateByName(cert.c_str()) != SEC_E_OK)
            break;

        if (CreateCredentialsFromCertificate() != SEC_E_OK)
            break;
        
        res = true;
    } while (0);
    
    daebootstrap::Logger::Instance().Info("check certification end, result: %s........\n", res ? "success" : "failed");
    return res;
}

bool CheckTDSConfig()
{
    std::string config_path = CommonFun::GetConfigFilePath();

    int krb_swtich = 0;
    std::string krb_username;
    std::string krb_password;
    std::string krb_domain;

    std::string workerstring;
    int guard_elapsed_time = 0;
    std::string remote_server;
    int remote_server_port = 0;
    std::string remote_server_instance;
    int local_proxy_port = 0;
    std::string database_spn;
    std::string cert_subject;
    int proxy_log_level = 0;
    std::vector<std::string> workers;

    try
    {
        INIReader reader(config_path);
        
        krb_swtich = reader.GetInteger("Kerberos", "kerberos_switch", 0);
        if (krb_swtich != 1 && krb_swtich != 0)
        {
            daebootstrap::Logger::Instance().Error("kerberos_switch must be '1' or '0', otherwise it will use default value '0'.");
            return false;
        }

        // Check kerberos config, only if kerberos authentication is open.
        if (krb_swtich == 1)
        {
            krb_username = reader.GetString("Kerberos", "windows_auth_username", "");
            krb_password = reader.GetString("Kerberos", "windows_auth_password", "");
            krb_domain = reader.GetString("Kerberos", "domain", "");

            boost::trim(krb_username);
            boost::trim(krb_password);
            boost::trim(krb_domain);
            if (krb_username.empty() || krb_password.empty() || krb_domain.empty())
            {
                daebootstrap::Logger::Instance().Error("windows_auth_username, windows_auth_password or domain must not be empty.");
                return false;
            }
        }
        else
        {
            daebootstrap::Logger::Instance().Info("Proxy kerberos authentication is off.");
        }

        cert_subject = reader.GetString("TDSProxyCommon", "cert_subject", "");
        proxy_log_level = reader.GetInteger("TDSProxyCommon", "proxy_log_level", 4);

        if (proxy_log_level < 0 || proxy_log_level > 7) {
            daebootstrap::Logger::Instance().Error("proxy_log_level in TDSProxyCommon must be between (0,7)");
            return false;
        }

        if (cert_subject.empty()) {
            daebootstrap::Logger::Instance().Error("cert_subject in TDSProxyCommon must not be empty.");
            return false;
        }
        else {
            if (!CheckCertification(cert_subject))
                return false;
        }

        workerstring = reader.GetString("TDSProxyWorkers", "workers", "");
        
        boost::split(workers, workerstring, boost::is_any_of(","), boost::token_compress_on);

        for each (std::string s in workers)
        {
            bool is_ok = true;
            boost::trim(s);
            if (s.empty())
                continue;

            daebootstrap::Logger::Instance().Info("start to check proxy config of worker %s", s.c_str());

            auto it = boost::find_if(s, boost::is_any_of("[].*\\/:?\"<>|"));
            if (it != s.end())
            {
                daebootstrap::Logger::Instance().Error("Worker name can not contain these character '[].*\\/:?\"<>|'");
                return false;
            }

            remote_server = reader.GetString(s, "remote_server", "");
            remote_server_port = reader.GetInteger(s, "remote_server_port", 0);
            remote_server_instance = reader.GetString(s, "remote_server_instance", "");
            local_proxy_port = reader.GetInteger(s, "local_proxy_port", 0);
            database_spn = reader.GetString(s, "database_spn", "");

            if (remote_server_instance.empty()) {
                if (remote_server.empty() || remote_server_port == 0) {
                    daebootstrap::Logger::Instance().Error("remote_server or remote_server_port in worker %s must not be empty.", s.c_str());
                    is_ok = false;
                }
                else {
                    if (remote_server_port <= 0 || remote_server_port > 0x0000FFFF) {
                        daebootstrap::Logger::Instance().Error("remote_server_port in worker %s must not be between (1, 65535).", s.c_str());
                        is_ok = false;
                    }
                    else
                        is_ok &= CheckSQLServerHost(remote_server, remote_server_port);
                }
            }
            else {
                if (local_proxy_port == 1433) {
                    daebootstrap::Logger::Instance().Error("In worker %s, must not set local_proxy_port to 1433 if set remote_server_instance.", s.c_str());
                    is_ok = false;
                }
                else
                    is_ok &= CheckSQLServerInstance(remote_server_instance, remote_server);
            }

            if (local_proxy_port <= 0 || local_proxy_port > 0x0000FFFF) {
                daebootstrap::Logger::Instance().Error("In worker %s, local_proxy_port must not be between (1, 65535).", s.c_str());
                is_ok = false;
            }

            if (krb_swtich == 1)
            {
                if (database_spn.empty())
                {
                    is_ok = false;
                    daebootstrap::Logger::Instance().Error("In worker %s, database_spn must not be empty.", s.c_str());
                }
                else
                {
                    boost::regex reg("MSSQLSvc/(.+\\.)+.+:\\d+");
                    if (!boost::regex_match(database_spn, reg))
                    {
                        is_ok = false;
                        daebootstrap::Logger::Instance().Error("In worker %s, database_spn must be in format 'MSSQLSvc/FQDN:[port numbe]'.", s.c_str());
                    }
                }
            }

            if (!is_ok) {
                daebootstrap::Logger::Instance().Error("config of worker %s have something wrong.\n", s.c_str());
                return false;
            }
        }
    }
    catch (std::exception e)
    {
        daebootstrap::Logger::Instance().Error("read TDSProxy config had a exception: %s", e.what());
        return false;
    }

    return true;
}