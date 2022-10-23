#ifndef ENFORCER_H_2019_01_21
#define ENFORCER_H_2019_01_21

#include <string>
#include "EMMaskDef.h"
#include "commfun.h"



struct SqlException;
typedef bool (*SQLEnforcerInitFun)(const char* szCallerModuleName,const char* szCfgFile, SqlException*pExc);
typedef USER_CONTEXT (*SQLEnforcerNewContextFun)(const char* userID,const char* Password, SqlException*pExc);
typedef void  (*SetUserContextInfoFun)(USER_CONTEXT ctx, USER_CONTEXT_INFO_TYPE infoType, const char *szInfo);
typedef void  (*SQLEnforcerFreeContextFun)(const wchar_t* userID);
typedef std::wstring (*SQLEnforcerEvaluationSQLWFun)(USER_CONTEXT context, const wchar_t* sqltext,  SqlException * pExc);
typedef std::string  (*SQLEnforcerEvaluationSQLAFun)(USER_CONTEXT context, const char*    sqltext,  SqlException * pExc);
typedef int  (*SQLEnforcerWriteLogAFun)(int level, const char* msg);
typedef bool (*SQLEnforcerQueryMetadata_cbFun)(USER_CONTEXT context, QueryMetadataFunc func);

class Enforcer
{
   public:
   static Enforcer* Instance(){
       static Enforcer* pEnforcer = new Enforcer();
       return pEnforcer;
   }

   bool LoadEnforcer();

   //
   public:
   SQLEnforcerInitFun SQLEnforcerInit = NULL;
   SQLEnforcerNewContextFun SQLEnforcerNewContext = NULL;
   SetUserContextInfoFun  SetUserContextInfo = NULL;
   SQLEnforcerFreeContextFun SQLEnforcerFreeContext = NULL;
   SQLEnforcerEvaluationSQLWFun SQLEnforcerEvaluationSQLW = NULL;
   SQLEnforcerEvaluationSQLAFun SQLEnforcerEvaluationSQLA = NULL;
   SQLEnforcerWriteLogAFun SQLEnforcerWriteLogA = NULL;
   SQLEnforcerQueryMetadata_cbFun SQLEnforcerQueryMetadata_cb = NULL;

   protected:
   Enforcer();
   Enforcer(const Enforcer&){};

};

#endif 