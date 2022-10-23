#include <windows.h>
#include "SQLEnforceHelper.h"
#include "CommonFunc.h"
#include "Log.h"

SQLEnforceMgr* SQLEnforceMgr::m_pThis = NULL;

SQLEnforceMgr* SQLEnforceMgr::GetInstance()
{
    if (m_pThis == NULL)
    {
        m_pThis = new SQLEnforceMgr;
        m_pThis->Load();
    }

    return m_pThis;
}

void SQLEnforceMgr::Release()
{
    if (m_pThis)
    {
        delete m_pThis;
        m_pThis = NULL;
    }
}

SQLEnforceMgr::SQLEnforceMgr()
{
    m_InitF = nullptr;
    m_isSqlText = nullptr;
    m_newResF = nullptr;
    m_freeResF = nullptr;
    m_getResF = nullptr;
    m_enforceSimpleF = nullptr;
    m_enforceSimple2F = nullptr;
}

SQLEnforceMgr::~SQLEnforceMgr()
{

}

void SQLEnforceMgr::Load()
{
    HMODULE  hmod = LoadLibraryA("SQLEnforcer.dll");
    if (hmod == NULL) {
        LOGPRINT(CELOG_EMERG, L"Load SQLEnforcer.dll failed! err: %d", GetLastError());
        return;
    }

    m_InitF = (DAESqlServerInit_Func)GetProcAddress(hmod, "DAESqlServerInit");
    m_isSqlText = (DAEIsSqlTextFormat)GetProcAddress(hmod, "DAEIsSqlTextFormat");
    m_newResF = (DAENewResult_Func)GetProcAddress(hmod, "DAENewResult");
    m_freeResF = (DAEFreeResult_Func)GetProcAddress(hmod, "DAEFreeResult");
    m_getResF = (DAEGetResult_Func)GetProcAddress(hmod, "DAEGetResult");
    m_enforceSimpleF = (EnforceSQL_simple_Func)GetProcAddress(hmod, "EnforceSQL_simple");
    m_enforceSimple2F = (EnforceSQL_simpleV2_Func)GetProcAddress(hmod, "EnforceSQL_simpleV2");

    if (m_InitF == nullptr || m_isSqlText == nullptr || m_newResF == nullptr || m_freeResF == nullptr || m_getResF == nullptr
        || m_enforceSimpleF == nullptr || m_enforceSimple2F == nullptr)
    {
        LOGPRINT(CELOG_EMERG, L"GetProcAddress from SQLEnforcer.dll failed");
        return;
    }
}

bool SQLEnforceMgr::DAEInit(const std::string& worker) throw()
{
    if (m_InitF == nullptr) {
        LOGPRINT(CELOG_EMERG, L"DAESqlServerInit function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_InitF(worker.c_str());
}

bool SQLEnforceMgr::DAEIsSqlText(const wchar_t* wsql)
{
    if (wsql == nullptr || !wsql[0])
        return false;

    if (m_isSqlText == nullptr) {
        LOGPRINT(CELOG_EMERG, L"DAEIsSqlTextFormat function is null, please load SQLEnforcer.dll");
        return false;
    }

    std::string text = ProxyCommon::UnicodeToUTF8(wsql);
    try
    {
        return m_isSqlText(text.c_str());
    }
    catch (std::exception e)
    {
        LOGPRINT(CELOG_ERR, "DAEIsSqlTextFormat exception message: %s", e.what());
        return false;
    }
}

bool SQLEnforceMgr::DAENewRes(DAESqlServer::DAEResultHandle* output_result) throw()
{
    if (m_newResF == nullptr) {
        LOGPRINT(CELOG_EMERG, L"DAENewResult function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_newResF(output_result);
}

bool SQLEnforceMgr::DAEFreeRes(DAESqlServer::DAEResultHandle result) throw()
{
    if (m_freeResF == nullptr) {
        LOGPRINT(CELOG_EMERG, L"DAEFreeResult function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_freeResF(result);
}

bool SQLEnforceMgr::DAEGetRes(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db) throw()
{
    if (m_getResF == nullptr) {
        LOGPRINT(CELOG_EMERG, L"DAEGetResult function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_getResF(result, output_code, output_detail, output_db);
}

bool SQLEnforceMgr::EnforceSimple(
    const char* sql, 
    const DAESqlServer::DAE_UserProperty* userattrs,
    const unsigned int userattrs_size, 
    DAESqlServer::DAEResultHandle result) throw()
{
    if (m_enforceSimpleF == nullptr) {
        LOGPRINT(CELOG_EMERG, L"EnforceSQL_simple function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_enforceSimpleF(sql, userattrs, userattrs_size, result);
}

bool SQLEnforceMgr::EnforceSimpleV2(
    const char* sql, 
    const DAESqlServer::DAE_UserProperty* userattrs,
    const unsigned int userattrs_size, 
    const DAESqlServer::DAE_BindingParam* bindingparams,
    const unsigned int params_size, 
    DAESqlServer::DAEResultHandle result) throw()
{
    if (m_enforceSimple2F == nullptr) {
        LOGPRINT(CELOG_EMERG, L"EnforceSQL_simpleV2 function is null, please load SQLEnforcer.dll");
        return false;
    }

    return m_enforceSimple2F(sql, userattrs, userattrs_size, bindingparams, params_size, result);
}