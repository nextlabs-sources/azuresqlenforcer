#ifndef SQLENFORCER_COMMONLIB_INCLUDE_EMDBCONFIG_H
#define SQLENFORCER_COMMONLIB_INCLUDE_EMDBCONFIG_H

// Used as cipher to check whether log sql
#define LOG_CIPHER 4312 // TODO: Remove it

#include <atomic>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#pragma region Enums
enum class LogLevel
{
    kTrace = 0,
    kDebug,
    kInfo,
    kWarning,
    kError,
    kFatal
};

enum class Usermode
{
    kApplication = 0,
    kDatabase,
    kActiveDirectory
};

enum class HandlerType
{
    kDeny = 0,
    kAllow
};

enum class ConnectionType
{
    kNone = 0,
    kJdbc,
    kOdbc,
    kAdoNet,
    kOci
};
#pragma endregion

class EMDBConfig
{
public:
    struct WorkerConfig {
        std::string worker_name_;
        std::string remote_server_;
        std::string remote_server_port_;
        std::string remote_server_instance_;
        std::string local_proxy_port_;
        bool use_azure_sql = false;
        std::string odbc_connection_string_;
    };

#pragma region Static methods
    static EMDBConfig& GetInstance();
    static std::string GenerateErrorMessage(const std::string& key, bool have_default_value);
#pragma endregion

#pragma region Other methods
    bool Load(const std::string& config_file_path, const std::string& worker_name = "");
#pragma endregion Other methods

#pragma region Getters
    std::string get_global_install_path() const;
    std::string get_policy_cchost() const;
    std::string get_policy_ccport() const;
    std::string get_policy_jpchost() const;
    std::string get_policy_jpcport() const;
    std::string get_policy_jpcuser() const;
    std::string get_policy_jpcpwd() const;
    std::string get_policy_ccuser() const;
    std::string get_policy_ccpwd() const;
    std::string get_policy_modelname() const;
    const unsigned int get_policy_cc_sync_time() const;
    bool get_policy_cc_switch_is_on() const;
    bool get_policy_jpc_switch_is_on() const;
    bool get_policy_jpc_cache_is_on() const;
    bool get_policy_pc_parse_is_on() const;
    unsigned int get_jpc_cache_clear_time() const;
    std::string get_cc_tag() const;

    LogLevel get_log_level() const;
    bool get_log_auditlogall() const;
    unsigned get_log_switch_report() const;
    unsigned get_log_max_size() const;
    unsigned get_log_min_free_space() const;
    unsigned get_log_max_files() const;

    Usermode get_usermode_mode() const;
    std::string get_usermode_jdbc_conn_string() const;
    std::string get_usermode_odbc_conn_string() const;
    std::string get_usermode_adonet_sqlclient_conn_string() const;
    std::string get_usermode_oci_conn_string() const;
    std::string get_usermode_user_info() const;
    unsigned get_usermode_sync_time() const;
    std::string get_usermode_multi_value_separator() const;

    HandlerType get_dae_error_exception_handler() const;
    HandlerType get_dae_error_star_handler() const;

    std::string get_metadata_odbc_conn_string() const;

    bool get_odbc_gettablestructure_switch() const;
    std::string get_odbc_driver() const;
    std::string get_odbc_uid() const;
    std::string get_odbc_pwd() const;
    std::string get_odbc_others() const;

    std::vector<std::string> get_initial_errors(const std::string type = "") const;
    std::vector<std::string> get_initial_errors(const ConnectionType type) const;
    const std::string & get_sqluser_format_l() const;
    const uint32_t get_sqluser_format_o() const;
    const std::string & get_sqluser_format_r() const;
    bool get_sql_user_keep()const;
    uint8_t get_user_source()const;
    
    const std::map<std::string, WorkerConfig>& get_tdsproxyworkers() const;

    bool get_tdsproxyworker_config(const std::string& worker_name, WorkerConfig& worker_config) const;
#pragma endregion

#pragma region Setters
    void set_global_install_path(const std::string& install_path);
    void set_policy_cchost(const std::string& cchost);
    void set_policy_ccport(const std::string& ccport);
    void set_policy_jpchost(const std::string& jpchost);
    void set_policy_jpcport(const std::string& jpcport);
    void set_policy_jpcuser(const std::string& jpcuser);
    void set_policy_jpcpwd(const std::string& jpcpwd);
    void set_policy_ccuser(const std::string& jpcuser);
    void set_policy_ccpwd(const std::string& jpcpwd);
    void set_policy_cc_sync_time(const unsigned int iseconds);

    void set_policy_modelname(const std::string& modelname);

