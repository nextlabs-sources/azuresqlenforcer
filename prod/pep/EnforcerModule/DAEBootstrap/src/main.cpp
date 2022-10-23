#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include "EMDBConfig.h"
#include "logger_class.h"
#include "query_cloudaz_sdk_cpp_wrapper_class.h"
#include "daeccpolicymgr_dll_wrapper_class.h"
#include "DAEServiceMgr.h"
#include "TDSConfigCheck.h"
#include "WorkerPolicyCacheCheck.h"
#include "commfun.h"

#include "check_result_enum.h"

using daebootstrap::Logger;
using daebootstrap::QueryCloudAzSdkCppWrapper;
using daebootstrap::DaeccpolicymgrDllWrapper;

const std::vector<std::string> files{
   "config/config.ini",
   "celog.dll",
   "DAECCPolicyMgr.dll",
   "frame.dll",
   "jsoncpp.dll",
   "LIBEAY32.dll",
   "ProxyMain.exe",
   "ProxyWorker.exe",
   "QueryCloudAZSDKcpp.dll",
   "SQL2003.dll",
   "SQLEnforcer.dll",
   "SQLProxy.dll",
   "SQLTask.dll",
   "SSLEAY32.dll",
   "UserAttribute.dll",
   "zlib1.dll"
};

bool CheckConfig(std::vector<std::string> &errors) noexcept {
    //  Check EnforcerModule config
    const auto& dae_config = EMDBConfig::GetInstance();
    auto initial_errors = dae_config.get_initial_errors();

    if (initial_errors.size() > 0) {
        errors = initial_errors;
        return false;
    }

    // Check TDS config
    if (!CheckTDSConfig()) {
        return false;
    }
    else {
        Logger::Instance().Info("TDS proxy config is all fine!\n");
    }

    return true;
}

bool CheckFilesExist(const std::vector<std::string> &files,
                     std::vector<std::string> &errors) noexcept {
    bool result = true;

    const auto program_directory_path = boost::dll::program_location().parent_path();
    
    for (const auto& file : files) {
        if (!boost::filesystem::exists(program_directory_path / file)) {
            errors.emplace_back(file + " is missing.");
            result = false;
        }
    }

    return result;
}

std::vector<std::string> g_vecJPCLog;
std::vector<std::string> g_vecCCLog;

int jpc_log(int level, const char* log) {
    if (level >= 4) {
        g_vecJPCLog.push_back(log);
    }
    return 0;
}
int cc_log(int level, const char* log) {
    if (level >= 4) {
        g_vecCCLog.push_back(log);
    }
    return 0;
}

bool CheckQueryJpc() noexcept {
    const auto& logger = Logger::Instance();
    const auto& dae_config = EMDBConfig::GetInstance();

    const auto cchost   = dae_config.get_policy_cchost();
    const auto ccport   = dae_config.get_policy_ccport();
    const auto jpchost  = dae_config.get_policy_jpchost();
    const auto jpcport  = dae_config.get_policy_jpcport();
    const auto jpcuser  = dae_config.get_policy_jpcuser();
    const auto jpcpwd   = dae_config.get_policy_jpcpwd();
    
    if (cchost.empty()  ||
        ccport.empty()  ||
        jpchost.empty() ||
        jpcport.empty() ||
        jpcuser.empty() ||
        jpcpwd.empty()) {
        logger.Error("JPC module initialization is failed, please check config.");
        return false;
    }

    auto& query_cloud_az_sdk_wrapper = QueryCloudAzSdkCppWrapper::Instance();

    if (!query_cloud_az_sdk_wrapper.Load()) {
        logger.Error("Faild to load QueryCloudAZSDKcpp.dll");
        return false;
    }
    
    if (query_cloud_az_sdk_wrapper.QueryCloudAZInit(
        jpchost.c_str(),
        jpcport.c_str(),
        cchost.c_str(),
        ccport.c_str(),
        jpcuser.c_str(),
        jpcpwd.c_str(),
        8,
        jpc_log
    )) {
        logger.Info("Successfully initalize JPC module.");
        logger.Info("Start query JPC:");

        auto req = query_cloud_az_sdk_wrapper.CreatePolicyRequest();
        // Action must be set
        req->SetAction("SELECT");

        // User attributes must be set
        auto user = query_cloud_az_sdk_wrapper.CreateCEAttr();
        user->AddAttribute("TEST", "TEST", XACML_string);
        req->SetUserInfo("unknown", "unknown", user);

        // Source attributes must be set
        auto source = query_cloud_az_sdk_wrapper.CreateCEAttr();
        source->AddAttribute("TEST", "TEST", XACML_string);
        req->SetSource("TEST:TEST", dae_config.get_policy_modelname().c_str(), source);

        // Set host info
        req->SetHostInfo("test", "255.255.255.255", nullptr);

        // Set APP info
        req->SetAppInfo("bootstrap", "bootstrap", "unknown", nullptr);

        IPolicyResult* result = nullptr;
        auto status = query_cloud_az_sdk_wrapper.CheckSingleResource(req, &result);

        if (nullptr != req->GetUserAttr()) {
            query_cloud_az_sdk_wrapper.FreeCEAttr(const_cast<IAttributes*>(req->GetUserAttr()));
        }

        if (nullptr != req->GetSourceAttr()) {
            query_cloud_az_sdk_wrapper.FreeCEAttr(const_cast<IAttributes*>(req->GetSourceAttr()));
        }

        query_cloud_az_sdk_wrapper.FreePolicyRequest(req);

        if (QS_S_OK != status) {
            if (QS_E_DisConnect != status) {
                logger.Error("JPC connection error, please check [Policy]jpchost/jpcport.");
            }

            return false;
        }
        else {
            query_cloud_az_sdk_wrapper.FreePolicyResult(result);
            return true;
        }
    }

    return false;
}

