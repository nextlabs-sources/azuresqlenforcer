 #ifndef POLICY_MGR_WRAPPER_H
#define POLICY_MGR_WRAPPER_H

#include "PolicyExport.h"


class CCPolicyMgrWrapper
{
    public:
    static CCPolicyMgrWrapper* Instance()
    {
        static CCPolicyMgrWrapper* pInstance = new CCPolicyMgrWrapper();
        return pInstance;
    }
    bool IsWell() { return m_bWell; }
    bool LoadPolicyMgr();
    protected:
    CCPolicyMgrWrapper(){
        m_bWell = false;
    }
    //CCPolicyMgrWrapper(const CCPolicyMgrWrapper&){}
    bool m_bWell;
    //wrapper interface
    public:
    PolicyInit_Func PolicyInit = NULL;
    GetTablePolicyinfo_Func GetTablePolicyinfo = NULL;
    Exit_Func Exit = NULL;
    UpdateSyncInterval_Func UpdateSyncInterval = NULL;
    IsNeedEnforcer_Func IsNeedEnforcer = NULL;
    BindUpdateNotification_Func BindUpdateNotification = NULL;
};


#endif 