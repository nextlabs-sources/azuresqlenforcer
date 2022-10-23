#include "DAELog.h"
#include "enforcermgr.h"
#include "sqlparserwrapper.h"
#include "policymgrwrapper.h"
#include "userattrwrapper.h"
#include "EMDBConfig.h"
#include "commfun.h"
#include "PolicyResource.h"
#include "usercontextinfo.h"
#include "ODBCMgrApi.h"
#include "assert.h"
#include "QueryCAZPolicyOpr.h"
#include "QueryCloudAZSDKCppWrapper.h"
#include "policymgrwrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

#include "nanodbc/nanodbc.h"

CEAttributeType parser_value_type(const std::string& src, const std::string& val_separator);

using namespace std::chrono;

#define MAX_TABLE 10


#include "SqlException.h"
#include "sql_collect.h"
#include <chrono>

#include "table_metadata_cache_manager.h"
#include "default_schema_cache_manager.h"

using namespace std::chrono;
#define MAGIC_NUMBER UINT64_MAX

const char * DAE_IGNORE_FLAG = "--NXLDAE";

ISqlCollect *theSqlCollector = nullptr;

void ComposePolicyResource(CPolicyResource &pResourceAttr, /*const*/ UserContextInfo *pUserCtx,/*const*/  ITableItem* tbi);
void ComposePolicyResource(CPolicyResource &pResourceAttr, /*const*/ UserContextInfo *pUserCtx,/*const*/  IStoreProcedure* pProcedure);
void query_pc_from_stmt(SharedPtrVector<QueryPcResult>& vecresult, const std::string & action, std::vector<ITableItem*> & tbis, SQLParseCtx* parse_ctx, SqlException * pExc);
void get_from_tables(std::vector<ITableItem*> & tbis, IStmt* stmt);
void get_action_tables(std::vector<ITableItem*> & tbis, IStmt* stmt);


CEnforcerMgr::CEnforcerMgr()
{
}
/*
bool CEnforcerMgr::Init(const char *szModuleName, const char *szCfgFile, SqlException * pExc)
{
	if (m_strModuleName.empty()) {
		m_strModuleName = szModuleName;
	}
	// else if (CommonFun::StrCaseCmp(m_strModuleName.c_str(), szModuleName) != 0) {
	// 	return FALSE;
	// }

    //log init
    CLog::Instance()->InitLog(m_strModuleName);
#ifdef _WIN32
        DWORD id = ::GetCurrentProcessId();
    theLog->WriteLog(log_info, "SQLEnforcer::Init %d", id);
#endif
    theLog->WriteLog(log_info, "SQLEnforcer::Init");

    //config file
    theLog->WriteLog(log_info, "Read config from:%s", szCfgFile);

    try
    {
        if (CommonFun::CaseInsensitiveEquals(szModuleName, "jdbc"))
        {
            EMDBConfig::Initialize(ConnectionType::kJdbc);
        }
        else if (CommonFun::CaseInsensitiveEquals(szModuleName, "odbc"))
        {
            EMDBConfig::Initialize(ConnectionType::kOdbc);
        }

        const auto& emdb_config = EMDBConfig::GetInstance();
        auto initial_errors = emdb_config.get_initial_errors();

        for (const auto& error : initial_errors)
        {
            theLog->WriteLog(log_error, error.c_str());
        }
    }
    catch (const std::exception& e)
    {
        pExc->SetInfo(ERR_CONFIG, e.what());
        return false;
    }

    // Set default log level to warning
    theLog->UpdateLogLevel("3");

    const auto& emdb_config = EMDBConfig::GetInstance();

    try
    {
        auto log_level = std::to_string(static_cast<int>(emdb_config.get_log_level())); // TODO: Change into enum
        //update log level
        theLog->UpdateLogLevel(log_level);
    }
    catch (const std::exception& e)
    {
        theLog->WriteLog(log_error, "Failed to set log level. Use the default log level.");
    }

//    //init ODBCAPI 
    if(m_strModuleName.compare("jdbc") != 0) {
        if(!ODBCMgrApi::Instance().m_bInit) {
            pExc->SetInfo(ERR_LIBRARY, detail_err_lib_odbc);
            theLog->WriteLog(log_fatal, "Load ODBCAPI failed.");
            return false;
        }
    }

    //load sqlparser
    if (!CSqlparserWrapper::Instance()->LoadParser())
    {
        pExc->SetInfo(ERR_LIBRARY, detail_err_lib_sql2003);
        theLog->WriteLog(log_fatal, "Load sql parser failed.");
        return false;
    }
    theLog->WriteLog(log_info, "LoadParser succeed.");

    //load userattribute module
    if (!UserAttrWrapper::Instance()->LoadUserAttributeModule())
    {
        pExc->SetInfo(ERR_LIBRARY, detail_err_lib_userattribute);
        theLog->WriteLog(log_fatal, "Load user attribute module failed.");
        return false;
    }
    theLog->WriteLog(log_info, "LoadUserAttribute Module succeed.");

    //const int user_mode = static_cast<int>(emdb_config.get_usermode_mode());
    //auto odbc_conn_string = emdb_config.get_usermode_odbc_conn_string();
    //auto user_info = emdb_config.get_usermode_user_info();
    auto val_separator = emdb_config.get_usermode_multi_value_separator();
    //auto sync_time = emdb_config.get_usermode_sync_time();

    //UserAttrWrapper::Instance()->UserAttrInit(SQLEnforcerWriteLogA,
    //                                          user_mode,
    //                                          odbc_conn_string.c_str(),
    //                                          user_info.c_str(),
    //                                          m_strModuleName.c_str(),
    //                                          val_separator.c_str(),
    //                                          (int)sync_time);
    UserAttrWrapper::Instance()->UserAttrInit(SQLEnforcerWriteLogA,
        0,
        "",
        "",
        m_strModuleName.c_str(),
        val_separator.c_str(),
        (int)1);
    theLog->WriteLog(log_info, "UserAttrInit succeed.");


    if (!QueryCloudAZSDKCppWrapper::Instance()->LoadQueryCloudAZSDKCpp())
    {
        theLog->WriteLog(log_fatal, "Load query policy failed.");
        pExc->SetInfo(ERR_LIBRARY, detail_err_lib_policymgr);
        return false;
    }
    if (!QueryCloudAZSDKCppWrapper::Instance()->QueryCloudAZInit(
            emdb_config.get_policy_jpchost().c_str(),
            emdb_config.get_policy_jpcport().c_str(),
            emdb_config.get_policy_cchost().c_str(),
            emdb_config.get_policy_ccport().c_str(),
            emdb_config.get_policy_jpcuser().c_str(),
            emdb_config.get_policy_jpcpwd().c_str(),
            1,
            [](int, const char*) { return 0; }))
    {
        pExc->SetInfo(ERR_POLICY, "QueryCloudAZInit failed.");
        _exc.SetInfo(ERR_POLICY, "QueryCloudAZInit failed.");
        theLog->WriteLog(log_error, "policy arguments is incorrect in config.ini.");
        //return false;
    } else {
        theLog->WriteLog(log_info, "PolicyInit succeed.");
    }


    theSqlCollector =  theSqlCollector ? theSqlCollector : make_sql_collector(pExc->code!=ERR_POLICY);
    if(theSqlCollector)
        theLog->WriteLog(log_info, "make_sql_collector succeed.");
    if(get_host(&_hostname, &_host_ip)){
        theLog->WriteLog(log_info, "get hostname and ip succeed.(%s:%s)",_hostname.c_str(),_host_ip.c_str());
    }
    if(get_app(&_app_name, &_app_path)){
        theLog->WriteLog(log_info, "get app name and app path succeed.(%s %s)", _app_path.c_str(), _app_name.c_str());
    }

    return true;
}
*/

