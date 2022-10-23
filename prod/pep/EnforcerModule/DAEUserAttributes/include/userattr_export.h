
#ifndef USERATTR_EXPORT_H
#define USERATTR_EXPORT_H

#include <stdio.h>
#include <string>
#include "iuserattr.h"
struct SqlException;

#ifndef WIN32
#define USERATTR_API __attribute__((visibility("default")))
#else
#ifdef  ENFORCER_EXPORT
#define USERATTR_API  __declspec(dllexport)
#else 
#define USERATTR_API  __declspec(dllimport)
#endif //ENFORCER_EXPORT
#endif //WIN32

typedef int(*WriteLog_Fun)(int nLogLevel, const char* szMsg);
typedef bool(*UserAttrInitFun)(WriteLog_Fun logFun, int mode, const char *param1, const char *param2, const char *param3, const char * p_separator, int interval);
typedef IUserAttr*(*GetUserAttrFun)(const char* szUserName, SqlException * pExc);

typedef bool(*UserAttrInit_SDKFun)(WriteLog_Fun logFun);
typedef IUserAttr* (*GetUserAttr_SDKFun)(const char*separator,const char * mode,const char * user_attrs);
typedef bool (*AddUserAttrValue_SDKFun)(IUserAttr * pUser, const char* key, const char* value, CEAttributeType attrType);
typedef void (*FreeUserAttr_SDKFun)(IUserAttr* puser);

extern "C"
{
  USERATTR_API bool UserAttrInit(WriteLog_Fun logFun, int mode, const char *param1, const char *param2, const char *param3, const char * p_separator, int interval);
  USERATTR_API IUserAttr* GetUserAttr(const char* szUserName, SqlException * pExc);

// get default userattr point
  USERATTR_API bool UserAttrInit_SDK(WriteLog_Fun logFun);
  USERATTR_API IUserAttr* GetUserAttr_SDK(const char*separator, const char * mode, const char* user_attrs);
  USERATTR_API bool AddUserAttrValue_SDK(IUserAttr * pUser, const char* key, const char* value, CEAttributeType attrType);
  USERATTR_API void FreeUserAttr_SDK(IUserAttr* puser);
}


#endif //USERATTR_EXPORT_H 