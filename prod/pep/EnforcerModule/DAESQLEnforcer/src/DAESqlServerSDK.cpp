#include "DAESqlServerSDK.h"
#include "EMDBConfig.h"
#include "DAELog.h"
#include "SqlException.h"
#include "enforcermgr.h"
#include "sqlparserwrapper.h"
#include "usercontextinfo.h"
#include <string>
#ifdef  WIN32
#include <windows.h>
long g_ora_enforcer_inited = 0;
#else
#include <atomic>
std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;
#endif

#include "emdb_handle.h"
#include "default_schema_cache_manager.h"
#define HANDLE_DAE_RESULT 43128

//bool copy_string_ora(const std::string& src, char *buf, unsigned int& buffsize) {
//    if (!buf || buffsize <= 0) {
//        return false;
//    }
//    std::string i_str = src;
//    bool ret = true;
//    int len = i_str.length();
//    if (len > buffsize - 1) {
//        len = buffsize - 1;
//        ret = false;
//    }
//    strncpy(buf, i_str.c_str(), len);
//    buf[len] = '\0';
//    buffsize = len;
//    return ret;
//}


bool initalize_enforcer_ora(const char* worker_name, SqlException* pExc)
{

    bool bRet = true;
#ifdef  WIN32
    bool bInit = InterlockedCompareExchange(&g_ora_enforcer_inited, 1, 0);
#else
    bool bInit = lock_stream.test_and_set();
#endif
    if (!bInit)
    {
        try
        {
            //load config file
            const std::string strCfgFile = CommonFun::GetConfigFilePath();
            EMDBConfig::GetInstance().Load(strCfgFile, worker_name);

            CEnforcerMgr::Instance()->Init_SDK(worker_name, pExc);
        }
        catch (const std::exception& e)
        {
            pExc->SetInfo(ERR_CONFIG, e.what());
            return false;
        }
    }
    return bRet;
}

void covert_binding_sql_params(const DAESqlServer::DAE_BindingParam* src, const unsigned int src_size, VecBindSqlParams & target) {
    if ( src==NULL || src_size == 0 ) return;
    for (size_t i = 0; i < src_size; ++i) {
        DAEBindingParam param;
        param._value = src[i]._value;
        target.push_back(param);
    }
}


class DAEResult :public CEMDBHandle {
public:
    DAEResult() :CEMDBHandle(HANDLE_DAE_RESULT), _code(DAESqlServer::DAE_ResultCode::DAE_IGNORE){}
    virtual ~DAEResult(){}

    DAESqlServer::DAE_ResultCode GetCode() { return _code; };
    const char* GetDetail() { return _detail.c_str(); };
    const char* GetDefaultDatabase() { return _default_database.c_str(); }
    void SetValue(DAESqlServer::DAE_ResultCode code, const std::string& detail, const std::string& default_db = "") {
        _code = code;
        _detail = detail;
        _default_database = default_db;
    };
private:
    DAESqlServer::DAE_ResultCode _code{ DAESqlServer::DAE_ResultCode::DAE_IGNORE };
    std::string _detail;
    std::string _default_database;
};



namespace DAESqlServer{