bool CEnforcerMgr::Init_SDK(const char* szWorkerName, SqlException * pExc){
    _worker_name = szWorkerName;

    try
    {
        //log init
        if (CLog::Instance()->InitLog(CommonFun::GetLogDirectoryPath() + "/" + _worker_name, "dae_sql_server"))
        {
            // Change log level
            try
            {
                auto log_level = std::to_string(static_cast<int>(EMDBConfig::GetInstance().get_log_level())); // TODO: Change into enum
                //update log level
                theLog->UpdateLogLevel(log_level);
            }
            catch (const std::exception& e)
            {
                theLog->WriteLog(log_error, "Failed to set log level. Use the default log level.");
            }

            //config file
            std::string sPath = CommonFun::GetConfigFilePath();
            theLog->WriteLog(log_info, "Read config from:%s", sPath.c_str());
        }

        const auto& emdb_config = EMDBConfig::GetInstance();
        auto initial_errors = emdb_config.get_initial_errors();

        for (const auto& error : initial_errors)
        {
            theLog->WriteLog(log_error, error.c_str());
        }
    }
    catch (const std::exception& e)
    {
        pExc->SetInfo(ERR_CONFIG, e.what());
        return false;
    }
    
    const auto& emdb_config = EMDBConfig::GetInstance();

#ifdef _WIN32
    DWORD id = ::GetCurrentProcessId();
    theLog->WriteLog(log_info, "SQLEnforcer::Init %d", id);
#else
    theLog->WriteLog(log_info, "SQLEnforcer::Init");
#endif

    EMDBConfig::WorkerConfig worker_config;
    if (emdb_config.get_tdsproxyworker_config(_worker_name, worker_config))
    {
      _odbc_connection_string = worker_config.odbc_connection_string_;
    }

    _db_server_name = QueryDbServerName();

    // Initialize TableMetadataCacheManager
    if (emdb_config.get_odbc_gettablestructure_switch()) {
      TableMetadataCacheManager::Instance().SetQueryTableColumnsFun(
          std::bind(&CEnforcerMgr::QueryTableColumns, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3));
      TableMetadataCacheManager::Instance().StartRefreshCache(emdb_config.get_policy_cc_sync_time());
    }

    // Initialize DefaultSchemaCacheManager
    DefaultSchemaCacheManager::Instance().SetQueryDefaultSchemaFun(
        std::bind(&CEnforcerMgr::QueryDefaultSchema, this, std::placeholders::_1, std::placeholders::_2));
    DefaultSchemaCacheManager::Instance().StartRefreshCache(emdb_config.get_policy_cc_sync_time());

    //load sqlparser
    if (!CSqlparserWrapper::Instance()->LoadParser())
    {
        pExc->SetInfo(ERR_LIBRARY, detail_err_lib_sql2003);
        theLog->WriteLog(LOG2CONSOLE|LOG2FILE, log_fatal, "Load sql parser module failed.");
        return false;
    }

    //load userattribute module
    if (!UserAttrWrapper::Instance()->LoadUserAttributeModule())
    {
        pExc->SetInfo(ERR_LIBRARY, detail_err_lib_userattribute);
        theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_fatal, "Load user attribute module failed.");
        return false;
    }
    UserAttrWrapper::Instance()->UserAttrInit_SDK(SQLEnforcerWriteLogA);//log format


    if (!QueryCloudAZSDKCppWrapper::Instance()->LoadQueryCloudAZSDKCpp())
    {
        theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_fatal, "Load query policy module failed.");
        pExc->SetInfo(ERR_LIBRARY, "Load query policy failed.");
        theLog->WriteLog( log_error, "Load QueryCloudAZSDKCpp dll failed");
        //return false;
    }
    else {
        if (QueryCloudAZSDKCppWrapper::Instance()->QueryCloudAZInit)
            theLog->WriteLog(LOG2CONSOLE, log_info, "Query PC module initializing...");
        if ( QueryCloudAZSDKCppWrapper::Instance()->QueryCloudAZInit(
                emdb_config.get_policy_jpchost().c_str(),
                emdb_config.get_policy_jpcport().c_str(),
                emdb_config.get_policy_cchost().c_str(),
                emdb_config.get_policy_ccport().c_str(),
                emdb_config.get_policy_jpcuser().c_str(),
                emdb_config.get_policy_jpcpwd().c_str(),
                8,
                [](int, const char*) { return 0; }))
        {
            theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_info, "Query PC module initialized successfully.");
        }
        else {
            pExc->SetInfo(ERR_POLICY, "QueryCloudAZInit failed.");
            _exc.SetInfo(ERR_POLICY, "QueryCloudAZInit failed.");
            theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_fatal, "Query PC module initialization failed.");
            theLog->WriteLog(LOG2CONSOLE, log_fatal, "Please check the configuration file config.ini, see log for details");
        }
    }
    
     
    
    theSqlCollector =  theSqlCollector ? theSqlCollector : make_sql_collector(pExc->code!=ERR_POLICY);
    if(theSqlCollector)
        theLog->WriteLog(log_info, "make_sql_collector succeed.");
    if(get_host(&_hostname, &_host_ip)){
        theLog->WriteLog(log_info, "get hostname and ip succeed.(%s:%s)",_hostname.c_str(),_host_ip.c_str());
    }
    if(get_app(&_app_name, &_app_path)){
        theLog->WriteLog(log_info, "get app name and app path succeed.(%s %s)", _app_path.c_str(), _app_name.c_str());
    }

    if (emdb_config.get_policy_jpc_cache_is_on()) {//clear cache
        std::thread thread(&CEnforcerMgr::ClearQueryPCResultCaches_SyncFunc, this);
        thread.detach();
        std::thread thread_const_interval(&CEnforcerMgr::ClearQueryPCResultCaches_SyncFunc2, this);
        thread_const_interval.detach();
    }

    ////load policy manager dll
    if (!CCPolicyMgrWrapper::Instance()->LoadPolicyMgr())
    {
        pExc->SetInfo(ERR_LIBRARY, "Load DAECCpolicyMgr.dll failed.");
        theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_fatal, "Load CC policy manager module failed.");
    }
    else {
        if (!emdb_config.get_policy_cc_switch_is_on()) {
            return true;
        }

        if (CCPolicyMgrWrapper::Instance()->PolicyInit)
            theLog->WriteLog(LOG2CONSOLE, log_info, "Policy manager module initializing...");

        S4HException s4hexc;

        EMDBConfig::WorkerConfig worker_config;
        std::string metadata_odbc_conn_string;

        if (emdb_config.get_tdsproxyworker_config(_worker_name, worker_config)) {
            metadata_odbc_conn_string = worker_config.odbc_connection_string_;
        }

        bool bRet = CCPolicyMgrWrapper::Instance()->PolicyInit(
            emdb_config.get_policy_cchost(),
            emdb_config.get_policy_ccport(),
            emdb_config.get_policy_ccuser(),
            emdb_config.get_policy_ccpwd(),
            emdb_config.get_policy_modelname(),
            emdb_config.get_cc_tag(),
            emdb_config.get_policy_cc_sync_time(),
            s4hexc,
            SQLEnforcerWriteLogA);
        if (!bRet) {
            pExc->SetInfo(ERR_POLICY, "cc init failed.");
            theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_fatal, "Policy manager module initialization failed.");
            theLog->WriteLog(LOG2CONSOLE, log_fatal, "Please check the configuration file config.ini, see log for details");
        }
        else {
            theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_info, "Policy manager module initialized successfully.");
        }

        CCPolicyMgrWrapper::Instance()->BindUpdateNotification( std::bind(&CEnforcerMgr::UpdatePCResultCachesStatus, this) );
    }

    return true;
}


USER_CONTEXT CEnforcerMgr::NewContext(const char *szUserName, const char *szPwd, SqlException * pExc)
{
    UserContextInfo *userCtxInfo = new UserContextInfo(szUserName, szPwd);
    userCtxInfo->InitUserAttr(pExc);
//    if(pExc->code == ERR_USERINFO){
//        _exc = *pExc;
//    }

    USER_CONTEXT userCtx = (USER_CONTEXT)userCtxInfo;

    return userCtx;
}

void CEnforcerMgr::FreeContext(USER_CONTEXT ctx) {
    UserContextInfo *userCtxInfo = GetUserContextInfo(ctx);
    delete (userCtxInfo);
}

USER_CONTEXT CEnforcerMgr::NewContext_SDK( SqlException * pExc){
    if(UserAttrWrapper::Instance()->GetUserAttr_SDK == NULL) {
        return 0;
    }
    std::string sName = "null";
    std::string sPWD = "null";
    std::string sep = EMDBConfig::GetInstance().get_usermode_multi_value_separator();
    std::string usr_mode = std::to_string(static_cast<unsigned>(EMDBConfig::GetInstance().get_usermode_mode())); // TODO: Use enum Usermode
    std::string user_info = EMDBConfig::GetInstance().get_usermode_user_info();
    UserContextInfo *userCtxInfo = new UserContextInfo(sName.c_str(), sPWD.c_str());
    userCtxInfo->m_pUserAttr = UserAttrWrapper::Instance()->GetUserAttr_SDK(sep.c_str(), usr_mode.c_str(), user_info.c_str());

    USER_CONTEXT userCtx = (USER_CONTEXT)userCtxInfo;
    return userCtx;
	
}

void CEnforcerMgr::AddUserAttrValue_SDK(USER_CONTEXT ctx, const char* key,const char* value, SqlException * pExc){
    if(!ctx){
        return;
    }
    UserContextInfo *pInfo  = (UserContextInfo*)ctx;
    if(pInfo->m_pUserAttr == nullptr && UserAttrWrapper::Instance()->GetUserAttr_SDK){
        std::string sep = EMDBConfig::GetInstance().get_usermode_multi_value_separator();
        std::string usr_mode = std::to_string(static_cast<unsigned>(EMDBConfig::GetInstance().get_usermode_mode())); // TODO: Use enum Usermode
        std::string user_info = EMDBConfig::GetInstance().get_usermode_user_info();

        pInfo->m_pUserAttr = UserAttrWrapper::Instance()->GetUserAttr_SDK(sep.c_str(),usr_mode.c_str(), user_info.c_str());
    }
    if(UserAttrWrapper::Instance()->AddUserAttrValue_SDK) {
        UserAttrWrapper::Instance()->AddUserAttrValue_SDK(pInfo->m_pUserAttr, key, value, parser_value_type(value, EMDBConfig::GetInstance().get_usermode_multi_value_separator()));
    }
}

void CEnforcerMgr::ClearUserContext_SDK(USER_CONTEXT ctx){
    if(!ctx){
        return;
    }
    UserContextInfo *pInfo  = (UserContextInfo*)ctx;
   
    UserAttrWrapper::Instance()->FreeUserAttr_SDK(pInfo->m_pUserAttr);

}

bool CEnforcerMgr::FreeUserContext_SDK(USER_CONTEXT ctx){
    if(ctx){
        UserContextInfo *pInfo  = (UserContextInfo*)ctx;
        if(pInfo->m_pUserAttr){
            UserAttrWrapper::Instance()->FreeUserAttr_SDK(pInfo->m_pUserAttr);
            pInfo->m_pUserAttr = nullptr;
        }
        delete pInfo;
        ctx = 0;
        return true;
    }
    
    return false;
}