bool CheckPolicyCache(const std::string& worker) noexcept {
    const auto& config_path = CommonFun::GetConfigFilePath();
    const auto& logger = Logger::Instance();

    if (!EMDBConfig::GetInstance().Load(config_path, worker)) {
        logger.Error("Failed to load config from %s: %s", config_path.c_str());
        return false;
    }

    auto errors = EMDBConfig::GetInstance().get_initial_errors();
    if (errors.size() > 0)
    {
        for (auto e : errors)
        {
            logger.Error(e);
        }
        return false;
    }

    const auto& dae_config = EMDBConfig::GetInstance();

    const auto cchost = dae_config.get_policy_cchost();
    const auto ccport = dae_config.get_policy_ccport();
    const auto ccuser = dae_config.get_policy_ccuser();
    const auto ccpwd = dae_config.get_policy_ccpwd();
    const auto modelname = dae_config.get_policy_modelname();
    //const auto metadata_odbc_conn_string = dae_config.get_metadata_odbc_conn_string();
    const auto cc_tag = dae_config.get_cc_tag();
    const auto cc_sync_time = dae_config.get_policy_cc_sync_time();

    EMDBConfig::WorkerConfig worker_config;
    if (!dae_config.get_tdsproxyworker_config(worker, worker_config)) {
        logger.Error("can not find worker %s config", worker.c_str());
        return false;
    }
    
    std::string metadata_odbc_conn_string;
    bool use_azure_sql = worker_config.use_azure_sql;
    std::string server;
    
    if (worker_config.remote_server_instance_.empty()) {
        server = CommonFun::StringFormat(
            "%s,%s",
            worker_config.remote_server_.c_str(),
            worker_config.remote_server_port_.c_str()
        );
    }
    else {
        server = CommonFun::StringFormat(
            "%s\\%s",
            worker_config.remote_server_.c_str(),
            worker_config.remote_server_instance_.c_str()
        );
    }

    metadata_odbc_conn_string = CommonFun::StringFormat(
        "Driver=%s;Server=%s;Uid=%s;Pwd=%s;%s",
        dae_config.get_odbc_driver().c_str(),
        server.c_str(),
        dae_config.get_odbc_uid().c_str(),
        dae_config.get_odbc_pwd().c_str(),
        dae_config.get_odbc_others().c_str()
    );

    if (cchost.empty() ||
        ccport.empty() ||
        ccuser.empty() ||
        ccpwd.empty() ||
        modelname.empty() ||
        metadata_odbc_conn_string.empty()) {
        logger.Error("JPC module initialization is failed, please check config.");
        return false;
    }

    auto& daeccpolicymgr_dll_wrapper = DaeccpolicymgrDllWrapper::Instance();

    if (!daeccpolicymgr_dll_wrapper.Load()) {
        logger.Error("Failed to load DAECCPolicyMgr.dll");
        return false;
    }

    S4HException s4hexc;

    if (!daeccpolicymgr_dll_wrapper.PolicyInit(
        cchost,
        ccport,
        ccuser,
        ccpwd,
        modelname,
        cc_tag,
        cc_sync_time,
        s4hexc,
        cc_log
    ) || g_vecCCLog.size() > 0) {
        logger.Error("Failed to initialize CC module.");
        return false;
    }

    TablePolicyInfoMap table_policy_info_map;

    if (!daeccpolicymgr_dll_wrapper.GetTablePolicyinfo(
        table_policy_info_map,
        s4hexc
    )) {
        logger.Error("Failed to get table policy infos.");
        return false;
    }

    std::ostringstream oss;
    oss << "CheckPolicies:" << std::endl;

    for (const auto& table_policy_info : table_policy_info_map) {
        const auto& table_name = table_policy_info.first;
        const auto& policy_info = table_policy_info.second;

        if (!policy_info._bfilter &&
            !policy_info._bmask) {
            oss << utils::StringFormat("Table: %s access is deny.",
                table_name.c_str())
                << std::endl;
        }
        else {
            oss << utils::StringFormat("Table: %s PredicateCondition=%s Mask=%s",
                policy_info._bfilter ? "TRUE" : "FALSE",
                policy_info._bmask ? "TRUE" : "FALSE")
                << std::endl;
        }
    }

    return true;
}

