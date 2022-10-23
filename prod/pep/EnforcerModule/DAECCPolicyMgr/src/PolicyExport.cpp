//
// Created by jeff on 2020/3/13.
//

#include "PolicyExport.h"
#include "PolicyEngine.h"
#include "log.h"

#define BOOST_PP_STRINGIZE(text) BOOST_PP_STRINGIZE_I(text)
#define BOOST_PP_STRINGIZE_I(text) #text

//const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

//Init Policy sync
bool PolicyInit(const std::string &cchost, const std::string &ccport, const std::string &ccuser, const std::string &ccpwd, const std::string &model, const std::string& tag,
        unsigned int sync_interval_seconds, S4HException & /*exc*/, WriteLog_Fun log_fun ){
    if(log_fun){
        Log::m_logFun = log_fun;
    }
    return PolicyEngine::Ins()->Init(cchost, ccport, ccuser, ccpwd, model,tag, sync_interval_seconds)==POLICY_ENGINE_SUCCESS;
}

bool  Exit(){
    return PolicyEngine::Ins()->Exit()==POLICY_ENGINE_SUCCESS;
}

void UpdateSyncInterval(unsigned int iseconds){
    PolicyEngine::Ins()->UpdateSyncInterval(iseconds);
}

bool GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc){
    return PolicyEngine::Ins()->GetTablePolicyinfo(tbs_map, exc);
}

bool IsNeedEnforcer(const std::string& dbname, const std::string& schema, const std::string& table, EnumAction act, S4HException& exc) {
    return PolicyEngine::Ins()->IsNeedEnforcer(dbname, schema, table,act, exc);
}

void EMP_API BindUpdateNotification(const NotificationFunc& func) {
    PolicyEngine::Ins()->BindUpdateNotification(func);
}