void CEnforcerMgr::SetUserContextInfo(USER_CONTEXT ctx, USER_CONTEXT_INFO_TYPE infoType, const char *szInfo)
{
    UserContextInfo *userCtx = GetUserContextInfo(ctx);
    if (NULL != userCtx)
    {
        switch (infoType)
        {
        case CONTEXT_INFO_SERVER: {
            userCtx->SetDBServer(szInfo);
        }   break;
        case CONTEXT_INFO_DATABASE: {
            userCtx->SetCurrentDB(szInfo);
        }   break;

        case CONTEXT_INFO_SCHEMA: {
            userCtx->SetSchema(szInfo);
        }   break;
        case CONTEXT_INFO_DB_TYPE: {
            //EMDB_DB_TYPE dbType = EMDB_DB_UNKNOW;
            std::string strDriverName(szInfo);
            EMDB_DB_TYPE dbType = get_dbtype_by_drivername(strDriverName);
            userCtx->SetEMDBType(dbType);
        }   break;
        case CONTEXT_INFO_CLIENT_APP: {
            userCtx->SetClientAppName(szInfo);
        }   break;
        case CONTEXT_INFO_CLIENT_HOST_NAME: {
            userCtx->SetClientHostName(szInfo);
        }   break;
        case CONTEXT_INFO_CLIENT_IP: {
            userCtx->SetClientIp(szInfo);
        }   break;
        }
    }
}

UserContextInfo * CEnforcerMgr::GetUserContextInfo(USER_CONTEXT ctx)
{
    UserContextInfo *p_user = (UserContextInfo*)ctx;
    return (p_user && p_user->m_magicNumber == HANDLE_USERCONTEXTINFO) ?  p_user : nullptr;
}

std::wstring CEnforcerMgr::EvaluationSQL(USER_CONTEXT context, const wchar_t *sqltext, VecBindSqlParams& params, SqlException * pExc)
{
    std::string u8_cvt_str;
    CommonFun::ToUTF8(sqltext, u8_cvt_str);

    std::string strNewSQL = EvaluationSQL(context, u8_cvt_str.c_str(), params, pExc);

    std::wstring u16_cvt_str ;
    //convert result
    CommonFun::FromUTF8(strNewSQL, u16_cvt_str);
    return u16_cvt_str;
}

std::string CEnforcerMgr::EvaluationSQL(USER_CONTEXT context, const char *sqltext, VecBindSqlParams& params, SqlException * pExc)
{
    auto st = system_clock::now();
   // out_is_blocked = false;
    CSqlparserWrapper *pSqlparser = CSqlparserWrapper::Instance();

    EnforcerResultCode code = EnforcerResultCode::EF_USE_NEW_TEXT;
    std::string old_sql(sqltext);
    size_t sz_sql = old_sql.size();
    if(sz_sql > 8){
        std::string flag = old_sql.substr(sz_sql-8);
        if(CommonFun::StrCaseCmp(flag.c_str(),DAE_IGNORE_FLAG)==0){
            theLog->WriteLog(log_info, "Sql statement contains comment(%s), ignore this time.",DAE_IGNORE_FLAG);
            pExc->SetInfo(ERR_POLICY_NOMATCH, "Ignore this time");
            return old_sql;
        }
    }
    //theLog->WriteLog(log_info, "ori_sql:%s", sqltext);

    //get usercontext info
    UserContextInfo *pUserCtxInfo = GetUserContextInfo(context);
    if (!pUserCtxInfo)
    {
        EnforcerResultData data;
        data.bParseSucceed = false;
        data.retCode = code;
        data.oriSql = old_sql;
        data.newSql = "--User Context is null";
        data.username = "";
        theLog->WriteLog(log_error, "CEnforcerMgr::EvaluationSQL failed because we can't get user context info[%s] by context[%i]."
                                    "And current sql is \"%s\" ", pUserCtxInfo, context, sqltext);
        pExc->SetInfo(ERR_PARSE, detail_err_parse2);
        
        theSqlCollector->collect_enforcer_result(data);
        return sqltext;
    }

//    {
//        IUserAttr* puser = pUserCtxInfo->UserAttribute();
//        if(puser){
//            theLog->WriteLogFunc(log_info, std::bind(&IUserAttr::Print, puser));
//        }
//    }

     if (!pSqlparser->IsWell())
    {
        theLog->WriteLog(log_error, "CEnforcerMgr::EvaluationSQL failed because sql parser module didn't well");
        pExc->SetInfo(ERR_PARSE, detail_err_parse1);
         
        EnforcerResultData data;
        data.bParseSucceed = false;
        data.oriSql = old_sql;
        data.newSql = "--Paser module not well";
        data.retCode = code;
        data.username = pUserCtxInfo->GetUsername();
        theSqlCollector->collect_enforcer_result(data);

        return sqltext;
    }

    bool b = false;

    std::string new_sql = old_sql;//use to cllection log
    std::string strNewSql = sqltext;
    IParseResult *parserResult = pSqlparser->ParseSql(strNewSql, pUserCtxInfo->GetEMDBType());

    if (parserResult->IsAccept())
    {
        //parse context
        SQLParseCtx parseCtx;
        parseCtx.pUserCtxInfo = pUserCtxInfo;
        parseCtx.bError = false;
        parseCtx._sql_params = params;

        //get parse result
        INode *tree = parserResult->GetParseTree();

        //tree->Print();

        if (tree)
        {
            IPlan *plan = pSqlparser->CreatePlan(std::bind(&CEnforcerMgr::StmtVisit, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&CEnforcerMgr::ColumnVisit, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&CEnforcerMgr::StartNewStmt, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&CEnforcerMgr::WhereCluase, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&CEnforcerMgr::ErrorOccur, this, std::placeholders::_1),
                                                 &parseCtx,
                                                 parserResult);

            plan->SetDefaultDatabase(pUserCtxInfo->GetCurrentDB().c_str());
            plan->SetDefaultSchema(pUserCtxInfo->GetSchema().c_str());

            pSqlparser->VisitPlan(plan);
            strNewSql = tree->Serialize();

            //tree->Print();
   //         theLog->WriteLog(log_info, "new_sql:%s", strNewSql.c_str());
            
            /* record user's protected sql script */
            {
                
                b = pSqlparser->BeautifySql(strNewSql, new_sql, pUserCtxInfo->GetEMDBType());
               // theLog->WriteLog(log_info, "beautiful_new_sql:%s", new_sql.c_str());
                if (b) {
                    //theLog->WriteLog(log_info, "new sql:\n%s", out_sql.c_str() );
                #ifdef _DEBUG
                    //printf("new sql:\n%s\n", new_sql.c_str());
                #endif
                }  
                else {
                    //theLog->WriteLog(log_info, "new sql:\n%s", strNewSql.c_str() );
                #ifdef _DEBUG
                    printf("New SQL Parser failed");
                #endif
                }
            }

            // 2022.04.02 reture default database after "USE"
            pUserCtxInfo->SetCurrentDB(plan->GetDefaultDatabase().c_str());
            pSqlparser->DestroyPlan(plan);
            plan = NULL;
            *pExc = parseCtx._exc;
            //-----exception summary----
            if(parseCtx._deny_times > 0){ // update/insert/delete  deny
                code = EnforcerResultCode::EF_BLOCK_THIS_TEXT;
                pExc->SetInfo(ERR_DENY, detail_err_evaluation);
                new_sql += "--DAE Deny";
            } else if (parseCtx._filter_times > 0 ){ //filter + deny select(where 1=0)
                if(parseCtx._filter_deny_times > 0){
                    if(pExc->code != ERR_DENY_SELECT) {
                        pExc->SetInfo(ERR_DENY_SELECT, ""); //ERR_DENY_SELECT Priority than E_ALLOW
                    }
                } else {
                    pExc->SetInfo(ERR_NULL, "");
                }
            } else if(parseCtx._mask_times > 0) {//masked 
                pExc->SetInfo(ERR_NULL, "");
            } else {
                pExc->SetInfo(ERR_POLICY_NOMATCH, "");
                strNewSql = sqltext;// reset sql
            }

            //collect report 
            if (pExc->code != ERR_POLICY_NOMATCH &&
                EMDBConfig::GetInstance().get_log_switch_report() &&
                parseCtx._vec_report.size() > 0) 
            {
                theSqlCollector->collect_report_result(parseCtx._vec_report, sqltext, strNewSql);
            }
            

        }
        
    }
#ifdef WINDOWS_APP_DEMO
    #include "sqlenforcer_create.h"
    using namespace DAE;
    CrtDrpTableInfo tbinfo;
     if(is_create_or_drop_stmt(old_sql, tbinfo)){// todo test for windows excel
        bool bdeny = deny_create_or_drop(tbinfo, pUserCtxInfo, pExc);
        if(bdeny){
            code = EnforcerResultCode::EF_BLOCK_THIS_TEXT;
            pExc->SetInfo(ERR_DENY, detail_err_evaluation);
        }else{
            pExc->SetInfo(ERR_NULL, "");
        }
    }
#endif
    else
    {
        char err[1024];
        sprintf(err, "sql script syntax error at(%d,%d): %s",(int)parserResult->GetErrorLine(), (int)parserResult->GetErrorColumn(), parserResult->GetErrorDetail().c_str());
        theLog->WriteLog(log_info, "sql script syntax error at(%d,%d): %s\n", 
            (int)parserResult->GetErrorLine(), (int)parserResult->GetErrorColumn(), parserResult->GetErrorDetail().c_str());

    //#ifdef _DEBUG
    //        printf("sql script syntax error at(%d,%d): %s\n", 
    //        (int)parserResult->GetErrorLine(), (int)parserResult->GetErrorColumn(), parserResult->GetErrorDetail().c_str());
    //#endif
        pExc->SetInfo(ERR_PARSE, err);
    }

    pSqlparser->DestroyParseResult(parserResult);
    parserResult = NULL;
    {
        EnforcerResultData data;
        data.bParseSucceed = b;
        data.oriSql = old_sql;
        data.newSql = pExc->code == ERR_PARSE?"--Unparsed SQL":new_sql;
        data.retCode = code;
        data.username = pUserCtxInfo->GetUsername();
        theSqlCollector->collect_enforcer_result(data);
        
    }
    auto ed = system_clock::now();
    auto duration = duration_cast<microseconds>(ed-st);
    double  timed = double (duration.count())*microseconds::period::num / microseconds::period::den;
    theLog->WriteLog(log_trace,"---------time:%lf s--------- \n",timed);
    return strNewSql;
}

