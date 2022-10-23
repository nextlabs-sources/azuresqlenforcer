#ifndef USER_ATTR_WRAPPER_H
#define USER_ATTR_WRAPPER_H
#include "userattr_export.h"

class UserAttrWrapper
{
    public:
    static UserAttrWrapper* Instance()
    {
        static UserAttrWrapper* pInstance = new UserAttrWrapper();
        return pInstance;
    }

    bool IsWell() { return m_bWell; }

    bool LoadUserAttributeModule();

    protected:
    UserAttrWrapper(){ 
        m_bWell = false;
    }
    //UserAttrWrapper(const UserAttrWrapper&){}
    bool m_bWell;

    //wrapper interface
    public:
    UserAttrInitFun UserAttrInit = NULL;
    GetUserAttrFun GetUserAttr = NULL;
    UserAttrInit_SDKFun UserAttrInit_SDK = NULL;
    GetUserAttr_SDKFun GetUserAttr_SDK = NULL;
    AddUserAttrValue_SDKFun AddUserAttrValue_SDK = NULL;
    FreeUserAttr_SDKFun FreeUserAttr_SDK = NULL;
};

#endif 