#include "EMDBConfig.h"

#include <regex>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "commfun.h"
#include "INIReader.h"
#include <stdexcept>

#pragma region Static variables
std::atomic<bool> EMDBConfig::loaded_ { false };
#pragma endregion Static variables

#pragma region Static methods
EMDBConfig& EMDBConfig::GetInstance()
{
    static EMDBConfig config;
    return config;
}

std::string EMDBConfig::GenerateErrorMessage(const std::string &key, bool have_default_value)
{
    if (have_default_value)
    {
        return "define a wrong value for " + key + " parameter, will use the default value";
    }
    else
    {
        return "define a wrong value for " + key + " parameter";
    }
}
#pragma endregion

#pragma region Other methods
bool EMDBConfig::Load(const std::string& config_file_path, const std::string& worker_name) {
  if (!loaded_.load()) {
    std::ifstream file(config_file_path);
    if (!file.is_open())
    {
      std::string error_message = "Can not open file " + config_file_path;
      initial_error.push_back(error_message);
      return false;
    }
    file.close();

    INIReader ini_reader(config_file_path);
    if (ini_reader.ParseError() < 0) {
      std::string msg = "Config file parse failed! file: " + config_file_path;
      initial_error.push_back(msg);
      return false;
    }

    set_policy_cchost(ini_reader.Get("policy", "cchost", ""));
    set_policy_ccport(ini_reader.Get("policy", "ccport", ""));
    set_policy_jpchost(ini_reader.Get("policy", "jpchost", ""));
    set_policy_jpcport(ini_reader.Get("policy", "jpcport", ""));
    set_policy_jpcuser(ini_reader.Get("policy", "jpcuser", ""));
    set_policy_jpcpwd(ini_reader.Get("policy", "jpcpwd", ""));
    set_policy_ccuser(ini_reader.Get("policy", "ccuser", ""));
    set_policy_ccpwd(ini_reader.Get("policy", "ccpwd", ""));
    set_policy_modelname(ini_reader.Get("policy", "componenttypename", ""));

    unsigned int iseconds = (unsigned)ini_reader.GetReal("policy", "sync_time", 180);
    set_policy_cc_sync_time(iseconds);

    set_log_level((LogLevel)ini_reader.GetInteger("LOG", "level", 3));
    set_log_auditlogall(ini_reader.GetInteger("LOG", "auditlogall", 0)==1);
    set_log_switch_report(ini_reader.GetInteger("LOG", "report_switch", 0));
    set_log_max_size(ini_reader.GetInteger("LOG","max_size", 1024));
    set_log_min_free_space(ini_reader.GetInteger("LOG", "min_free_space", 100));
    set_log_max_files(ini_reader.GetInteger("LOG", "max_files", 512));

    _cc_switch = ini_reader.GetInteger("policy", "prefilter_switch", 1);

    _jpc_switch = ini_reader.GetInteger("policy", "jpc_switch", 1);
    _jpc_caches = ini_reader.GetInteger("policy", "jpc_cache", 1);
    _jpc_parse = ini_reader.GetInteger("policy", "jpc_parse", 1);
    _jpc_cache_clear_time = ini_reader.GetInteger("policy", "jpc_cache_clear_time", 3600);
    if (_jpc_cache_clear_time < 600) {
        _jpc_cache_clear_time = 600;
    }
    _cc_tag = ini_reader.GetString("policy", "policy_tag", "");

    set_usermode_multi_value_separator(ini_reader.Get("USERMODE", "multi_value_separator", ","));

    std::string format_ = ini_reader.Get("USERMODE", "user_attr_format", "");
    parser_sql_userattr_format(format_, _sqluser_format_l, _sqluser_format_o, _sqluser_format_r);
    _sql_user_keep = ini_reader.GetInteger("USERMODE", "sql_condition_change", 0) <= 0 ? true : false;
    _user_source = ini_reader.GetInteger("USERMODE", "source", 5);
    if (_user_source > 5 || _user_source <= 0) {
      _user_source = 5;
    }
    set_dae_error_exception_handler(ini_reader.Get("DAE_ERROR", "exception_handler", "deny"));
    set_dae_error_star_handler(ini_reader.Get("DAE_ERROR", "star_handler", "deny"));

    set_metadata_odbc_conn_string(ini_reader.Get("METADATA", "odbc_conn_string", ""));

    set_odbc_gettablestructure_switch(ini_reader.GetInteger("ODBC", "gettablestructure_switch", 0)==1);
    set_odbc_driver(ini_reader.Get("ODBC", "driver", ""));
    set_odbc_uid(ini_reader.Get("ODBC", "uid", ""));
    set_odbc_pwd(ini_reader.Get("ODBC", "pwd", ""));
    set_odbc_others(ini_reader.Get("ODBC", "others", ""));

    if (!worker_name.empty()) {
      const auto remote_server = ini_reader.Get(worker_name, "remote_server", "");
      const auto remote_server_port = ini_reader.Get(worker_name, "remote_server_port", "");
      const auto remote_server_instance = ini_reader.Get(worker_name, "remote_server_instance", "");
      const auto local_proxy_port = ini_reader.Get(worker_name, "local_proxy_port", "");
      const auto use_azure_sql = ini_reader.GetInteger(worker_name, "use_azure_sql", 0);

      std::string server{};
      if (remote_server_instance.empty()) {
        server = CommonFun::StringFormat(
            "%s,%s",
            remote_server.c_str(),
            remote_server_port.c_str()
        );
      }
      else {
        server = CommonFun::StringFormat(
            "%s\\%s",
            remote_server.c_str(),
            remote_server_instance.c_str()
        );
      }

      const auto odbc_conn_string = CommonFun::StringFormat(
          "Driver=%s;Server=%s;Uid=%s;Pwd=%s;%s",
          get_odbc_driver().c_str(),
          server.c_str(),
          get_odbc_uid().c_str(),
          get_odbc_pwd().c_str(),
          get_odbc_others().c_str()
      );

      add_tdsproxyworker_config(worker_name, remote_server, remote_server_port, remote_server_instance, local_proxy_port, odbc_conn_string, use_azure_sql);
    }

    loaded_.store(true);
    return true;
  }

  return false;
}
#pragma endregion Other methods

