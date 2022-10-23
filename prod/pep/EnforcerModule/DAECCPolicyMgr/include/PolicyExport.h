//
// Created by jeff on 2020/3/13.
//

#ifndef EMPOLICY_EXPORT_H
#define EMPOLICY_EXPORT_H


#include "StructDefine.h"
#include <functional>

#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#endif


#ifndef WIN32
    #undef EMP_API
    #define EMP_API __attribute__((visibility("default")))
#else
    #ifdef HDB_POLICYMGR_IMP
        #define EMP_API __declspec(dllexport)
    #else
        #define EMP_API __declspec(dllimport)
    #endif 
#endif

typedef bool(*PolicyInit_Func)(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& model, const std::string& tag,
                            unsigned int sync_interval_seconds, S4HException& exc, WriteLog_Fun log_fun);
typedef bool(*GetTablePolicyinfo_Func)(TablePolicyInfoMap& tbs_map, S4HException& exc);
typedef bool(*Exit_Func)();
typedef void (*UpdateSyncInterval_Func)(unsigned int iseconds);
typedef bool (*IsNeedEnforcer_Func)(const std::string& dbname, const std::string& schema, const std::string& table, EnumAction act, S4HException& exc);


typedef std::function<void()> NotificationFunc;
typedef void (*BindUpdateNotification_Func)(const NotificationFunc & func);

typedef void(*GetDefaultSchema_Func)(const std::string&, std::string&);
typedef const std::string&(*GetDbServerName_Func)();


#ifdef __cplusplus
extern "C" {                         /* Assume C declarations for C++ */
#endif

//Init Policy sync
bool EMP_API PolicyInit(const std::string &cchost, const std::string &ccport, const std::string &ccuser, const std::string &ccpwd, const std::string &model, const std::string& tag,
           unsigned int sync_interval_seconds, S4HException &exc, WriteLog_Fun log_fun=NULL);

bool EMP_API GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc);

bool EMP_API IsNeedEnforcer(const std::string& dbname, const std::string& schema, const std::string& table, EnumAction act, S4HException& exc);//todo dbname schema  no useed

bool EMP_API Exit();

void EMP_API UpdateSyncInterval(unsigned int iseconds);

void EMP_API BindUpdateNotification(const NotificationFunc& func);
#ifdef __cplusplus
}
#endif
  






#endif //EMPOLICYMGR_INTERFACE_H