    void set_log_level(const LogLevel level);
    void set_log_auditlogall(const bool auditlogall);
    void set_log_switch_report(const unsigned int switch_report);
    void set_log_max_size(const unsigned log_max_size);
    void set_log_min_free_space(const unsigned min_free_space);
    void set_log_max_files(const unsigned max_files);

    void set_usermode_mode(const Usermode mode);
    void set_usermode_jdbc_conn_string(const std::string& conn_string);
    void set_usermode_odbc_conn_string(const std::string& conn_string);
    void set_usermode_adonet_sqlclient_conn_string(const std::string& conn_string);
    void set_usermode_oci_conn_string(const std::string& conn_string);
    
    void set_usermode_user_info(const std::string& user_info);
    void set_usermode_sync_time(const unsigned int sync_time);
    void set_usermode_multi_value_separator(const std::string& multi_value_separator);

    void set_dae_error_exception_handler(const std::string& exception_handler);
    void set_dae_error_star_handler(const std::string& star_handler);

    void set_metadata_odbc_conn_string(const std::string& conn_string);

    void set_odbc_gettablestructure_switch(const bool gettablestructure_switch);
    void set_odbc_driver(const std::string& driver);
    void set_odbc_uid(const std::string& uid);
    void set_odbc_pwd(const std::string& pwd);
    void set_odbc_others(const std::string& others);

    void add_tdsproxyworker_config(const std::string& worker_name,
                                   const std::string& remote_server,
                                   const std::string& remote_server_port,
                                   const std::string& remote_server_instance,
                                   const std::string& local_proxy_port,
                                   const std::string& odbc_connection_string,
                                   int use_azure_sql);

    void parser_sql_userattr_format(const std::string& src, std::string& l, uint32_t& o, std::string& r);//o is operator, 1=equal 0=notEqual
#pragma endregion

private:
#pragma region Constructors
    EMDBConfig();

    EMDBConfig(const EMDBConfig&) = delete;
    EMDBConfig(EMDBConfig&&) = delete;
    EMDBConfig& operator=(const EMDBConfig&) = delete;
    EMDBConfig& operator=(EMDBConfig&&) = delete;
#pragma endregion

#pragma region Member variables
    static std::atomic<bool> loaded_;

    std::string global_install_path_;

    std::string policy_cchost_;
    std::string policy_ccport_;
    std::string policy_jpchost_;
    std::string policy_jpcport_;
    std::string policy_jpcuser_;
    std::string policy_jpcpwd_;
    std::string policy_ccuser_;
    std::string policy_ccpwd_;
    std::string policy_modelname_;
    unsigned int _cc_sync_time = 0;
    unsigned int _cc_switch = 1;
    unsigned int _jpc_switch = 1;
    unsigned int _jpc_caches = 1;
    unsigned int _jpc_parse = 1;
    unsigned int _jpc_cache_clear_time = 3600;
    std::string _cc_tag;

    LogLevel log_level_ = LogLevel::kInfo;
    bool log_auditlogall_ = false;
    unsigned log_switch_report_ = 0; // TODO: Chang into bool
    unsigned log_max_size_ = 1024;
    unsigned log_min_free_space_ = 100;
    unsigned log_max_files_ = 512;

    Usermode usermode_mode_ = Usermode::kDatabase;

    std::string usermode_jdbc_conn_string_;
    std::string usermode_odbc_conn_string_;
    std::string usermode_adonet_sqlclient_conn_string_;
    std::string usermode_oci_conn_string_;
    std::string usermode_user_info_;
    unsigned usermode_sync_time_ = 86400;    // Unit seconds; 86400s == 1 day
    std::string usermode_multi_value_separator_ = ",";

    HandlerType dae_error_exception_handler_ = HandlerType::kDeny;
    HandlerType dae_error_star_handler_ = HandlerType::kDeny;

    std::string metadata_odbc_conn_string_;

    bool odbc_gettablestructure_switch_ = false;
    std::string odbc_driver_;
    std::string odbc_uid_;
    std::string odbc_pwd_;
    std::string odbc_others_;

    std::vector<std::string> initial_error;

    std::string _sqluser_format_l;
    uint32_t _sqluser_format_o;
    std::string _sqluser_format_r;
    bool _sql_user_keep;
    uint8_t _user_source = 7;

    std::map<std::string, WorkerConfig> tdsproxyworkers_workers_;
#pragma endregion
};
#define USER_SOURCE_SQL_USER 4
#define USER_SOURCE_AD_USER 2
#define USER_SOURCE_DB_USER 1

#endif //SQLENFORCER_COMMONLIB_INCLUDE_EMDBCONFIG_H