    bool DAESqlServerInit(const char* worker_name) {
        SqlException exc;
        initalize_enforcer_ora(worker_name, &exc);
        if (exc.IsBreak()) {
            theLog->WriteLog(LOG2FILE, log_error, "Enforce init error: %s", exc.cdetail.c_str());
            theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_error, "SQL enforcer module initialization failed.");
            return false;
        }
        theLog->WriteLog(LOG2CONSOLE | LOG2FILE, log_info, "SQL enforcer module initialized successfully.");
        return true;
    }

    bool DAEIsSqlTextFormat(const char *sql)
    {
        if (sql == nullptr || !sql[0] || CSqlparserWrapper::Instance()->ParseSql == nullptr)
            return false;

        IParseResult* res = CSqlparserWrapper::Instance()->ParseSql(sql, EMDB_DB_SQLSERVER);
        if (res)
        {
            bool bsql = res->IsAccept();
            CSqlparserWrapper::Instance()->DestroyParseResult(res);
            return bsql;
        }

        return false;
    }

    bool    EnforceSQL_simple(
        const char* sql,   /*in*/
        const DAE_UserProperty* userattrs,   /*in*/
        const unsigned int userattrs_size,  /*in*/
        DAEResultHandle result /*in*/
    ) {
        return EnforceSQL_simpleV2(sql,
            userattrs,
            userattrs_size,
            nullptr,
            0,
            result);
    }

    bool    EnforceSQL_simpleV2(
        const char *sql, 
        const DAE_UserProperty* userattrs, 
        const unsigned int userattrs_size,
        const DAE_BindingParam* bindingparams, /*in*/
        const unsigned int params_size,  /*in*/
        DAEResultHandle result)
        {
         
            // EMDB_SUCCESS, EMDB_ERROR /* todo */, EMDB_INVALID_HANDLE
        CEMDBHandle* phd = (CEMDBHandle*)result;
        DAEResult* daeresult = NULL;
        if (phd && phd->IsType(HANDLE_DAE_RESULT)) {
            daeresult = dynamic_cast<DAEResult*>(phd);
        }
        if (!daeresult) {
            //printf(" EnforceSQL_simpleV2 failed (var daeresult == NULL)");
            return false;
        }
         
        if(sql == NULL){
            daeresult->SetValue(DAE_ResultCode::DAE_IGNORE, "SQL stayement is empty");
            //printf(" EnforceSQL_simpleV2 failed (sql == NULL)");
            return false;
        }
        //if(userattrs_size == 0 || userattrs == NULL){
        //    daeresult->SetValue(DAE_ResultCode::DAE_IGNORE, "user id and user property is empty");
        //    printf(" EnforceSQL_simpleV2 failed (userattrs_size == 0 || userattrs == NULL)");
        //    return false;
        //}
        SqlException exc;
        //initalize_enforcer_ora(&exc);
        //if(exc.IsBreak()){
        //    daeresult->SetValue(DAE_ResultCode::DAE_FAILED, exc.cdetail);
        //    //printf(" EnforceSQL_simpleV2 initalize_enforcer_ora failed err:%s ", exc.cdetail.c_str());
        //    return false;
        //}
        theLog->WriteLog(log_info, "begin EnforceSQL userattrs_size=%d params_size=%d", userattrs_size, params_size);

        bool bret = true;
        USER_CONTEXT ctx = CEnforcerMgr::Instance()->NewContext_SDK( &exc);
        if(exc.IsBreak()){
            daeresult->SetValue(DAE_ResultCode::DAE_FAILED, exc.cdetail);
            theLog->WriteLog(log_info, "block EnforceSQL  err:%s", exc.cdetail.c_str());
            return false;
        }
        if(!ctx){
            daeresult->SetValue(DAE_ResultCode::DAE_FAILED, exc.cdetail);
            theLog->WriteLog(log_info, "block EnforceSQL  ctx is null");
            return false;
        }

        const auto db_server_name = CEnforcerMgr::Instance()->GetDbServerName();

        CEnforcerMgr::Instance()->SetUserContextInfo(ctx,CONTEXT_INFO_SERVER,db_server_name.c_str());
        //CEnforcerMgr::Instance()->SetUserContextInfo(ctx,CONTEXT_INFO_DATABASE,"" );
        //CEnforcerMgr::Instance()->SetUserContextInfo(ctx,CONTEXT_INFO_SCHEMA,""  );
        CEnforcerMgr::Instance()->SetUserContextInfo(ctx,CONTEXT_INFO_DB_TYPE,"SQL Server" );

        CEnforcerMgr::Instance()->AddUserAttrValue_SDK(ctx, "id", "dae_unknown", &exc);
        uint8_t source = EMDBConfig::GetInstance().get_user_source();
        bool sid = false;
        for (size_t i = 0; i < userattrs_size; ++i) {
            if (strcmp(userattrs[i]._key, KEY_SERVICE_NAME) == 0) {
                CEnforcerMgr::Instance()->SetUserContextInfo(ctx, CONTEXT_INFO_DATABASE, userattrs[i]._value);
            }
            else if (strcmp(userattrs[i]._key, KEY_OWNER) == 0) {
                UserContextInfo *userCtx = CEnforcerMgr::Instance()->GetUserContextInfo(ctx);
                const auto owner = userattrs[i]._value;
                const auto database = userCtx->GetCurrentDB();
                const auto default_schema = DefaultSchemaCacheManager::Instance().GetDefaultSchema(database, owner);
                const auto schema = default_schema.empty() ? "dbo" : default_schema;

                CEnforcerMgr::Instance()->SetUserContextInfo(ctx, CONTEXT_INFO_SCHEMA, schema.c_str());
                if (sid == false && (USER_SOURCE_DB_USER & source) != 0) {
                    CEnforcerMgr::Instance()->AddUserAttrValue_SDK(ctx, "id", userattrs[i]._value, &exc);
                }
            }
            else if (strcmp(userattrs[i]._key, KEY_CLIENT_APP_NAME) == 0) {
                CEnforcerMgr::Instance()->SetUserContextInfo(ctx, CONTEXT_INFO_CLIENT_APP, userattrs[i]._value);
            }
            else if (strcmp(userattrs[i]._key, KEY_CLIENT_HOST_NAME) == 0) {
                CEnforcerMgr::Instance()->SetUserContextInfo(ctx, CONTEXT_INFO_CLIENT_HOST_NAME, userattrs[i]._value);
            }
            else if (strcmp(userattrs[i]._key, KEY_CLIENT_IP) == 0) {
                CEnforcerMgr::Instance()->SetUserContextInfo(ctx, CONTEXT_INFO_CLIENT_IP, userattrs[i]._value);
            }
            else {
                CEnforcerMgr::Instance()->AddUserAttrValue_SDK(ctx, userattrs[i]._key, userattrs[i]._value, &exc);
                if (strcmp(userattrs[i]._key, KEY_AUTH_SID) == 0 && (USER_SOURCE_AD_USER & source) != 0) {
                    sid = true;
                    CEnforcerMgr::Instance()->AddUserAttrValue_SDK(ctx, "id", userattrs[i]._value, &exc);
                }
            }
        }
        
        VecBindSqlParams params;
        covert_binding_sql_params(bindingparams, params_size, params);

        std::string strNew = CEnforcerMgr::Instance()->EvaluationSQL(ctx, sql, params, &exc);
        switch (exc.code){
            case ERR_PARSE:
            case ERR_POLICY_PARSER:
            case ERR_MASK_OPR:
            case ERR_FILTER_OPR:
            case ERR_POLICY_NOMATCH:{
                daeresult->SetValue(DAE_ResultCode::DAE_IGNORE, exc.cdetail, ((UserContextInfo*)ctx)->GetCurrentDB());
                theLog->WriteLog(log_info, "EnforceSQL SDK for SQL Server:IGNORE exception:%s",exc.cdetail.c_str());
            } break;
            case ERR_POLICY:
            case ERR_USERINFO:{
                daeresult->SetValue(DAE_ResultCode::DAE_FAILED, exc.cdetail, ((UserContextInfo*)ctx)->GetCurrentDB());
                theLog->WriteLog(log_info, "EnforceSQL SDK for SQL Server:FAILED exception:%s",exc.cdetail.c_str());
                bret = false;
            } break;
            default:{
                strNew += "--NXLDAE";
                daeresult->SetValue(DAE_ResultCode::DAE_ENFORCED, strNew, ((UserContextInfo*)ctx)->GetCurrentDB());

                //
                theLog->WriteLog(log_info, "EnforceSQL SDK for SQL Server:ENFORCED ");
                break;
            }
        }
        CEnforcerMgr::Instance()->FreeUserContext_SDK(ctx);
        theLog->WriteLog(log_info, "end EnforceSQL resultcode=%d\n=======================================>>>>>>>\n\n", daeresult->GetCode());
        return bret;
    
    }


    bool DAENewResult(DAEResultHandle* output_result) {
        *output_result = new DAEResult();
        return true;
    }

    bool DAEFreeResult(DAEResultHandle result) {
        if (nullptr == result) {
            return false;
        }

        CEMDBHandle* phd = (CEMDBHandle*)result;
        if (phd->IsType(HANDLE_DAE_RESULT)) {
            DAEResult* pResult = dynamic_cast<DAEResult*>(phd);
            if (!pResult) {
                return false;
            }
            delete pResult;
            return true;
        }
        return false;
        
    }

    bool DAEGetResult(DAEResultHandle result, DAE_ResultCode* output_code, const char** output_detail, const char** output_db) {
        if (nullptr == result) {
            return false;
        }

        CEMDBHandle* phd = (CEMDBHandle*)result;
        if (phd->IsType(HANDLE_DAE_RESULT)) {
            DAEResult* presult = dynamic_cast<DAEResult*>(phd);
            if (!presult) {
                return false;
            }
            *output_code = presult->GetCode();
            *output_detail = presult->GetDetail();
            *output_db = presult->GetDefaultDatabase();
            return true;
        }
        return false;
    }

}