#pragma region Getters
std::string EMDBConfig::get_global_install_path() const
{
    return global_install_path_;
}

std::string EMDBConfig::get_policy_cchost() const
{
    return policy_cchost_;
}

std::string EMDBConfig::get_policy_ccport() const {
    return policy_ccport_;
}

std::string EMDBConfig::get_policy_jpchost() const {
    return policy_jpchost_;
}

std::string EMDBConfig::get_policy_jpcport() const {
    return policy_jpcport_;
}

std::string EMDBConfig::get_policy_jpcuser() const
{
    return policy_jpcuser_;
}

std::string EMDBConfig::get_policy_jpcpwd() const
{
    return policy_jpcpwd_;
}

std::string EMDBConfig::get_policy_ccuser() const
{
    return policy_ccuser_;
}

std::string EMDBConfig::get_policy_ccpwd() const
{
    return policy_ccpwd_;
}

std::string EMDBConfig::get_policy_modelname() const
{
    return policy_modelname_;
}

const unsigned int EMDBConfig::get_policy_cc_sync_time() const {
    return _cc_sync_time;
}

bool  EMDBConfig::get_policy_cc_switch_is_on() const {
    return _cc_switch == 0 ? false : true;
}

bool  EMDBConfig::get_policy_jpc_switch_is_on() const {
    return _jpc_switch == 0 ? false : true;
}

bool  EMDBConfig::get_policy_jpc_cache_is_on() const {
    return _jpc_caches == 0 ? false : true;
}

bool  EMDBConfig::get_policy_pc_parse_is_on() const {
    return _jpc_parse == 0 ? false : true;
}