bool check_policy_and_user_deny(SqlException & exc, SQLParseCtx* pParseCtx){
    if(exc.IsDeny(false)){
        return true;
    }
    bool except_deny = true;
    auto exception_handler = EMDBConfig::GetInstance().get_dae_error_exception_handler();

    if(exception_handler == HandlerType::kAllow){
        except_deny = false;
    }

    if(except_deny){
        if(exc.code != ERR_POLICY) {
            UserContextInfo* pUserCtx = pParseCtx->pUserCtxInfo;
            IUserAttr * puser = pUserCtx->UserAttribute();
            if(puser){
                if(puser->GetAttributeCount() == 0){
                    theLog->WriteLog(log_info, "deny all SQL, becase user attr is empty, and DAE_ERROR.exception_handler is deny.");
                    return true;
                }
            }
        } else {
            theLog->WriteLog(log_info, "deny all SQL, becase Policy init failed, and DAE_ERROR.exception_handler is deny.");
            return true;
        }

    }

    return false;
}

void CEnforcerMgr::SetMetadataFunc(USER_CONTEXT ctx, QueryMetadataFunc func){
    UserContextInfo* pinfo = GetUserContextInfo(ctx);
    if(pinfo && func){
        pinfo->_meta_func = func;
    }
}

void alter_resource(IUseStmt *stmt,UserContextInfo * puser_ctx){
    if (stmt == NULL) {
        theLog->WriteLog(log_info, "alter_resource new shceme failed stmt is NULL" );
        return;
    }
    EMDB_DB_TYPE tp = puser_ctx->GetEMDBType();
    if(tp == EMDB_DB_HANA || tp == EMDB_DB_ORACLE){
        puser_ctx->SetSchema(stmt->GetSchema());
        theLog->WriteLog(log_info, "alter_resource new shceme:%s",stmt->GetSchema());
    } else if(tp == EMDB_DB_SQLSERVER){
        puser_ctx->SetCurrentDB(stmt->GetDbname());
        theLog->WriteLog(log_info, "alter_resource new database:%s",stmt->GetDbname());
    }
}

CEAttributeType parser_value_type(const std::string& src, const std::string& val_separator) {
    size_t index = src.find(val_separator);
    if (!val_separator.empty() && index != std::string::npos) {
        if (CommonFun::IsNum(src.substr(0, index))) {
            return XACML_ArrInt;
        }
        else {
            return XACML_ArrStr;
        }

    }
    else if (CommonFun::IsNum(src)) {
        if (src.find('.') != std::string::npos) {
            return XACML_Double;
        }
        else {
            return XACML_int;
        }
    }
    else if (CommonFun::StrCaseCmp(src.c_str(), "TRUE") == 0
        || CommonFun::StrCaseCmp(src.c_str(), "FALSE") == 0)
    {
        return XACML_bool;
    }
    else {
        return XACML_string;
    }
}


void set_user_attr_bySQL(const IStmt::SQL_USER_VALUE& val, IUserAttr* puser) {
    if (val._val.empty()) {
        return;
    }
    if (!puser) {
        return;
    }
    std::string src = val._val;
    const std::string& r = EMDBConfig::GetInstance().get_sqluser_format_r();
    if (r.length() <= 0 || src.length() <= 0) {
        return;
    }
    //if (src.length() >=2 && src[0] == '\'' && src[src.length() - 1] == '\'') {
    //    src = s.substr(1, src.length() - 2);
    //    if (src.length() == 0) {
    //        return;
    //    }
    //}
    EMStrVector vec;
    CommonFun::SplitString(src, ";", vec);
    if (vec.size() >= 1) {
        for (auto& attr : vec) {
            size_t lf = attr.find("=");
            if (lf != std::string::npos) {
                std::string key = attr.substr(0, lf);
                std::string val = attr.substr(lf + 1);
                CEAttributeType type = XACML_string;
                if (key.length() > 2) {
                    std::string stype = key.substr(0, 2);
                    if (stype == "s_") {
                        key = key.substr(2);//
                        type = XACML_string;
                    }
                    else if (stype == "n_") {
                        key = key.substr(2);//
                        type = XACML_int;
                    }
                }
                UserAttrWrapper::Instance()->AddUserAttrValue_SDK(puser, key.c_str(), val.c_str(), type);
                //
            }
            else {
                std::string key = r;
                CEAttributeType type = XACML_string;
                if (key.length() >= 2) {
                    if (key[0] == '\'' && key[key.length() - 1] == '\'') {
                        key = key.substr(1, key.length() - 2);
                        if (key.length() == 0) {
                            if ((USER_SOURCE_SQL_USER & EMDBConfig::GetInstance().get_user_source()) != 0) {
                                key = "id";
                            }
                            else {
                                continue;
                            }
                        }
                        type = XACML_string;
                    }
                    if (key.length() > 2) {
                        std::string stype = key.substr(0, 2);
                        if (stype == "s_") {
                            key = key.substr(2);//
                            type = XACML_string;
                        }
                        else if (stype == "n_") {
                            key = key.substr(2);//
                            type = XACML_int;
                        }
                    }
                }
                else {
                    if ((USER_SOURCE_SQL_USER & EMDBConfig::GetInstance().get_user_source()) != 0) {
                        key = "id";
                    }
                    else {
                        continue;
                    }
                }
                if (val._type == IStmt::SQL_USER_VALUE::ValueType::TYPE_NUMBER) {
                    type = XACML_int;
                }
                //
                UserAttrWrapper::Instance()->AddUserAttrValue_SDK(puser, key.c_str(), src.c_str(), type);
                //
                break;
            }
        }
    }
}

void modify_user_attr_from_sessioncontext(SQLParseCtx* parser_ctx, IStmt* stmt) {
    /*
    unsigned int sctx_param_id = stmt->GetSessionContextParamId();
    if (sctx_param_id == 0) {
        std::string param_val = stmt->GetSessionContextValue();
        if (!param_val.empty()) {
            UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
            IUserAttr* user = puser_ctx->UserAttribute();
            UserAttrWrapper::Instance()->AddUserAttrValue_SDK(user, "id", param_val.c_str(), XACML_string);
        }
    }
    else {
        if (parser_ctx->_sql_params.size() >= sctx_param_id) {
            UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
            IUserAttr* user = puser_ctx->UserAttribute();
            const std::string val = parser_ctx->_sql_params[sctx_param_id - 1]._value;
            UserAttrWrapper::Instance()->AddUserAttrValue_SDK(user, "id", val.c_str(), XACML_string);
        }
    }
    */
    const IStmt::SQL_USER_VALUE& val = stmt->GetSqlUserValue();
    IStmt::SQL_USER_VALUE val2 = val;

    std::string value;
    if (val2._type == IStmt::SQL_USER_VALUE::ValueType::TYPE_PARAM_ID) {
        unsigned int param_id = atoi(val2._val.c_str());
        if (parser_ctx->_sql_params.size() >= param_id) {
            val2._type = IStmt::SQL_USER_VALUE::ValueType::TYPE_STRING;
            val2._val = parser_ctx->_sql_params[param_id - 1]._value;
        }
        else {

        }
    }

    if (!val2._val.empty()) {
        UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
        IUserAttr* user = puser_ctx->UserAttribute();
        theLog->WriteLog(log_info, " user attr:%s In the SQL", value.c_str());
        set_user_attr_bySQL(val2, user);
    }
}

bool prefilter_stmttables_and_ignore(const SQLParseCtx* parser_ctx, IStmt* stmt, std::vector<ITableItem*> & tbis) { //prefilter
    UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
    StmtType stmt_type = stmt->GetStmtType();

    //std::vector<ITableItem*>  tbis;
    get_action_tables(tbis, stmt);
    if (stmt_type == StmtType::E_STMT_TYPE_UPDATE && stmt->GetFromItemCount() != 0) {
        get_from_tables(tbis, stmt);
    }
    //
    std::string action = CEnforcerMgr::GetPolicyActionByStmtType(stmt_type);

    if (ACTION_CREATE != action &&
        ACTION_DROP != action) {
        S4HException s4hexc;
        for (auto it = tbis.begin(); it != tbis.end(); ) {
            auto& tbi = *it;
            std::string tbname = tbi->GetTableName();
            if (puser_ctx->GetEMDBType() == EMDB_DB_ORACLE && CommonFun::StrCaseCmp("DUAL", tbname.c_str()) == 0) {
                it = tbis.erase(it);
            }
            else if (EMDBConfig::GetInstance().get_policy_cc_switch_is_on() &&
                !CCPolicyMgrWrapper::Instance()->IsNeedEnforcer("", "", tbname, TablePolicyInfo::GetEnumActionFromString(action), s4hexc))
            {
                it = tbis.erase(it);
                theLog->WriteLog(log_info, "remove table: %s(because cc does not include this table)", tbname.c_str());
            }
            else {
                ++it;
            }
        }
    }

    if (tbis.empty())
        return true;
    else {
        return false;
    }

}

void prefilter_tables(const std::vector<ITableItem*> & enforcer_tbis,  std::vector<ITableItem*>& out) {
    
    for (auto ito = out.begin(); ito != out.end();) {
        bool bfind = false;
        for (auto& ite : enforcer_tbis) {
            if (CommonFun::CaseInsensitiveEquals(ite->GetTableName(), (*ito)->GetTableName())) {
                bfind = true;
            }
        }
        if (!bfind)
            ito = out.erase(ito);
        else
            ito++;
    }
}

