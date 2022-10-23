#ifndef DAEENFORCERWRAPPER_H12
#define DAEENFORCERWRAPPER_H12

#include <string>
#include "DAESqlServerSDK.h"


typedef bool (*DAESqlServerInit_Func)(const char* worker_name);
typedef bool (*DAENewResult_Func)(DAESqlServer::DAEResultHandle* output_result);
typedef bool (*DAEFreeResult_Func)(DAESqlServer::DAEResultHandle result);
typedef bool (*DAEGetResult_Func)(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db);
typedef bool    (*EnforceSQL_simple_Func)(
    const char* sql,   /*in*/
    const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
    const unsigned int userattrs_size,  /*in*/
    DAESqlServer::DAEResultHandle result /*in*/
);
typedef bool    (*EnforceSQL_simpleV2_Func)(
    const char* sql,   /*in*/
    const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
    const unsigned int userattrs_size,  /*in*/
    const DAESqlServer::DAE_BindingParam* bindingparams, /*in*/
    const unsigned int params_size,  /*in*/
    DAESqlServer::DAEResultHandle result /*in*/
);

class DAEEnforcerMgr
{
public:
    static DAEEnforcerMgr* Instance();
private:
    DAEEnforcerMgr();

public:
    bool DAESqlServerInit(const char* worker_name) const throw();
    bool DAENewResult(DAESqlServer::DAEResultHandle* output_result) const throw();
    bool DAEFreeResult(DAESqlServer::DAEResultHandle result) const throw();
    bool DAEGetResult(DAESqlServer::DAEResultHandle result, DAESqlServer::DAE_ResultCode* output_code, const char** output_detail, const char** output_db) const throw();
    bool EnforceSQL_simpleV2(
        const char* sql,   /*in*/
        const DAESqlServer::DAE_UserProperty* userattrs,   /*in*/
        const unsigned int userattrs_size,  /*in*/
        const DAESqlServer::DAE_BindingParam* bindingparams, /*in*/
        const unsigned int params_size,  /*in*/
        DAESqlServer::DAEResultHandle result /*in*/
        ) const throw();

    bool IsValid() const;

private:
    bool LoadEnforcer();
    void Clear();

private:
    DAESqlServerInit_Func m_pfuncDAESqlServerInit;
    DAENewResult_Func m_pfuncDAENewResult;
    DAEFreeResult_Func m_pfuncDAEFreeResult;
    DAEGetResult_Func m_pfuncDAEGetResult;
    EnforceSQL_simpleV2_Func m_pfuncEnforceSQL_simpleV2;
};

#endif 