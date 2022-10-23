#include "emdb_sdk.h"

#include <vector>
#include <cstring>

#include "enforcerwrapper.h"
#include "commfun.h"
#include "EMDBConfig.h"
#include <stdio.h>
#include "SqlException.h"

#include "enforcermgr.h"
#include "emdb_result_sdk.h"
#include "usercontextinfo.h"
#include <atomic>
#ifdef  WIN32
#include <windows.h>
#include <Shlobj.h>
long g_sdkinit = 0;
#else
std::atomic_flag lock_sdkinit = ATOMIC_FLAG_INIT;
#endif
std::atomic_int64_t inti_ret = 0;

//#ifdef  WIN32
//            InterlockedCompareExchange(&g_sdkinit, 0, 1);  //https://docs.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-interlockedcompareexchange
//#else
//            lock_sdkinit.clear();   //http://www.cplusplus.com/reference/atomic/atomic_flag/test_and_set/
//#endif

EMDBReturn EMDBInit(const char *module_name){
    SqlException exc;
    bool bRet = true;
#ifdef  WIN32
    bool bInit = InterlockedCompareExchange(&g_sdkinit, 1, 0);
#else
    bool bInit = lock_sdkinit.test_and_set();
#endif
    if (!bInit)
    {
        bRet = CEnforcerMgr::Instance()->Init_SDK(module_name, &exc);
        if(bRet) inti_ret = 1;
        else inti_ret = 0;
    }

	if(inti_ret==0){
		return EMDBReturn::EMDB_ERROR;
	}
    return EMDBReturn::EMDB_SUCCESS;
}


EMDBReturn EMDBNewUserCtx(const char *server_name, const char *database_name, const char *schema_name, const char *dbtype, EMDBUserCtxHandle *output_user_ctx){
    //new user context 
    SqlException exc;

    *output_user_ctx = (EMDBUserCtxHandle)CEnforcerMgr::Instance()->NewContext_SDK( &exc);
	if(exc.IsBreak()){
		return EMDB_ERROR;
	}
    CEMDBHandle* p= (CEMDBHandle*)(*output_user_ctx);
    if(!dynamic_cast<UserContextInfo*>(p)){
        return EMDB_INVALID_HANDLE;
    }
    CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_SERVER,server_name );
    CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_DATABASE,database_name );
    CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_SCHEMA,schema_name );
    CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_DB_TYPE,dbtype );

    return EMDB_SUCCESS;
}

EMDBReturn EMDBFreeUserCtx(EMDBUserCtxHandle user_ctx){

	if(nullptr == user_ctx){
		return EMDB_INVALID_HANDLE;
	}
	CEMDBHandle* p= (CEMDBHandle*)user_ctx;
	if(!dynamic_cast<UserContextInfo*>(p)){
		return EMDB_INVALID_HANDLE;
	}
	if( CEnforcerMgr::Instance()->FreeUserContext_SDK((USER_CONTEXT)user_ctx) ){
		return EMDB_SUCCESS;
	}

    return EMDB_INVALID_HANDLE;
}

EMDBReturn EMDBSetUserCtxProperty(EMDBUserCtxHandle user_ctx, const char *key, const char *value){
     //m_pUserAttr set keyvalue
	 if(nullptr == user_ctx){
		return EMDB_INVALID_HANDLE;
	}
	CEMDBHandle* p= (CEMDBHandle*)user_ctx;
	if(!dynamic_cast<UserContextInfo*>(p)){
		return EMDB_INVALID_HANDLE;
	}

	if(0 == CommonFun::StrCaseCmp(key, "server")){
        CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_SERVER,value );
	} else if(0 == CommonFun::StrCaseCmp(key, "database")){
        CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_DATABASE,value );
	} else if (0 == CommonFun::StrCaseCmp(key, "schema")){
        CEnforcerMgr::Instance()->SetUserContextInfo((USER_CONTEXT)p,CONTEXT_INFO_SCHEMA,value );
	} else {
        SqlException exc;
        CEnforcerMgr::Instance()->AddUserAttrValue_SDK((USER_CONTEXT)p, key, value, &exc);
	}

	
    return EMDB_SUCCESS;
}

EMDBReturn EMDBClearUserCtxProperty(EMDBUserCtxHandle user_ctx){
	if(nullptr == user_ctx){
		return EMDB_INVALID_HANDLE;
	}
	CEMDBHandle* p= (CEMDBHandle*)user_ctx;
	if(!dynamic_cast<UserContextInfo*>(p)){
		return EMDB_INVALID_HANDLE;
	}
    CEnforcerMgr::Instance()->ClearUserContext_SDK((USER_CONTEXT)user_ctx);
    return EMDB_SUCCESS;
}