void CEnforcerMgr::StmtVisit(IPlan *plan, IStmt *stmt)
{
    assert(stmt);
    //get parse context
    SQLParseCtx *parser_ctx = (SQLParseCtx *)(plan->GetContext());
    std::vector<ITableItem*> enforcer_tbis;
    bool bignore = prefilter_stmttables_and_ignore(parser_ctx, stmt, enforcer_tbis);
    if (bignore) {
        return;
    }

    if(stmt->GetStmtType() == E_STMT_TYPE_USE){
        UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
        alter_resource(dynamic_cast<IUseStmt *>(stmt), puser_ctx);
        return;
    }

    //std::string sql_old = parser_ctx->_node->Serialize();
    //UserContextInfo *pUserCtx = pParseCtx->pUserCtxInfo;

    StmtType stmt_type = stmt->GetStmtType();
    std::string action = GetPolicyActionByStmtType(stmt_type);

    //std::vector<QueryPcResult*> results;
    SharedPtrVector<QueryPcResult> results;
    

    SqlException exc = _exc;//policy init failed.
    UserContextInfo* puser_ctx = parser_ctx->pUserCtxInfo;
    bool deny = check_policy_and_user_deny(exc, parser_ctx);

    modify_user_attr_from_sessioncontext(parser_ctx, stmt);

    if(!deny){
        std::vector<ITableItem*>  tbis;
        get_action_tables(tbis, stmt);
        prefilter_tables(enforcer_tbis, tbis);
        query_pc_from_stmt(results, action, tbis, parser_ctx, &exc);
        tbis.clear();
        if( stmt_type == StmtType::E_STMT_TYPE_UPDATE && stmt->GetFromItemCount() != 0) {
            get_from_tables(tbis, stmt);
            prefilter_tables(enforcer_tbis, tbis);
            std::string action = GetPolicyActionByStmtType(E_STMT_TYPE_SELECT);
            query_pc_from_stmt(results, action, tbis, parser_ctx, &exc);
            
        }
    }
    deny = check_policy_and_user_deny(exc, parser_ctx);//query policy exception need deny

    std::string strCond ;
    if(!deny) {
        for (auto & itrs:results){
            if(!itrs) continue;

            auto table_id = itrs->GetTableId();
            // Reset join predicate when the table, which should be enforced, is in joined tables.
            if (auto join_table = stmt->GetJoinedTable(table_id))
            {
                std::string cond = dynamic_cast<EnforcerPcResult*>(itrs.get())->_filter_cond;

                if (itrs->IsDeny()) {
                  cond = "1 = 0";
                }

                if (join_table->ResetJoinPredicate(table_id, cond)) {
                  continue;
                }
            }

            if(itrs->IsDeny()){
                exc.SetInfo(ERR_DENY_SELECT,detail_err_evaluation);
                deny = true;
                break;
            } else {
                std::string cond = dynamic_cast<EnforcerPcResult*>(itrs.get())->_filter_cond;
                if(cond.empty()){
                    continue;
                }
                if(strCond.empty()){
                    strCond += "(";
                    strCond += cond;
                    strCond += ")";
                } else {
                    strCond += " AND (";
                    strCond += cond;
                    strCond += ")";
                }
            }
        }
    }

    switch (stmt_type)
    {
        case E_STMT_TYPE_SELECT:{
            if(!deny){
                for (auto & itrs:results){
                    if(!itrs) continue;
                    EnforcerPcResult * pitrs = dynamic_cast<EnforcerPcResult*>(itrs.get());
                    if(dynamic_cast<ISelectStmt*>(stmt)->GetSelectExpr() && pitrs->_mask_map.size()>0){
                        //set metadata to ITableItem
                        std::vector<ITableItem*>  tbis;
                        get_action_tables(tbis, stmt);

                        if (EMDBConfig::GetInstance().get_odbc_gettablestructure_switch()) {
                          for(auto & ittb:tbis){
                            if(E_BASIC_TABLE ==  ittb->GetTableItemType() || E_BASIC_TABLE_WITH_ALIAS == ittb->GetTableItemType()){
                              const std::string db = ittb->GetDatabaseName().empty() ? puser_ctx->GetCurrentDB() : ittb->GetDatabaseName();
                              const std::string sc = ittb->GetSchemaName().empty()?puser_ctx->GetSchema():ittb->GetSchemaName();
                              MetadataVec pmetadata;
                              puser_ctx->GetTableMetadata(db, sc, ittb->GetTableName(), pmetadata);
                              if(pmetadata.size() > 0)
                                ittb->SetMetadata(pmetadata);
                            }
                          }
                        }

                        //mask opr
                        dynamic_cast<ISelectStmt*>(stmt)->GetSelectExpr()->MaskOpr(&pitrs->_mask_map,exc);
                        if(exc.code == ERR_MASK_OPR){
                            theLog->WriteLog(log_warning, "mask failed, %s",exc.cdetail.c_str());
                        }
                        if(exc.code == ERR_MASK_OPR  && EMDBConfig::GetInstance().get_dae_error_star_handler() == HandlerType::kDeny){
                            deny = true;
                            break;
                        }
                        parser_ctx->_mask_times++;
                    }
                }
            }
            if(deny){
                strCond = " 1 = 0 ";
                parser_ctx->_filter_deny_times++;
            }
            if(!strCond.empty() && dynamic_cast<ISelectStmt*>(stmt)->GetWhereClause()){
                dynamic_cast<ISelectStmt*>(stmt)->GetWhereClause()->AddCondition(strCond, exc);
                parser_ctx->_filter_times++;
                parser_ctx->_sql_enforcer = true;
            }
        }break;
        case E_STMT_TYPE_CREATE_TABLE: {
            if(deny){
                parser_ctx->_sql_enforcer = true;
                parser_ctx->_filter_times++;
                std::vector<ITableItem*> tbis;
                get_action_tables(tbis, stmt);
                stmt->DenyTables(tbis);
                break;
            }
        }break;
        case E_STMT_TYPE_DROP_TABLE: {
            if(deny){
                parser_ctx->_sql_enforcer = true;
                parser_ctx->_filter_times++;
                std::vector<ITableItem*> tbis;
                get_action_tables(tbis, stmt);
                stmt->DenyTables(tbis);
                break;
            }
        }break;
        case E_STMT_TYPE_UPDATE:{
            if(deny){
                //parser_ctx->_deny_times++;
                parser_ctx->_sql_enforcer = true;
                parser_ctx->_filter_times++;
                std::vector<ITableItem*>  tbis;
                get_action_tables(tbis, stmt);
                stmt->DenyTables(tbis);
                break;
            }
            if(!strCond.empty() && dynamic_cast<IUpdateStmt*>(stmt)->GetWhereClause()){
                dynamic_cast<IUpdateStmt*>(stmt)->GetWhereClause()->AddCondition(strCond, exc);
                parser_ctx->_filter_times++;
                parser_ctx->_sql_enforcer = true;
            }
        }break;
        case E_STMT_TYPE_DELETE:{
            if(deny){
                //parser_ctx->_deny_times++;
                parser_ctx->_sql_enforcer = true;
                parser_ctx->_filter_times++;
                std::vector<ITableItem*>  tbis;
                get_action_tables(tbis, stmt);
                stmt->DenyTables(tbis);
                break;
            }
            if(!strCond.empty() && dynamic_cast<IDeleteStmt*>(stmt)->GetWhereClause()){
                dynamic_cast<IDeleteStmt*>(stmt)->GetWhereClause()->AddCondition(strCond, exc);
                parser_ctx->_filter_times++;
                parser_ctx->_sql_enforcer = true;
            }
        }   break;

        case E_STMT_TYPE_INSERT:{
            if(deny){
                //parser_ctx->_deny_times++;
                parser_ctx->_sql_enforcer = true;
                parser_ctx->_filter_times++;
                std::vector<ITableItem*>  tbis;
                get_action_tables(tbis, stmt);
              stmt->DenyTables(tbis);
                break;
            }
        }break;
        default:
            break;
    }

    if(exc.code != ERR_NULL){
        parser_ctx->_exc = exc;
    }
    results.clear();

    // deny update while select table masked
    if(stmt_type == E_STMT_TYPE_UPDATE) {
        do{
            std::vector<ITableItem*>  tbis; // this is stmt member ref
            get_action_tables(tbis, stmt);
            assert(tbis.size() == 1) ;  //  update table size need == 1
            std::string table = tbis[0]->GetTableName();
            
            std::string action_ = GetPolicyActionByStmtType(E_STMT_TYPE_SELECT);
            query_pc_from_stmt(results, action_, tbis, parser_ctx, &exc);
            IUpdateStmt * stmt_update = dynamic_cast<IUpdateStmt*>(stmt);
            const std::vector<IUpdateItem*>& upits = stmt_update->get_update_items();
            MaskItemMap * mask_map = NULL;
            for (auto & itrs:results){
                auto it = dynamic_cast<EnforcerPcResult*>(itrs.get())->_mask_map.find(table);
                if(it!=  dynamic_cast<EnforcerPcResult*>(itrs.get())->_mask_map.end()){
                    mask_map = it->second;//
                }
            }
            if(mask_map == NULL || mask_map->size() == 0){
                break;
            }

            UserContextInfo* pUserCtx = parser_ctx->pUserCtxInfo;
            ITableItem * tbi = stmt_update->GetUpdateTable();
            std::string qualify_ = tbi->GetTableAliasName();
            if (qualify_.empty())
            {
                qualify_ = tbi->GetTableObject();
            }
            qualify_ = get_field_qualify( pUserCtx->GetEMDBType(), qualify_);

            for(auto it : upits){
                auto itf = mask_map->find(it->GetFieldName());
                if(itf != mask_map->end()) {
                    it->DenyUpdate(itf->second->_condition, qualify_, itf->second->_datatype, pUserCtx->GetEMDBType());
                    parser_ctx->_sql_enforcer = true;
                    parser_ctx->_filter_times++;
                }
            }
            results.clear();
        } while(false);
        //free_pc_result(results);
        
        
    }
    
}

