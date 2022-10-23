#pragma once

#ifndef __SQLENFORCE_HELPER_H__
#define __SQLENFORCE_HELPER_H__

#include <string>
#include "DAESqlServerSDK.h"

typedef bool(*DAESqlServerInit_Func)(const char* worker_name);
typedef bool(*DAEIsSqlTextFormat)(const char* sql);
typedef bool(*DAENewResult_Func)(DAESqlServer::DAEResultHandle* output_result);
typedef bool(*DAEFreeResult_Func)(DAESqlServer::DAEResultHandle result);
typedef bool(*DAEGetResult_Func)(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db);
typedef bool(*EnforceSQL_simple_Func)(
    const char* sql,   /*in*/
    const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
    const unsigned int userattrs_size,  /*in*/
    DAESqlServer::DAEResultHandle result /*in*/
    );
typedef bool(*EnforceSQL_simpleV2_Func)(
    const char* sql,   /*in*/
    const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
    const unsigned int userattrs_size,  /*in*/
    const DAESqlServer::DAE_BindingParam* bindingparams, /*in*/
    const unsigned int params_size,  /*in*/
    DAESqlServer::DAEResultHandle result /*in*/
    );

class SQLEnforceMgr
{
public:
    static SQLEnforceMgr* GetInstance();
    static void Release();

    ~SQLEnforceMgr();

    bool DAEInit(const std::string& worker) throw();
    bool DAEIsSqlText(const wchar_t* wsql);
    bool DAENewRes(DAESqlServer::DAEResultHandle* output_result) throw();
    bool DAEFreeRes(DAESqlServer::DAEResultHandle result) throw();
    bool DAEGetRes(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db) throw();
    bool EnforceSimple(
        const char* sql,   /*in*/
        const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
        const unsigned int userattrs_size,  /*in*/
        DAESqlServer::DAEResultHandle result /*in*/) throw();

    bool EnforceSimpleV2(
        const char* sql,   /*in*/
        const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
        const unsigned int userattrs_size,  /*in*/
        const DAESqlServer::DAE_BindingParam* bindingparams, /*in*/
        const unsigned int params_size,  /*in*/
        DAESqlServer::DAEResultHandle result /*in*/) throw();

private:
    SQLEnforceMgr();

    void Load();
    
private:
    static SQLEnforceMgr* m_pThis;

    DAESqlServerInit_Func   m_InitF;
    DAEIsSqlTextFormat      m_isSqlText;
    DAENewResult_Func       m_newResF;
    DAEFreeResult_Func      m_freeResF;
    DAEGetResult_Func       m_getResF;
    EnforceSQL_simple_Func  m_enforceSimpleF;
    EnforceSQL_simpleV2_Func    m_enforceSimple2F;
};

#endif