//EMDBReturn EMDBEvalSql(const char *sql, EMDBUserCtxHandle user_ctx, EMDBResultHandle result){
//    //according user_ctx to evaluation alter sql statement
//	if(nullptr == result || nullptr == user_ctx){
//		return EMDB_INVALID_HANDLE;
//	}
//	CEMDBHandle* p= (CEMDBHandle*)user_ctx;
//	if(!dynamic_cast<UserContextInfo*>(p)){
//		return EMDB_INVALID_HANDLE;
//	}
//
//	CEMDBHandle* pHandle= (CEMDBHandle*)result;
//	CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
//	if(!pResult){
//		return EMDB_INVALID_HANDLE;
//	}
//
//	SqlException exc;
//	std::string strNew = CEnforcerMgr::Instance()->EvaluationSQL((USER_CONTEXT)user_ctx, sql, &exc);
//
//	if(exc.IsBreak()){
//		pResult->SetValue(EMDB_BLOCK_THIS_TEXT, strNew);
//	}
//	else{
//		pResult->SetValue(EMDB_USE_NEW_TEXT, strNew);
//	}
//     return EMDB_SUCCESS;
//}

//#include "sqlenforcer_sp.h"
EMDBReturn EMDBEvalSql(const char *sql, EMDBUserCtxHandle user_ctx, EMDBResultHandle result){ //hard code to sp
    //according user_ctx to evaluation alter sql statement
    if(nullptr == result || nullptr == user_ctx){
        return EMDB_INVALID_HANDLE;
    }
    CEMDBHandle* p= (CEMDBHandle*)user_ctx;
    if(!dynamic_cast<UserContextInfo*>(p)){
        return EMDB_INVALID_HANDLE;
    }

    CEMDBHandle* pHandle= (CEMDBHandle*)result;
    CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
    if(!pResult){
        return EMDB_INVALID_HANDLE;
    }

    SqlException exc;
    std::string strNew;

#ifdef WINDOWS_APP_DEMO
    bool bret = true;
    do{
        std::string sql_sp(sql);
        EMDB_SP_INFO info;
        bret = EMDBSP_parser_sp_cmd(sql, info);
        if(!bret){
            break;
        }
        //std::string ori_cmd = EMDBSP_output_new_sp(info);
        std::map<unsigned int,std::string> sqls;
        bret = EMDBSP_load_new_sp( info);
        if(!bret){
            break;
        }
        bret = EMDBSP_carete_indirect_sql(sqls,info);
        if(!bret){
            break;
        }
        auto sqlit = sqls.begin();
        for(; sqlit!=sqls.end(); ++sqlit){
            std::string strNew = CEnforcerMgr::Instance()->EvaluationSQL((USER_CONTEXT)user_ctx, sqlit->second.c_str(), &exc);
            sqlit->second = strNew;

        }
        bret = EMDBSP_resolve_new_sp(sqls, info);
        if(!bret){
            break;
        }
        strNew = EMDBSP_output_new_sp(info);
    } while(false);
#else
    bool bret = false;
#endif

    if(!bret){
        VecBindSqlParams params;
        strNew = CEnforcerMgr::Instance()->EvaluationSQL((USER_CONTEXT)user_ctx, sql, params, &exc);
    }


    if(exc.code == ERR_DENY){
        pResult->SetValue(EMDB_DENY_EXECUTE, strNew);
    } else if(exc.IsBreak()){
        pResult->SetValue(EMDB_BLOCK_THIS_TEXT, strNew);
    } else{
        pResult->SetValue(EMDB_USE_NEW_TEXT, strNew);
    }
    return EMDB_SUCCESS;
}


EMDBReturn EMDBNewResult(EMDBResultHandle *output_result){
    // new EMDBResult 
	*output_result = (EMDBResultHandle)new CEMDBResult();
     return EMDB_SUCCESS;
}

// EMDBReturn EMDBClearResult(EMDBResultHandle result){
// 	if(nullptr == result){
// 		return EMDB_INVALID_HANDLE;
// 	}
// 	CEMDBHandle* pHandle= (CEMDBHandle*)result;
// 	CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
// 	if(!pResult){
// 		return EMDB_INVALID_HANDLE;
// 	}
	
// 	pResult->Clear();

//     return EMDB_SUCCESS;
// }