unsigned int EMDBConfig::get_jpc_cache_clear_time() const {
    return _jpc_cache_clear_time ;
}



std::string  EMDBConfig::get_cc_tag() const {
    return _cc_tag;
}

LogLevel EMDBConfig::get_log_level() const
{
    return log_level_;
}

bool EMDBConfig::get_log_auditlogall() const
{
    return log_auditlogall_;
}

unsigned EMDBConfig::get_log_switch_report() const
{
    return log_switch_report_;
}

unsigned EMDBConfig::get_log_max_size() const
{
    return log_max_size_;
}

unsigned EMDBConfig::get_log_min_free_space() const
{
    return log_min_free_space_;
}

unsigned EMDBConfig::get_log_max_files() const
{
    return log_max_files_;
}

Usermode EMDBConfig::get_usermode_mode() const
{
    return usermode_mode_;
}

std::string EMDBConfig::get_usermode_jdbc_conn_string() const
{
    return usermode_jdbc_conn_string_;
}

std::string EMDBConfig::get_usermode_odbc_conn_string() const
{
    return usermode_odbc_conn_string_;
}

std::string EMDBConfig::get_usermode_adonet_sqlclient_conn_string() const
{
    return usermode_adonet_sqlclient_conn_string_;
}

std::string EMDBConfig::get_usermode_oci_conn_string() const
{
    return usermode_oci_conn_string_;
}

std::string EMDBConfig::get_usermode_user_info() const
{
    return usermode_user_info_;
}

unsigned EMDBConfig::get_usermode_sync_time() const
{
    return usermode_sync_time_;
}

std::string EMDBConfig::get_usermode_multi_value_separator() const
{
    return usermode_multi_value_separator_;
}

HandlerType EMDBConfig::get_dae_error_exception_handler() const
{
    return dae_error_exception_handler_;
}

HandlerType EMDBConfig::get_dae_error_star_handler() const
{
    return dae_error_star_handler_;
}

std::string EMDBConfig::get_metadata_odbc_conn_string() const
{
    return metadata_odbc_conn_string_;
}

bool EMDBConfig::get_odbc_gettablestructure_switch() const
{
    return odbc_gettablestructure_switch_;
}

std::string EMDBConfig::get_odbc_driver() const
{
    return odbc_driver_;
}

std::string EMDBConfig::get_odbc_uid() const
{
    return odbc_uid_;
}

std::string EMDBConfig::get_odbc_pwd() const
{
    return odbc_pwd_;
}

std::string EMDBConfig::get_odbc_others() const
{
    return odbc_others_;
}

std::vector<std::string> EMDBConfig::get_initial_errors(const std::string type) const
{
    ConnectionType conn_type = ConnectionType::kNone;

    if (CommonFun::CaseInsensitiveEquals(type, "jdbc"))
    {
        conn_type = ConnectionType::kJdbc;
    }
    else if (CommonFun::CaseInsensitiveEquals(type, "odbc"))
    {
        conn_type = ConnectionType::kOdbc;
    }
    else if (CommonFun::CaseInsensitiveEquals(type, "adonet"))
    {
        conn_type = ConnectionType::kAdoNet;
    }
    else if (CommonFun::CaseInsensitiveEquals(type, "oci"))
    {
        conn_type = ConnectionType::kOci;
    }

    return get_initial_errors(conn_type);
}

std::vector<std::string> EMDBConfig::get_initial_errors(const ConnectionType type) const
{
    auto errors = initial_error;

    if (Usermode::kDatabase == usermode_mode_)
    {
        if (ConnectionType::kJdbc == type)
        {
            if (usermode_jdbc_conn_string_.empty())
            {
                errors.push_back("[USERMODE.jdbc_conn_string] is incorrect in config.ini.");
            }
        }
        else if (ConnectionType::kOdbc == type)
        {
            if (usermode_odbc_conn_string_.empty())
            {
                errors.push_back("[USERMODE.odbc_conn_string] is incorrect in config.ini.");
            }
        }
        else if (ConnectionType::kAdoNet == type)
        {
            if (usermode_adonet_sqlclient_conn_string_.empty())
            {
                errors.push_back("[USERMODE.adonet_sqlclient_conn_string] is incorrect in config.ini.");
            }
        }
        else if (ConnectionType::kOci == type)
        {
            if (usermode_oci_conn_string_.empty())
            {
                errors.push_back("[USERMODE.oci_conn_string] is incorrect in config.ini.");
            }
        }
    }


    return errors;
}

