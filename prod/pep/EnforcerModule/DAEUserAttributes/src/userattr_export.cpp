#include "userattr_export.h"
#include "userattr_mgr.h"
#include "log.h"
#ifdef _WIN32
#include "ImplQueryUserAttributeAD.h"
#endif
bool UserAttrInit(WriteLog_Fun logFun, int mode, const char *param1, const char *param2, const char *param3, const char * p_separator, int interval) {
    Log::m_logFun = logFun;
    if(theUserAttrMgr==NULL)
        theUserAttrMgr = new UserAttrMgr(mode, param1, param2, param3, p_separator, interval);
    

    return true;
}

IUserAttr* GetUserAttr(const char* szUserName, SqlException * pExc) {
    return theUserAttrMgr->GetUserAttr(szUserName,  pExc);
}

////--------------------------------------------------------------/////
bool UserAttrInit_SDK(WriteLog_Fun logFun){
    if(logFun) {
        Log::m_logFun = logFun;
        return true;
    }
    else
        return false;
}
IUserAttr* GetUserAttr_SDK(const char * separator,const char * mode, const char* user_attrs) {
    UserAttr * pRet = new UserAttr("null");
    pRet->SetSeparator(separator);
#ifdef _WIN32
    if(atoi(mode)==2){
        query_ad_attr_2_user(pRet, user_attrs);
    }
#endif
    return pRet;
}

void FreeUserAttr_SDK(IUserAttr* puser) {
    if (puser) {
        delete puser;
    }
}

bool AddUserAttrValue_SDK(IUserAttr * pUser, const char* key, const char* value, CEAttributeType attrType) {
    if (!pUser)
        return false;
    ((UserAttr*)pUser)->AddAttrWithType(key, value, attrType);
    return true;
    
}