std::string get_datatype_name(const EMDataType & datatype)
{
  std::string ret;

  switch (datatype)
  {
    case EMDATA_STRING:
    {
      ret = "STRING";
    } break;
    case EMDATA_NUMBER:
    {
      ret = "NUMBER";
    } break;
    case EMDATA_DATE:
    {
      ret = "DATE";
    } break;
    case EMDATA_TIME:
    {
      ret = "TIME";
    } break;
    case EMDATA_TIMESTAMP:
    {
      ret = "TIMESTAMP";
    } break;
    case EMDATA_STRING2:
    {
      ret = "STRING2";
    } break;
    case EMDATA_TIMESTAMP_TZ_ORA:
    {
      ret = "TIMESTAMP_TZ_ORA";
    } break;
    case EMDATA_YMINTERVAL:
    {
      ret = "YMINTERVAL";
    } break;
    case EMDATA_DSINTERVAL:
    {
      ret = "DSINTERVAL";
    } break;
    case EMDATA_NONSUPPORT:
    {
      ret = "NONSUPPORT";
    } break;
    default:
    {
      // do nothing
    } break;
  }

  return ret;
}

std::string print_mask(const MaskConditionMap & map_){
    std::string ret = " ";
    for(auto ittb:map_ ){
        ret += "{ \n";
        for(auto it:*ittb.second){
            ret += ittb.first;
            ret += '.';
            ret += it.first;
            ret += ":[ ";
            ret += "\n    COND =";
            ret += it.second->_condition;
            ret += "\n    TYPE =";
            ret += get_datatype_name(it.second->_datatype);
            ret += "\n    FORMAT =";
            if(it.second->_format==0) ret+= "FullMask";
            else if(it.second->_format==1) ret+= "RandNumber";
            else if(it.second->_format==2) ret+= "PartialMask";
            else if(it.second->_format==3) ret+= "KeyMask";
            ret += "\n    SYMBOL =";
            ret += it.second->_symbols;
            ret += "  ],\n";
        }
        ret += "},\n";
    }
    return ret;
}

void get_from_tables(std::vector<ITableItem*> & tbis, IStmt* stmt){
    size_t cnt = stmt->GetTableItemCount();
    //std::vector<ITableItem*> tbis;
    for (size_t i = 0; i < cnt; ++i)
    {
        ITableItem *tbi = stmt->GetTableItem(i);
        if(!tbi)
            continue;
        if (tbi->GetTableItemType() != E_BASIC_TABLE && tbi->GetTableItemType() != E_BASIC_TABLE_WITH_ALIAS)
            continue;
        tbis.push_back(tbi);
    }
}

void get_action_tables(std::vector<ITableItem*> & tbis, IStmt* stmt){
    StmtType stmt_type = stmt->GetStmtType();
    switch(stmt_type){
        case E_STMT_TYPE_UPDATE:
        {
            ITableItem* tbi = dynamic_cast<IUpdateStmt*>(stmt)->GetUpdateTable();
            if(!tbi)
                return;
            if (tbi->GetTableItemType() != E_BASIC_TABLE && tbi->GetTableItemType() != E_BASIC_TABLE_WITH_ALIAS)
                return;

            tbis.push_back(tbi); 
        } break;
        case E_STMT_TYPE_DELETE:
        {
            ITableItem* tbi = dynamic_cast<IDeleteStmt*>(stmt)->GetDeleteTable();
            if(!tbi)
                return;
            if (tbi->GetTableItemType() != E_BASIC_TABLE && tbi->GetTableItemType() != E_BASIC_TABLE_WITH_ALIAS)
                return;
            tbis.push_back(tbi);
        } break;
        case E_STMT_TYPE_INSERT:
        {
            ITableItem* tbi = dynamic_cast<IInsertStmt*>(stmt)->GetInsertTable();
            if(!tbi)
                return;
            if (tbi->GetTableItemType() != E_BASIC_TABLE && tbi->GetTableItemType() != E_BASIC_TABLE_WITH_ALIAS)
                return;
            tbis.push_back(tbi);
        } break;
        default:{
            get_from_tables( tbis,stmt);
        } break;
    }
}


