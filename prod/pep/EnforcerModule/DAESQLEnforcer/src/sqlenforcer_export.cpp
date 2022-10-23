#include "sqlenforcer_export.h"
#include "enforcermgr.h"
#include "DAELog.h"



 bool SQLEnforcerInit(const char* szCallerModuleName,const char* szConfigFile, SqlException * pExc)
{
    bool bInit = CEnforcerMgr::Instance()->Init_SDK(szCallerModuleName,  pExc);
    return bInit;
}

 USER_CONTEXT SQLEnforcerNewContext(const char* szUserName, const char* szPwd, SqlException * pExc)
{
    USER_CONTEXT context = CEnforcerMgr::Instance()->NewContext(szUserName, szPwd, pExc);
    return context;
}


 void SQLEnforcerFreeContext(USER_CONTEXT context)
{
    CEnforcerMgr::Instance()->FreeContext(context);
}
 
void SetUserContextInfo(USER_CONTEXT ctx, USER_CONTEXT_INFO_TYPE infoType, const char *szInfo)
{
    CEnforcerMgr::Instance()->SetUserContextInfo(ctx, infoType, szInfo);

}

 std::wstring SQLEnforcerEvaluationSQLW(USER_CONTEXT context, const wchar_t* sqltext , SqlException * pExc)
{
     VecBindSqlParams params;
     return CEnforcerMgr::Instance()->EvaluationSQL(context, sqltext, params, pExc);
}

 std::string SQLEnforcerEvaluationSQLA(USER_CONTEXT context, const char* sqltext, SqlException * pExc)
{
     VecBindSqlParams params;
    return CEnforcerMgr::Instance()->EvaluationSQL(context, sqltext, params, pExc);
}

 int SQLEnforcerWriteLogA(int level, const char* msg)
{
   return theLog->WriteLog(level, "%s", msg);
}

bool SQLEnforcerQueryMetadata_cb(USER_CONTEXT context, QueryMetadataFunc func){
    if(context && func){
        CEnforcerMgr::Instance()->SetMetadataFunc((USER_CONTEXT)context,func);
        return true;
    } else 
    {
        return false;
    }
    
}

