#include "enforcerwrapper.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif 

#include "commfun.h"
#include "DAELog.h"

#define GET_Enforcer_PROC(funname) funname=(funname##Fun)CommonFun::GetProcAddress(hEnforcer, #funname);

Enforcer::Enforcer()
{
    SQLEnforcerInit             = NULL;
    SQLEnforcerNewContext       = NULL;
    SQLEnforcerFreeContext      = NULL;
    SQLEnforcerEvaluationSQLW   = NULL;
    SQLEnforcerEvaluationSQLA   = NULL;
    SQLEnforcerWriteLogA        = NULL;
    SetUserContextInfo          = NULL;
    SQLEnforcerQueryMetadata_cb = NULL;
}

bool Enforcer::LoadEnforcer()
{
    std::string EnforcerPath = CommonFun::GetEnforcerModule();
    //printf("load enforcer from:%s\n", EnforcerPath.c_str() );   /* here we can not call theLog */
    theLog->WriteLog(log_trace, "load enforcer from:%s", EnforcerPath.c_str());
    void* hEnforcer = CommonFun::LoadShareLibrary(EnforcerPath.c_str());
    if(hEnforcer==NULL)
    {
        //printf(LOAD_MODULE_FAILED, EnforcerPath.c_str(), CommonFun::ShareLibraryError().c_str());
        theLog->WriteLog(log_error, LOAD_MODULE_FAILED, EnforcerPath.c_str(), CommonFun::ShareLibraryError().c_str());
        return false;
    }

    //get process address
    
    GET_Enforcer_PROC(SQLEnforcerInit);
    GET_Enforcer_PROC(SQLEnforcerNewContext);
    GET_Enforcer_PROC(SQLEnforcerFreeContext);
    GET_Enforcer_PROC(SQLEnforcerEvaluationSQLW);
    GET_Enforcer_PROC(SQLEnforcerEvaluationSQLA);
    GET_Enforcer_PROC(SQLEnforcerWriteLogA);
    GET_Enforcer_PROC(SetUserContextInfo);
    GET_Enforcer_PROC(SQLEnforcerQueryMetadata_cb);

    return SQLEnforcerInit!=NULL &&
           SQLEnforcerNewContext!=NULL && 
           SQLEnforcerFreeContext!=NULL &&
           SQLEnforcerEvaluationSQLW!=NULL &&
           SQLEnforcerEvaluationSQLA!=NULL &&
           SQLEnforcerWriteLogA!=NULL && 
           SetUserContextInfo!=NULL &&
           SQLEnforcerQueryMetadata_cb!=NULL;
}