bool StartDaeService() noexcept {
    const auto& logger = Logger::Instance();

    const auto exe_name = "ProxyMain.exe";
    const auto service_name = "DAE for SQL Server Service";

    DAEServiceMgr daesrv{ exe_name, service_name };

    if (!daesrv.OpenSCMgr()) {
        const auto error_code = ::GetLastError();
        std::string error_detail{};

        if (ERROR_ACCESS_DENIED == error_code) {
            error_detail = "Please run this program with administrator privileges.";
        }
        else if (ERROR_DATABASE_DOES_NOT_EXIST == error_code) {
            error_detail = "The specified database does not exist.";
        }
        else {
            // Should not be reached.
        }

        logger.Error("Error happened when open service manager: %s.", error_detail.c_str());
        return false;
    }

    bool bcreate = daesrv.CreateDaeService();
    if (!bcreate) {
        //Already installed
    }
    daesrv.StopDaeService();

    if (!daesrv.StartDaeService()) {
        logger.Error("Error happened when start service: %s.", CommonFun::ShareLibraryError().c_str());
        return false;
    }

    return true;
}

void NormalCheck()
{
    CheckResult result = CheckResult::kSucceed;
    const auto& logger = Logger::Instance();
    std::vector<std::string> errors{};

    // Check config
    {
        logger.Info("==Start checking config==");

        try
        {
            if (EMDBConfig::GetInstance().Load(CommonFun::GetConfigFilePath()) &&
                CheckConfig(errors)) {
                logger.Info("Configuration file initialization is normal.");
            }
            else {
                for (const auto& err : errors) {
                    logger.Error("%s", err.c_str());
                }

                errors.clear();
                result |= CheckResult::kConfigFailed;
            }
        }
        catch (std::exception e)
        {
            logger.Error("%s", e.what());

            errors.clear();
            result |= CheckResult::kConfigFailed;
        }

        logger.Info("==End checking config==\n\n");
    }

#ifndef _DEBUG
    // Check file exists
    {
        logger.Info("==Start checking file exists==");

        if (CheckFilesExist(files, errors)) {
            logger.Info("All files exists.");
        }
        else {
            for (const auto& err : errors) {
                logger.Error("%s", err.c_str());
            }

            errors.clear();
            result |= CheckResult::kFileMissingFailed;
        }

        logger.Info("==End checking file exists==\n\n");
    }

    // Check JPC
    if ((result & CheckResult::kConfigFailed) == CheckResult::kSucceed)
    {
        logger.Info("==Start checking query JPC==");
        if (CheckQueryJpc() && g_vecJPCLog.empty()) {
            logger.Info("Successfully query JPC");
        }
        else {
            for (const auto& err_msg : g_vecJPCLog) {
                logger.Error("%s", err_msg.c_str());
            }

            logger.Error("Failed to query JPC");

            result |= CheckResult::kQueryJpcFailed;
        }
        logger.Info("==End checking query JPC==\n\n");
    }
#endif

    // Check policy cache of workers
    if ((result & CheckResult::kConfigFailed) == CheckResult::kSucceed)
    {
        logger.Info("==Start checking workers policy cache==");

        if (WorkerPolicyCacheCheck()) {
            logger.Info("Successfully cached policy.");
        }
        else {
            logger.Error("Failed to cache policy.");
            result |= CheckResult::kQueryCCPolicyCacheFailed;
        }

        logger.Info("==End checking policy cache==\n\n");
    }

    // Start service
    if (CheckResult::kSucceed == result) {
        logger.Info("==Start service==");

        if (StartDaeService()) {
            logger.Info("Successfully start service.");
        }
        else {
            logger.Error("Failed to start service.");
        }

        logger.Info("==End start service==\n\n");
    }
}

int main(int argc, char **argv) 
{
    if (argc >= 3)
    {
        std::string worker;
        bool bstart_sub = false;
        for (int i = 0; i < argc; i++) {
            if (_stricmp(argv[i], "-sub") == 0)
                bstart_sub = true;
            else if (_stricmp(argv[i], "-w") == 0) {
                if (i + 1 < argc) {
                    worker = argv[i + 1] ? argv[i + 1] : "";
                }
            }
        }

        if (bstart_sub) {
            if (!CheckPolicyCache(worker) || g_vecCCLog.size() > 0) {
                for (const auto& err_msg : g_vecCCLog) {
                    Logger::Instance().Error("%s", err_msg.c_str());
                }
            }
        }
    }
    else if (argc <= 1)
    {
        NormalCheck();

        Logger::Instance().Info("Press any key to exit the program.....");
        std::cin.get();
    }

	return EXIT_SUCCESS;
}