const std::string& EMDBConfig::get_sqluser_format_l() const
{
    return _sqluser_format_l;
}

const uint32_t EMDBConfig::get_sqluser_format_o() const
{
    return _sqluser_format_o;
}

const std::string& EMDBConfig::get_sqluser_format_r() const
{
    return _sqluser_format_r;
}

const std::map<std::string, EMDBConfig::WorkerConfig>& EMDBConfig::get_tdsproxyworkers() const
{
    return tdsproxyworkers_workers_;
}

bool EMDBConfig::get_tdsproxyworker_config(const std::string& worker_name, EMDBConfig::WorkerConfig& worker_config) const
{
    const auto it = tdsproxyworkers_workers_.find(worker_name);

    if (it != tdsproxyworkers_workers_.end())
    {
        worker_config = it->second;
        return true;
    }

    return false;
}
bool EMDBConfig::get_sql_user_keep()const {
    return _sql_user_keep;
}
uint8_t EMDBConfig::get_user_source()const {
    return _user_source;
}

#pragma endregion

#pragma region Public setters
void EMDBConfig::set_global_install_path(const std::string& install_path)
{
    if (!install_path.empty())
    {
        global_install_path_ = install_path;
    }
    else
    {
        initial_error.push_back("[GLOBAL.install_path] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_cchost(const std::string &cchost)
{
    if (CommonFun::IsValidHttpUrl(cchost))
    {
        policy_cchost_ = cchost;
    }
    else
    {
        initial_error.push_back("[POLICY.cchost] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_ccport(const std::string &ccport)
{
    if (CommonFun::IsValidPort(ccport))
    {
        policy_ccport_ = ccport;
    }
    else
    {
        initial_error.push_back("[POLICY.ccport] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_jpchost(const std::string &jpchost)
{
    if (CommonFun::IsValidHttpUrl(jpchost))
    {
        policy_jpchost_ = jpchost;
    }
    else
    {
        initial_error.push_back("[POLICY.jpchost] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_jpcport(const std::string &jpcport)
{
    if (CommonFun::IsValidPort(jpcport))
    {
        policy_jpcport_ = jpcport;
    }
    else
    {
        initial_error.push_back("[POLICY.jpcport] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_jpcuser(const std::string &jpcuser)
{
    if (!jpcuser.empty())
    {
        policy_jpcuser_ = jpcuser;
    }
    else
    {
        initial_error.push_back("[POLICY.jpcuser] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_jpcpwd(const std::string &jpcpwd)
{
    if (!jpcpwd.empty())
    {
        policy_jpcpwd_ = CommonFun::DecryptString(jpcpwd);

        if (policy_jpcpwd_.empty())
        {
            initial_error.push_back("[POLICY.jpcpwd] is incorrect in config.ini. It cannot be decrypted.");
        }
    }
    else
    {
        initial_error.push_back("[POLICY.jpcpwd] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_ccuser(const std::string& ccuser)
{
    if (!ccuser.empty())
    {
        policy_ccuser_ = ccuser;
    }
    else
    {
        initial_error.push_back("[POLICY.ccuser] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_ccpwd(const std::string& ccpwd)
{
    if (!ccpwd.empty())
    {
        policy_ccpwd_ = CommonFun::DecryptString(ccpwd);
        if (policy_ccpwd_.empty()) {
            initial_error.push_back("[POLICY.ccpwd] is incorrect in config.ini. It cannot be decrypted");
        }
    }
    else
    {
        initial_error.push_back("[POLICY.ccpwd] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_policy_cc_sync_time(const unsigned int iseconds) {
    if (iseconds != 0 )
    {
        _cc_sync_time = iseconds;
    }
    else
    {
        initial_error.push_back("Invalid ccpwd");
    }
}

void EMDBConfig::set_policy_modelname(const std::string &modelname)
{
    if (!modelname.empty())
    {
        policy_modelname_ = modelname;
    }
    else
    {
        initial_error.push_back("[POLICY.componenttypename] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_log_level(const LogLevel level)
{
    if (level >= LogLevel::kTrace && level <= LogLevel::kFatal)
    {
        log_level_ = level;
    }
    else
    {
        initial_error.push_back("[Log.level] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_log_auditlogall(const bool auditlogall)
{
    log_auditlogall_ = auditlogall;
}

void EMDBConfig::set_log_switch_report(const unsigned switch_report)
{
    if (switch_report == 0 || switch_report == 1 || switch_report == LOG_CIPHER)
    {
        log_switch_report_ = switch_report;
    }
    else
    {
        initial_error.push_back("[Log.switch_report] is incorrect in config.ini. It will use report_switch=0");
    }
}

void EMDBConfig::set_log_max_size(const unsigned log_max_size)
{
    const auto max = (std::numeric_limits<unsigned>::max)();

    if (log_max_size > 0 && log_max_size < max)
    {
        log_max_size_ = log_max_size;
    }
    else
    {
        initial_error.push_back("[Log.max_size] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_log_min_free_space(const unsigned log_min_free_space)
{
    const auto max = (std::numeric_limits<unsigned>::max)();

    if (log_min_free_space > 0 && log_min_free_space < max)
    {
        log_min_free_space_ = log_min_free_space;
    }
    else
    {
        initial_error.push_back("[Log.min_free_space] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_log_max_files(const unsigned log_max_files)
{
    const auto max = (std::numeric_limits<unsigned>::max)();

    if (log_max_files > 0 && log_max_files < max)
    {
        log_max_files_ = log_max_files;
    }
    else
    {
        initial_error.push_back("[Log.max_files] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_usermode_mode(const Usermode mode)
{
    if (mode >= Usermode::kApplication && mode <= Usermode::kActiveDirectory)
    {
        usermode_mode_ = mode;
    }
    else
    {
        initial_error.push_back("[USERMODE.mode] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_usermode_jdbc_conn_string(const std::string &conn_string)
{
    usermode_jdbc_conn_string_ = conn_string;
}

void EMDBConfig::set_usermode_odbc_conn_string(const std::string &conn_string)
{
    usermode_odbc_conn_string_ = CommonFun::DecryptString(conn_string);
}

void EMDBConfig::set_usermode_adonet_sqlclient_conn_string(const std::string& conn_string)
{
    usermode_adonet_sqlclient_conn_string_ = CommonFun::DecryptString(conn_string);
}

void EMDBConfig::set_usermode_oci_conn_string(const std::string& conn_string)
{
    usermode_oci_conn_string_ =  CommonFun::DecryptString(conn_string);
}

void EMDBConfig::set_usermode_user_info(const std::string &user_info)
{
    if (!user_info.empty())
    {
        usermode_user_info_ = user_info;
    }
    else if (Usermode::kDatabase == usermode_mode_)
    {
        initial_error.push_back("[USERMODE.user_info] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_usermode_sync_time(const unsigned days)
{
    if (days > 0 && days < 366)
    {
        usermode_sync_time_ = days * 86400;
    }
    else
    {
        initial_error.push_back("[USERMODE.sync_time] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_usermode_multi_value_separator(const std::string& multi_value_separator)
{
    if (!multi_value_separator.empty())
    {
        usermode_multi_value_separator_ = multi_value_separator;
    }
    else
    {
        initial_error.push_back("[USERMODE.multi_value_separator] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_dae_error_exception_handler(const std::string& exception_handler)
{
    if (CommonFun::CaseInsensitiveEquals(exception_handler, "deny"))
    {
        dae_error_exception_handler_ = HandlerType::kDeny;
    }
    else if (CommonFun::CaseInsensitiveEquals(exception_handler, "allow"))
    {
        dae_error_exception_handler_ = HandlerType::kAllow;
    }
    else
    {
        initial_error.push_back("[DAE_ERROR.exception_handler] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_dae_error_star_handler(const std::string& star_handler)
{
    if (CommonFun::CaseInsensitiveEquals(star_handler, "deny"))
    {
        dae_error_star_handler_ = HandlerType::kDeny;
    }
    else if (CommonFun::CaseInsensitiveEquals(star_handler, "allow"))
    {
        dae_error_star_handler_ = HandlerType::kAllow;
    }
    else
    {
        initial_error.push_back("[DAE_ERROR.star_handler] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_metadata_odbc_conn_string(const std::string& conn_string)
{
    if (!conn_string.empty())
    {
        metadata_odbc_conn_string_ = CommonFun::DecryptString(conn_string);
    }
    else
    {
        //throw std::runtime_error("metadata_odbc_conn_string must not be empty!");
    }
}

void EMDBConfig::set_odbc_gettablestructure_switch(const bool gettablestructure_switch)
{
    odbc_gettablestructure_switch_ = gettablestructure_switch;
}

void EMDBConfig::set_odbc_driver(const std::string& driver)
{
    if (!driver.empty())
    {
        odbc_driver_ = driver;
    }
    else
    {
        initial_error.push_back("[ODBC.driver] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_odbc_uid(const std::string& uid)
{
    if (!uid.empty())
    {
        odbc_uid_ = uid;
    }
    else
    {
        initial_error.push_back("[ODBC.uid] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_odbc_pwd(const std::string& pwd)
{
    if (!pwd.empty())
    {
        odbc_pwd_ = CommonFun::DecryptString(pwd);
        
        if (policy_ccpwd_.empty()) {
            initial_error.push_back("[ODBC.pwd] is incorrect in config.ini. It cannot be decrypted");
        }
    }
    else
    {
        initial_error.push_back("[ODBC.pwd] is incorrect in config.ini.");
    }
}

void EMDBConfig::set_odbc_others(const std::string& others)
{
    if (CommonFun::IsEmptyString(others))
    {
        return;
    }

    std::regex double_quoted{R"d("(.*)")d"};
    std::smatch match_result;

    if (std::regex_match(others, match_result, double_quoted))
    {
        auto double_quoted_content = match_result[1].str();
        if (CommonFun::IsValidOdbcConnectionString(double_quoted_content))
        {
            odbc_others_ = double_quoted_content;
        }
        else
        {
            initial_error.push_back("[ODBC.others] is incorrect in config.ini. It is not valid ODBC connection string. "
                                    "The valid value should be like \"Trusted_Connection=Yes;\" or \"Trusted_Connection=Yes;TrustServerCertificate=Yes;\"");
        }
    } else {
        initial_error.push_back("[ODBC.others] is incorrect in config.ini. It should be empty or enclosed in double quotes.");
    }
}


void EMDBConfig::add_tdsproxyworker_config(const std::string& worker_name,
                                           const std::string& remote_server,
                                           const std::string& remote_server_port,
                                           const std::string& remote_server_instance,
                                           const std::string& local_proxy_port,
                                           const std::string& odbc_connection_string,
                                           int use_azure_sql)
{
    WorkerConfig config;

    if (CommonFun::IsNotEmptyString(worker_name))
    {
        config.worker_name_ = worker_name;
    }
    else
    {
        initial_error.push_back(worker_name + " worker name is incorrect in config.ini.");
    }

    if (CommonFun::IsNotEmptyString(remote_server))
    {
        config.remote_server_ = remote_server;
    }
    else
    {
        initial_error.push_back(
            CommonFun::StringFormat("[%s.remote_server] is incorrect in config.ini.", worker_name.c_str())
        );
    }

    if (CommonFun::IsEmptyString(remote_server_port) &&
        CommonFun::IsEmptyString(remote_server_instance))
    {
        initial_error.push_back(CommonFun::StringFormat(
            "[%s.remote_server_port] and [%s.remote_server_instance] cannot be null at the same time in config.ini.",
            worker_name.c_str(),
            worker_name.c_str()));
    }
    else
    {
        config.remote_server_port_ = remote_server_port;
        config.remote_server_instance_ = remote_server_instance;
    }

    if (CommonFun::IsValidPort(local_proxy_port))
    {
        config.local_proxy_port_ = local_proxy_port;
    }
    else
    {
        initial_error.push_back(
            CommonFun::StringFormat("[%s.local_proxy_port] is incorrect in config.ini.", worker_name.c_str())
        );
    }

    if (use_azure_sql != 1 && use_azure_sql != 0)
    {
        initial_error.push_back(
            CommonFun::StringFormat("[%s.use_azure_sql] only accept value '1' or '0', it will use '0' as default if not.", worker_name.c_str())
        );
    }
    else
    {
        config.use_azure_sql = (use_azure_sql == 1);
    }

    if (CommonFun::IsValidOdbcConnectionString(odbc_connection_string))
    {
        config.odbc_connection_string_ = odbc_connection_string;
    }
    else
    {
        initial_error.push_back(
            CommonFun::StringFormat("Bad ODBC connection string for %s worker.", worker_name.c_str())
        );
    }

    tdsproxyworkers_workers_[worker_name] = config;
}
#pragma endregion

#pragma region Constructors
EMDBConfig::EMDBConfig()
{
}

void EMDBConfig::parser_sql_userattr_format(const std::string& src, std::string& _sqluser_format_l,
    uint32_t& _sqluser_format_o,
    std::string& _sqluser_format_r) {
    if (src.empty()) {
        _sqluser_format_l = "'DAE_APP_USER'";
        _sqluser_format_o = 0;
        _sqluser_format_r = "id";
        return;
    }
    std::string l, o, r;
    uint64_t pos = 0;
    while (pos < src.length()) {
        char c = src[pos];
        switch (c) {
        case '=':
        case '!':
        case '>':
        case '<': {
            if (l.empty()) {
                break;
            }
            pos++;
            o += c;
        }break;
        case '\'':
        case '"': {
            std::string t;
            if (c != '"') {
                t += src[pos];
            }

            pos++;
            while (c != src[pos] && pos < src.length()) {
                t += src[pos];
                pos++;
            }
            if (c != '"' && pos < src.length()) {
                t += src[pos];
            }
            pos++;

            if (l.empty()) {
                l = t;
            }
            else {
                r = t;
            }
        }break;
        case ' ':
        case '\r':
        case '\t': {
            pos++;
        }break;
        default:
        {
            std::string t;
            t += src[pos];
            pos++;
            while (pos < src.length()) {
                if (' ' == src[pos] || '\t' == src[pos] || '\r' == src[pos] || '<' == src[pos] || '!' == src[pos] || '=' == src[pos]) {
                    break;
                }
                t += src[pos];
                pos++;
            }
            if (l.empty()) {
                l = t;
            }
            else {
                r = t;
            }
        }break;
        }

    }
    //
    if (l.empty()) {
        initial_error.push_back("USERMODE.user_attr_format is wrong format.");
        _sqluser_format_l = "'DAE_APP_USER'";
    }
    else {
        _sqluser_format_l = l;
    }
    //
    if (o == "<>" || o == "^=" || o == "!=") {
        _sqluser_format_o = 0;
    }
    else if (o == "=") {
        _sqluser_format_o = 1;
    }
    else {
        initial_error.push_back("USERMODE.user_attr_format is wrong format.");
        _sqluser_format_o = 0;
    }
    //
    if (r.empty()) {
        _sqluser_format_r = "";
    }
    else {
        _sqluser_format_r = r;
    }

}
#pragma endregion