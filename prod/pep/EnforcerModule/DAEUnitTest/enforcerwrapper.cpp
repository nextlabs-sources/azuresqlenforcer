#include "enforcerwrapper.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif 


#define GET_Enforcer_PROC(funname) m_pfunc##funname=(funname##_Func)GetProcAddress(hEnforcer, #funname); 

DAEEnforcerMgr* DAEEnforcerMgr::Instance()
{
    static DAEEnforcerMgr* pEnforcer = new DAEEnforcerMgr();
    return pEnforcer;
}
DAEEnforcerMgr::DAEEnforcerMgr()
{
    Clear();
    LoadEnforcer();
}

bool DAEEnforcerMgr::DAESqlServerInit(const char* worker_name) const throw()
{
    bool bRet = false;
    if (nullptr != m_pfuncDAESqlServerInit)
    {
        bRet = m_pfuncDAESqlServerInit(worker_name);
    }
    return bRet;
}
bool DAEEnforcerMgr::DAENewResult(DAESqlServer::DAEResultHandle* output_result) const throw()
{
    bool bRet = false;
    if (nullptr != m_pfuncDAENewResult)
    {
        bRet = m_pfuncDAENewResult(output_result);
    }
    return bRet;
}
bool DAEEnforcerMgr::DAEFreeResult(DAESqlServer::DAEResultHandle result) const throw()
{
    bool bRet = false;
    if (nullptr != m_pfuncDAEFreeResult)
    {
        bRet = m_pfuncDAEFreeResult(result);
    }
    return bRet;
}
bool DAEEnforcerMgr::DAEGetResult(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db) const throw()
{
    bool bRet = false;
    if (nullptr != m_pfuncDAEGetResult)
    {
        bRet = m_pfuncDAEGetResult(result, output_code, output_detail, output_db);
    }
    return bRet;
}
bool DAEEnforcerMgr::EnforceSQL_simpleV2(
    const char* sql,   /*in*/
    const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
    const unsigned int userattrs_size,  /*in*/
    const DAESqlServer::DAE_BindingParam* bindingparams, /*in*/
    const unsigned int params_size,  /*in*/
    DAESqlServer::DAEResultHandle result /*in*/
) const throw()
{
    bool bRet = false;
    if (nullptr != m_pfuncEnforceSQL_simpleV2)
    {
        bRet = m_pfuncEnforceSQL_simpleV2(sql,userattrs, userattrs_size, bindingparams, params_size, result);
    }
    return bRet;
}

bool DAEEnforcerMgr::LoadEnforcer()
{
    ::SetLastError(ERROR_SUCCESS);
    std::string strEnforcerPath = "sqlenforcer.dll";
    HMODULE hEnforcer = ::LoadLibraryA(strEnforcerPath.c_str());
    if(hEnforcer==NULL)
    {
        //NLPRINT_DEBUGLOG("ERROR Load dll:%s with error:[0x%x]", strEnforcerPath.c_str(), ::GetLastError());
        return false;
    }

    //get process address
    GET_Enforcer_PROC(DAESqlServerInit);
    GET_Enforcer_PROC(DAENewResult);
    GET_Enforcer_PROC(DAEFreeResult);
    GET_Enforcer_PROC(DAEGetResult);
    GET_Enforcer_PROC(EnforceSQL_simpleV2);

    return IsValid();
}

void DAEEnforcerMgr::Clear()
{
    m_pfuncDAESqlServerInit = NULL;
    m_pfuncDAENewResult = NULL;
    m_pfuncDAEFreeResult = NULL;
    m_pfuncDAEGetResult = NULL;
    m_pfuncEnforceSQL_simpleV2 = NULL;
}

bool DAEEnforcerMgr::IsValid() const
{
    return (m_pfuncDAESqlServerInit != NULL) &&
        (m_pfuncDAENewResult != NULL) &&
        (m_pfuncDAEFreeResult != NULL) &&
        (m_pfuncDAEGetResult != NULL) &&
        (m_pfuncEnforceSQL_simpleV2 != NULL);
}