void query_pc_from_stmt(SharedPtrVector<QueryPcResult> & vecresult, const std::string & action, std::vector<ITableItem*> & tbis, SQLParseCtx* parse_ctx, SqlException * pExc){

    //chaeck is need enforcer
  //  S4HException s4hexc;
  //  for(auto it = tbis.begin(); it != tbis.end(); ){
  //      auto & tbi = *it;
  //      std::string tbname = tbi->GetTableName();
  //      if( user_ctx->GetEMDBType() == EMDB_DB_ORACLE && CommonFun::StrCaseCmp("DUAL", tbname.c_str()) == 0){
  //          it = tbis.erase(it);
		//} else if(EMDBConfig::GetInstance().get_policy_cc_switch_is_on() && 
  //          !CCPolicyMgrWrapper::Instance()->IsNeedEnforcer("", "", tbname, TablePolicyInfo::GetEnumActionFromString(action), s4hexc)) {
  //          it = tbis.erase(it);
  //          theLog->WriteLog(log_info, "remove table: %s(because cc does not include this table)", tbname.c_str());
  //      } else {
		//	++it;
		//}
  //  }
    //SharedPtrVector<QueryPcResult> vecresult;

    UserContextInfo* user_ctx = parse_ctx->pUserCtxInfo;
    if(!EMDBConfig::GetInstance().get_policy_jpc_switch_is_on() && tbis.size() > 0){
        //EnforcerPcResult* ret = new EnforcerPcResult();
        std::shared_ptr<EnforcerPcResult> ret = std::make_shared<EnforcerPcResult>();
        //EnforcerPcResult* ret = (EnforcerPcResult*)sptr.get();
        ret->SetTableId(tbis[0]->GetTableID());
        ret->SetDeny(false);
        ret->_filter_cond = "('QUERY_JPC_SWITCH'<>'OFF')";
        vecresult.push_back(ret);
        return;
    }



    if ( EMDBConfig::GetInstance().get_policy_jpc_cache_is_on() && tbis.size() > 0){//get cache
        //todo alter result tableid
        S4HException s4hexc;
        for(auto it = tbis.begin(); it != tbis.end(); ){
            auto & tbi = *it;
            std::string tbname = tbi->GetTableName();


            //create key;
            std::shared_ptr<RequestInfo> reqinfo2 = std::make_shared<RequestInfo>();
            RequestInfo reqinfo;
            reqinfo2->_action = action;
            //set schema,table name
            CPolicyResource resources;
            ComposePolicyResource(resources, user_ctx, tbi);
            resources.CopyDictionary2(reqinfo2->_rescs2);
            reqinfo2->_client_app_name = user_ctx->GetClientAppName();
            reqinfo2->_client_host_name = user_ctx->GetClientHostName();
            reqinfo2->_client_ip = user_ctx->GetClientIp();
            IUserAttr* attr = user_ctx->UserAttribute();
            attr->CopyDictionary2(reqinfo2->_users2);
            std::string key = reqinfo2->GenerateCacheKey();//todo

            std::shared_ptr<QueryPcResult> sval;
            if( CEnforcerMgr::Instance()->GetQueryPCResultCaches(key, sval)) {
                parse_ctx->_vec_report.push_back(reqinfo2);
                vecresult.push_back(sval);
                
                sval->SetTableId(tbi->get_table_id());

                //modify condition prefix   //todo  $user.attr  and  $table
                std::shared_ptr<EnforcerPcResult> pc = std::dynamic_pointer_cast<EnforcerPcResult>(sval);
                if (pc->IsDeny()) {
                    it = tbis.erase(it);
                    continue;
                }

                std::string qualify_ = tbi->GetTableAliasName();
                if (qualify_.empty())
                {
                    qualify_ = tbi->GetTableObject();
                }
                ConditionInfo condinfo(
                    tbi->get_table_id(),
                    tbi->GetTableName(),
                    qualify_,
                    action,
                    user_ctx->GetEMDBType(),
                    resources,
                    attr,
                    tbi->GetUsingColumns(),
                    tbi->GetJoinType(),
                    key
                );
                std::set<std::string> fields;
                if (!pc->_filter_cond.empty()) {
                    pc->_filter_cond= parser_condition_v2(pc->_filter_cond, condinfo, fields);
                    fields.clear();
                }
                for (auto& maskit : pc->_mask_map) {
                    MaskItemMap* maskitmap = maskit.second;
                    assert(maskitmap);
                    for (auto & it : (*maskitmap)) {
                        MaskItem* pitem = it.second;
                        assert(pitem);
                        if (!pitem->_condition.empty()) {
                            pitem->_condition = parser_condition_v2(pitem->_condition, condinfo, fields);
                            fields.clear();
                        }
                    }
                }
                it = tbis.erase(it);
                
            } else {
                ++it;
            }
        }
    }


    size_t cnt_real = tbis.size();
    if (cnt_real <= 0 || cnt_real > 10) return; /*todo  quantity limit*/
    //IPolicyRequest * reqs[MAX_TABLE] = {NULL};
    const IPolicyRequest ** reqs = (const IPolicyRequest**)calloc(cnt_real,sizeof(IPolicyRequest*));
    std::vector<ConditionInfo> conds;
    std::vector<std::shared_ptr<RequestInfo>> log_reqs2;

    std::string  user = user_ctx->GetUsername();
    for (size_t i = 0; i < cnt_real; ++i)
    {
        ITableItem *tbi = tbis[i];

        std::shared_ptr<RequestInfo> reqinfo2 = std::make_shared<RequestInfo>();
        reqinfo2->_action = action;
        //set schema,table name
        CPolicyResource resources;
        ComposePolicyResource(resources, user_ctx, tbi);
        resources.CopyDictionary2(reqinfo2->_rescs2);
        reqinfo2->_client_app_name = user_ctx->GetClientAppName();
        reqinfo2->_client_host_name = user_ctx->GetClientHostName();
        reqinfo2->_client_ip = user_ctx->GetClientIp();

        IUserAttr * attr = user_ctx->UserAttribute();
        attr->CopyDictionary2(reqinfo2->_users2);
        if(user.empty() || user.compare("null") == 0){
            auto it = reqinfo2->_users2.find("id");
            if(it != reqinfo2->_users2.end()){
                user = it->second.get()->strValue;
            } else {
                user = "user";
            }
        }

        reqs[i] = create_request((*reqinfo2.get()), user);

        log_reqs2.push_back(reqinfo2);

        std::string qualify_ = tbi->GetTableAliasName();
        if (qualify_.empty())
        {
            qualify_ = tbi->GetTableObject();
        }
        conds.push_back(
                ConditionInfo(
                    tbi->get_table_id(),
                    tbi->GetTableName(),
                    qualify_,
                    action,
                    user_ctx->GetEMDBType(),
                    resources,
                    user_ctx->UserAttribute(),
                    tbi->GetUsingColumns(),
                    tbi->GetJoinType(),
                    reqinfo2->GenerateCacheKey()
                )
        );
    }


    if(cnt_real == 1) {
        IPolicyResult* result = NULL;

        QueryStatus  status = single_query_pc(reqs[0], &result);
        if(TEST_LOG){
            size_t ob = 0;
            if(result){
                ob = result->ObligationCount();
            }
            theLog->WriteLog(log_info, "query result(status=%d,obligation=%d)",status,ob);
        }
/*
        auto st = system_clock::now();
        auto ed = system_clock::now();
        auto duration = duration_cast<microseconds>(ed-st);
        double  timed = double (duration.count())*microseconds::period::num / microseconds::period::den;
        theLog->WriteLog(log_error, "query_single_time:%lf",timed);
*/
        free_pc_request(const_cast<IPolicyRequest*>(reqs[0]));
        free(reqs);

        if (QS_S_OK != status ) {
            pExc->SetInfo(ERR_POLICY, "single_query_pc failed.");
            theLog->WriteLog(log_error, "single_query_pc failed");
            return;
        }

        if (!EMDBConfig::GetInstance().get_policy_pc_parse_is_on() ) {
            //EnforcerPcResult* ret = new EnforcerPcResult();
            std::shared_ptr<EnforcerPcResult> ret = std::make_shared<EnforcerPcResult>();
            //EnforcerPcResult* ret = (EnforcerPcResult*)sptr.get();
            ret->SetTableId(conds[0].table_id);
            ret->SetDeny(false);
            ret->_filter_cond = "('PC_RESULT_PARSE'<>'OFF')";
            theLog->WriteLog(log_debug, "jpc_parse = 0, Do not parse the query pc result");

            vecresult.push_back(ret);
            if (EMDBConfig::GetInstance().get_policy_jpc_cache_is_on()) {
                CEnforcerMgr::Instance()->InsertQueryPCResultCaches(conds[0]._cache_key, ret);
            }

            free_pc_result(result);

            return;
        }

        SqlException e;
        std::shared_ptr<EnforcerPcResult> pc = std::dynamic_pointer_cast<EnforcerPcResult>(parser_result_array(result, conds[0], e));
        
        //EnforcerPcResult * pc = dynamic_cast<EnforcerPcResult*>(tmp);
        if(e.code != ERR_NULL ){
            theLog->WriteLog(log_warning, "single_query_pc parser_result_array failed: %s", e.cdetail.c_str());
            //log
            free_pc_result(result);
            return;
        }
        if( pc ==NULL){
            pExc->SetInfo(ERR_POLICY_NOMATCH, "no policy matched.");
            free_pc_result(result);
            return;
        }

        if(CommonFun::StrCaseCmp( action.c_str(), ACTION_SELECT) == 0){
            if (pc->IsDeny())
            {
                theLog->WriteLog(log_info, "QUERY PC DENY");
            }
            else
            {
                std::string mask = print_mask(pc->_mask_map);
                //printf("%s\n",mask.c_str());
                theLog->WriteLog(log_info, "QUERY PC MASK:%s", mask.c_str());
                theLog->WriteLog(log_info, "QUERY PC FILTER:%s", pc->_filter_cond.c_str());
            }

        }else if(CommonFun::StrCaseCmp( action.c_str(), ACTION_UPDATE) == 0 ||
                 CommonFun::StrCaseCmp( action.c_str(), ACTION_DELETE) == 0) {
            if (pc->IsDeny())
            {
                theLog->WriteLog(log_info, "QUERY PC DENY");
            }
            else
            {
                theLog->WriteLog(log_info, "QUERY PC FILTER:%s", pc->_filter_cond.c_str());
            }
        }
        free_pc_result(result);

        parse_ctx->_vec_report.push_back(log_reqs2[0]);
        vecresult.push_back(pc);
        if (EMDBConfig::GetInstance().get_policy_jpc_cache_is_on()) {
            CEnforcerMgr::Instance()->InsertQueryPCResultCaches(conds[0]._cache_key, pc);
        }

    } else {
        IPolicyResult ** result = (IPolicyResult**)calloc(cnt_real,sizeof(IPolicyResult*));

        QueryStatus status= multi_query_pc(&reqs[0], (int)cnt_real, result);
/*
        auto st = system_clock::now();
        auto ed = system_clock::now();
        auto duration = duration_cast<microseconds>(ed-st);
        double  timed = double (duration.count())*microseconds::period::num / microseconds::period::den;
        theLog->WriteLog(log_error, "query_multi_time:%lf",timed);
*/
        for(size_t i = 0 ;i < cnt_real ;++i) {
            free_pc_request(const_cast<IPolicyRequest *>(reqs[i]));
        }
        free(reqs);

        if (QS_S_OK != status ) {
            pExc->SetInfo(ERR_POLICY, "multi_query_pc failed.");
            theLog->WriteLog(log_error, "multi_query_pc failed");
            free(result);
            return;
        }
        for (size_t i = 0; i < cnt_real; ++i)
        {
            if (!EMDBConfig::GetInstance().get_policy_pc_parse_is_on()) {
                //EnforcerPcResult* ret = new EnforcerPcResult();
                std::shared_ptr<EnforcerPcResult> ret = std::make_shared<EnforcerPcResult>();
                //EnforcerPcResult* ret = (EnforcerPcResult*)sptr.get();
                ret->SetTableId(conds[i].table_id);
                ret->SetDeny(false);
                ret->_filter_cond = "('PC_RESULT_PARSE'<>'OFF')";
                theLog->WriteLog(log_debug, "jpc_parse = 0, Do not parse the query pc result");

                vecresult.push_back(ret);
                free_pc_result(result[i]);

                if (EMDBConfig::GetInstance().get_policy_jpc_cache_is_on()) {
                    CEnforcerMgr::Instance()->InsertQueryPCResultCaches(conds[i]._cache_key, ret);
                }

                continue;
            }

            SqlException e;
            std::shared_ptr<EnforcerPcResult> pc = NULL;
            pc = std::dynamic_pointer_cast<EnforcerPcResult>(parser_result_array(result[i], conds[i], e));

            if(e.code != ERR_NULL){
                //log
                theLog->WriteLog(log_warning, "multi_query_pc parser_result_array failed: %s", e.cdetail.c_str());
                free_pc_result(result[i]);
                continue;
            }
            if(pc==NULL){
                free_pc_result(result[i]);
                continue;
            }
            if(CommonFun::StrCaseCmp( action.c_str(), ACTION_SELECT) == 0){
                if (pc->IsDeny())
                {
                    theLog->WriteLog(log_info, "QUERY PC DENY");
                }
                else
                {
                    std::string mask = print_mask(pc->_mask_map);
                    theLog->WriteLog(log_info, "QUERY PC MASK:%s", mask.c_str());
                    theLog->WriteLog(log_info, "QUERY PC FILTER:%s", pc->_filter_cond.c_str());
                }
            }else if(CommonFun::StrCaseCmp( action.c_str(), ACTION_UPDATE) == 0 ||
                     CommonFun::StrCaseCmp( action.c_str(), ACTION_DELETE) == 0) {
                if (pc->IsDeny())
                {
                    theLog->WriteLog(log_info, "QUERY PC DENY");
                }
                else
                {
                    theLog->WriteLog(log_info, "QUERY PC FILTER:%s", pc->_filter_cond.c_str());
                }
            }else if(CommonFun::StrCaseCmp(action.c_str(), ACTION_CREATE) == 0) {
                if (pc->IsDeny())
                {
                    theLog->WriteLog(log_info, "QUERY PC DENY");
                }
            }else if(CommonFun::StrCaseCmp(action.c_str(), ACTION_DROP) == 0) {
                if (pc->IsDeny())
                {
                    theLog->WriteLog(log_info, "QUERY PC DENY");
                }
            }
            free_pc_result(result[i]);

            parse_ctx->_vec_report.push_back(log_reqs2[i]);

            vecresult.push_back(pc);
            if (EMDBConfig::GetInstance().get_policy_jpc_cache_is_on()) {
                CEnforcerMgr::Instance()->InsertQueryPCResultCaches(conds[i]._cache_key, pc);
            }
        }
        if(vecresult.size() == 0){
            pExc->SetInfo(ERR_POLICY_NOMATCH, "no policy matched.");
            theLog->WriteLog(log_info, "QUERY PC :No Matched");
        }
        free(result);
    }
}

void CEnforcerMgr::ColumnVisit(IPlan *plan, IColumnsRefItem *cli)
{
    return;

}

void CEnforcerMgr::StartNewStmt(IPlan *plan,  uint64_t lastStatID)
{
    return;

}











void CEnforcerMgr::WhereCluase(IPlan *plan, IWhereCluase *wc)
{
    return;

}

void CEnforcerMgr::ErrorOccur(IPlan *plan)
{
    //get parse context
    SQLParseCtx *pParseCtx = (SQLParseCtx *)(plan->GetContext());
    pParseCtx->bError = true;
    pParseCtx->strErrorDetail = plan->GetErrorDetail();
    pParseCtx->_exc.SetInfo(ERR_PARSE,plan->GetErrorDetail());
    #ifdef _DEBUG
    printf("ErrorOccur:%s\n", plan->GetErrorDetail().c_str());
    #endif
}