EMDBReturn EMDBFreeResult(EMDBResultHandle result){
     if(nullptr == result ){
		return EMDB_INVALID_HANDLE;
	}

	CEMDBHandle* pHandle= (CEMDBHandle*)result;
	CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
	if(!pResult){
		return EMDB_INVALID_HANDLE;
	}
	delete pResult;

	return EMDB_SUCCESS;
}

EMDBReturn EMDBResultGetCode(EMDBResultHandle result, EMDBResultCode *output_code){
	if(nullptr == result  ){
		return EMDB_INVALID_HANDLE;
	}
	if(nullptr == output_code){
		return EMDB_ERROR;
	}
	CEMDBHandle* pHandle= (CEMDBHandle*)result;
	CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
	if(!pResult){
		return EMDB_INVALID_HANDLE;
	}
	*output_code = pResult->GetCode();

    return EMDB_SUCCESS; 
}

EMDBReturn EMDBResultGetDetail(EMDBResultHandle result, const char **output_detail){
	if(nullptr == result ){
		return EMDB_INVALID_HANDLE;
	}

	if(nullptr == output_detail){
		return EMDB_ERROR;
	}

	CEMDBHandle* pHandle= (CEMDBHandle*)result;
	CEMDBResult * pResult = dynamic_cast<CEMDBResult*>(pHandle);
	if(!pResult){
		return EMDB_INVALID_HANDLE;
	}

	*output_detail = pResult->GetDetail();

    return EMDB_SUCCESS;
}

EMDBReturn EMDBQueryMetadata_cb(EMDBUserCtxHandle user_ctx, QueryMetadataFunc func){
    CEnforcerMgr::Instance()->SetMetadataFunc((USER_CONTEXT)user_ctx,func);
    return EMDB_SUCCESS;
}

#ifdef _WIN32

bool QueryMetadata(QueryMetadataFunc_c func, const std::string& tableName, MetadataVec& metadataVec){
    Metadata_c* metadataCArray = NULL;
    int metadataArraySize = 0;

    func(tableName.c_str(), &metadataCArray, &metadataArraySize);

    Metadata_c* curr = metadataCArray;

    for (int i = 0; i < metadataArraySize; ++i, ++curr)
    {
        Metadata metadata;
        metadata._col = curr->_col;
        metadata._type = curr->_type;

        metadataVec.push_back(metadata);
    }

    CoTaskMemFree(metadataCArray);

    return true;
}

EMDBReturn EMDBQueryMetadata_cb_c(EMDBUserCtxHandle user_ctx, QueryMetadataFunc_c func){
    CEnforcerMgr::Instance()->SetMetadataFunc((USER_CONTEXT)user_ctx, std::bind(QueryMetadata, func, std::placeholders::_1, std::placeholders::_2));
    return EMDB_SUCCESS;
}

bool encrypt_or_decrypt(const char * src, char * target, size_t &len , bool isdecrypt){
    std::string sp = isdecrypt?CommonFun::DecryptString(src):CommonFun::EncryptString(src);
    size_t  lensrc = sp.length();
    if(lensrc > len){
        return  false;
    }

    for(size_t i = 0; i < len; ++i){
        if(i < lensrc)
        {
            target[i] = sp[i];
        } else {
            target[i] = '\0';
            break;
        }
    }
    len = lensrc;
    return true;
}
bool DAEEncryptString(const char * src, char* target, size_t&len ){
    return encrypt_or_decrypt( src,  target, len , false);
}
bool DAEDecryptString(const char * src, char* target, size_t&len ){
    return encrypt_or_decrypt( src,  target, len , true);
}

int GetCfgUsermodeMode()
{
    const auto mode = EMDBConfig::GetInstance().get_usermode_mode();
    return static_cast<int>(mode);
}

unsigned GetCfgUsermodeSyncTime()
{
    const auto sync_time = EMDBConfig::GetInstance().get_usermode_sync_time();
    return sync_time;
}

void GetCfgUsermodeAdonetSqlclientConnString(char* buff, int buff_size)
{
    const auto conn_string = EMDBConfig::GetInstance().get_usermode_adonet_sqlclient_conn_string();

    strncpy(buff, conn_string.c_str(), buff_size);
    buff[buff_size - 1] = '\0'; // Ensure a valid terminating zero
}

void GetCfgUsermodeUserInfo(char* buff, int buff_size)
{
    const auto user_info = EMDBConfig::GetInstance().get_usermode_user_info();

    strncpy(buff, user_info.c_str(), buff_size);
    buff[buff_size - 1] = '\0'; // Ensure a valid terminating zero
}

#endif