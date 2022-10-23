#ifndef SQLENFORCER_EXPORT_H
#define SQLENFORCER_EXPORT_H

#include <stdio.h>

#ifndef WIN32
#define ENFORCER_API __attribute__((visibility("default")))
#else
#define SQLENFORCER_EXPORT
#ifdef SQLENFORCER_EXPORT
#define ENFORCER_API  __declspec(dllexport)
#else 
#define ENFORCER_API  __declspec(dllimport)
#endif //SQLENFORCER_EXPORT
#endif //WIN32


#include "commfun.h"
#include "EMMaskDef.h"


struct SqlException;
//extern "C"
//{
  ENFORCER_API bool SQLEnforcerInit(const char* szCallerModuleName/*odbc/jdbc*/, const char* szConfigPath, SqlException * pExc);

  /*
   * create user context based on username and pwd, this information can be get from ConnectString.
   * username and password can be used to get attribute of user from AD.
   */
  ENFORCER_API USER_CONTEXT SQLEnforcerNewContext(const char* szUserName,const char* szPwd, SqlException * pExc);
  ENFORCER_API void  SetUserContextInfo(USER_CONTEXT ctx, USER_CONTEXT_INFO_TYPE infoType, const char *szInfo);
  ENFORCER_API void  SQLEnforcerFreeContext(USER_CONTEXT context);
  ENFORCER_API std::wstring SQLEnforcerEvaluationSQLW(USER_CONTEXT context, const wchar_t* sqltext, SqlException * pExc); 
  ENFORCER_API std::string  SQLEnforcerEvaluationSQLA(USER_CONTEXT context, const char*    sqltext, SqlException * pExc);
  ENFORCER_API int  SQLEnforcerWriteLogA(int level, const char* msg);
  ENFORCER_API bool SQLEnforcerQueryMetadata_cb(USER_CONTEXT context, QueryMetadataFunc func);
//}






#endif // SQLENFORCER_EXPORT_H 