std::string CEnforcerMgr::GetPolicyActionByStmtType(StmtType stmtType)
{
    switch (stmtType)
    {
    case E_STMT_TYPE_SELECT:
        return ACTION_SELECT;
    case E_STMT_TYPE_UPDATE:
        return ACTION_UPDATE;
    case E_STMT_TYPE_DELETE:
        return ACTION_DELETE;
    case E_STMT_TYPE_INSERT:
        return ACTION_INSERT;
    case E_STMT_TYPE_CREATE_TABLE:
        return ACTION_CREATE;
    case E_STMT_TYPE_DROP_TABLE:
        return ACTION_DROP;
    default:
        return ACTION_NONE;
    }
}

void ComposePolicyResource(CPolicyResource & resource,  UserContextInfo *pUserCtx,  ITableItem* tbi){
    assert(pUserCtx && tbi );
    resource.AddedAttribute(ATTR_NAME_SERVER, pUserCtx->GetDBServer().c_str());
    resource.AddedAttribute(ATTR_NAME_DATABASE, pUserCtx->GetCurrentDB().c_str());
    EMDB_DB_TYPE type = pUserCtx->GetEMDBType();
    switch(type) {
        case EMDB_DB_DB2:
        case EMDB_DB_HANA:
        case EMDB_DB_BIGQUERY:
        case EMDB_DB_SQLSERVER: {
            resource.AddedAttribute(ATTR_NAME_DATABASE, tbi->GetDatabaseName().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, tbi->GetSchemaName().c_str());
        } break; 
        case EMDB_DB_MYSQL: { 
            resource.AddedAttribute(ATTR_NAME_DATABASE, tbi->GetSchemaName().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, tbi->GetSchemaName().c_str());
        } break; 
        case EMDB_DB_ORACLE: {
            resource.AddedAttribute(ATTR_NAME_DATABASE, pUserCtx->GetCurrentDB().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, tbi->GetSchemaName().c_str());
        } break;
        default:{
            assert(false); /* unreachable */
        }
    }
    
    resource.AddedAttribute(ATTR_NAME_TABLE, tbi->GetTableName().c_str());           
}

void ComposePolicyResource(CPolicyResource& resource, /*const*/ UserContextInfo *pUserCtx,/*const*/  IStoreProcedure* pProcedure) {
    assert(pUserCtx && pProcedure );
    resource.AddedAttribute(ATTR_NAME_SERVER, pUserCtx->GetDBServer().c_str());
    resource.AddedAttribute(ATTR_NAME_DATABASE, pUserCtx->GetCurrentDB().c_str());
    EMDB_DB_TYPE type = pUserCtx->GetEMDBType();
    switch(type) {
        case EMDB_DB_DB2:
        case EMDB_DB_HANA:
        case EMDB_DB_BIGQUERY:
        case EMDB_DB_SQLSERVER: {
            resource.AddedAttribute(ATTR_NAME_DATABASE, pProcedure->GetDatabaseName().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, pProcedure->GetSchemaName().c_str());
        } break; 
        case EMDB_DB_MYSQL: { 
            resource.AddedAttribute(ATTR_NAME_DATABASE, pProcedure->GetSchemaName().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, pProcedure->GetSchemaName().c_str());
        } break; 
        case EMDB_DB_ORACLE: {
            resource.AddedAttribute(ATTR_NAME_DATABASE, pUserCtx->GetCurrentDB().c_str());
            resource.AddedAttribute(ATTR_NAME_SCHEMA, pProcedure->GetSchemaName().c_str());
        } break;
        default: {
            assert(false); /* unreachable */
        } break;
    }
    
    resource.AddedAttribute(ATTR_NAME_PROCEDURE, pProcedure->GetStoreProcedureName().c_str()); 
}

void CEnforcerMgr::GetLocalHostInfo(std::string & host_name, std::string & host_ip){
    if(_hostname.empty()){
        host_name = "localhost";
    } else {
        host_name = _hostname;
    }
    if(_host_ip.empty()){
        host_ip = "255.255.255.255";
    } else {
        host_ip = _host_ip;
    }
}

void CEnforcerMgr::GetAppNameInfo(std::string & app_name, std::string & app_path){
    if(_app_name.empty()){
        app_name = "unknown";
    } else {
        app_name = _app_name;
    }
    if(_app_path.empty()){
        app_path = "unknown";
    } else {
        app_path = _app_path;
    }
}


void  CEnforcerMgr::InsertQueryPCResultCaches(const std::string& key, std::shared_ptr<QueryPcResult>  result) {
    EnforcerPcResult* pers = dynamic_cast<EnforcerPcResult*>(result.get());
    if (pers) {
        std::lock_guard<std::shared_timed_mutex> writerLock(_querypc_caches_mutex);
        _map_querypc_caches[key] = std::make_shared<EnforcerPcResult>(*pers);
    }
    
}

bool  CEnforcerMgr::GetQueryPCResultCaches(const std::string& key, std::shared_ptr<QueryPcResult> & result) {
    std::shared_lock<std::shared_timed_mutex> readerLock(_querypc_caches_mutex);
    auto it = _map_querypc_caches.find(key);
    if (it == _map_querypc_caches.end()) {
        return false;
    }
    else {
        QueryPcResult* prs = it->second.get();
        
        if (prs) {
            EnforcerPcResult* pers = dynamic_cast<EnforcerPcResult*>(prs);
            EnforcerPcResult* pnew = new EnforcerPcResult(*pers);
            result = std::shared_ptr<EnforcerPcResult>(pnew);
        }
        
    }
    return true;
}

void CEnforcerMgr::ClearQueryPCResultCaches_SyncFunc() {

    //wait time 
    do {
        if (EMDBConfig::GetInstance().get_policy_cc_switch_is_on()) {
            std::unique_lock< std::mutex > uniqlock(_caches_mutex);
            _caches_cond.wait(uniqlock,
                [this] {
                    return _b_need_update_cache;
                });
            _b_need_update_cache = false;
            uniqlock.unlock();

            std::lock_guard<std::shared_timed_mutex> writerLock(_querypc_caches_mutex);
            _map_querypc_caches.clear();
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(EMDBConfig::GetInstance().get_policy_cc_sync_time()));
        }
    } while (1);    
}

void CEnforcerMgr::ClearQueryPCResultCaches_SyncFunc2() {
    //wait time 
    do {
        if (EMDBConfig::GetInstance().get_policy_jpc_cache_is_on()) {
            std::this_thread::sleep_for(std::chrono::seconds(EMDBConfig::GetInstance().get_jpc_cache_clear_time()));
            std::lock_guard<std::shared_timed_mutex> writerLock(_querypc_caches_mutex);
            if (_map_querypc_caches.size() > 100000) {//check size
                _map_querypc_caches.clear();
                theLog->WriteLog(log_info, "clear querypc result(by jpc_cache_clear_time config)");
            }
        }
        else {
            break;
        }
    } while (1);
}

void CEnforcerMgr::UpdatePCResultCachesStatus() {
    _caches_mutex.lock();
    _b_need_update_cache = true;
    _caches_mutex.unlock();
    _caches_cond.notify_one();
}

std::vector<TableColumn> CEnforcerMgr::QueryTableColumns(const std::string& dbname, const std::string& schema, const std::string& table) {
    const auto& emdb_config = EMDBConfig::GetInstance();

    EMDBConfig::WorkerConfig worker_config;
    std::string metadata_odbc_conn_string;

    if (emdb_config.get_tdsproxyworker_config(_worker_name, worker_config)) {
        metadata_odbc_conn_string = worker_config.odbc_connection_string_;
    }

    std::vector<TableColumn> table_columns{};

    static const std::string kQueryTableMetaSqlFormat
        = "SELECT TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, COLUMN_NAME, DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS"
            " WHERE TABLE_CATALOG = '%s' AND TABLE_SCHEMA = '%s' AND TABLE_NAME = '%s'";
    const std::string sql = CommonFun::StringFormat(kQueryTableMetaSqlFormat, dbname.c_str(), schema.c_str(), table.c_str());
    const std::string conn_str = metadata_odbc_conn_string + CommonFun::StringFormat("Database=%s;", dbname.c_str());

    nanodbc::connection conn{conn_str};

    auto result = nanodbc::execute(conn, NANODBC_TEXT(sql));

    while (result.next()) {
        const auto column_name = result.get<std::string>(3);
        const auto column_type = result.get<std::string>(4);

        table_columns.emplace_back(TableColumn(column_name, column_type));
    }

    return table_columns;
}

std::string CEnforcerMgr::QueryDbServerName() {
  static const std::string sql = "select @@SERVERNAME";
  std::string db_server_name{};
  nanodbc::connection conn{ _odbc_connection_string };
  auto result = nanodbc::execute(conn, sql);

  if (result.next())
  {
    db_server_name = result.get<std::string>(0);
  }

  return db_server_name;
}

std::string CEnforcerMgr::QueryDefaultSchema(const std::string& dbname, const std::string& owner) {
  static const std::string kQueryDefaultSchemaSqlFormat = "EXEC AS LOGIN = '%s';SELECT SCHEMA_NAME();REVERT;";
  const std::string sql = CommonFun::StringFormat(kQueryDefaultSchemaSqlFormat, owner.c_str());
  std::string default_schema{};
  const std::string conn_str = _odbc_connection_string + CommonFun::StringFormat("Database=%s;", dbname.c_str());
  nanodbc::connection conn{conn_str, 60};
  auto result = nanodbc::execute(conn, NANODBC_TEXT(sql), 1, 60);

  if (result.next())
  {
    if (!result.is_null(0)) {
      default_schema = result.get<std::string>(0);
    }
  }

  return default_schema;
}
