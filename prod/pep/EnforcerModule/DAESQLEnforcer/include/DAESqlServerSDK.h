#ifndef DAESQLSERVERSDK_H
#define DAESQLSERVERSDK_H


#ifndef _WIN32
#define DAE_SDK_API __attribute__((visibility("default")))
#else

#ifdef DAE_SDK_EXPORT
#define DAE_SDK_API  __declspec(dllexport)
#else 
#define DAE_SDK_API  __declspec(dllimport)
#endif //EMDB_SDK_EXPORT
#endif //WIN32

namespace DAESqlServer{

static const char* KEY_SERVICE_NAME = "SERVICE_NAME";//database name
static const char* KEY_OWNER = "OWNER"; //user name
static const char* KEY_AUTH_SID = "AUTH_SID"; //auth sid
static const char* KEY_CLIENT_APP_NAME = "APP_NAME"; //client name
static const char* KEY_CLIENT_HOST_NAME = "COMPUTER_NAME"; //client host name
static const char* KEY_CLIENT_IP = "USER_IP"; //client ip

enum class DAE_ResultCode{ DAE_ENFORCED, DAE_IGNORE, DAE_FAILED };

struct DAE_UserProperty{ 
    const char * _key;
    const char * _value;
};
struct DAE_BindingParam {
    enum class DataType{
        ORAVarchar2,//string
        ORAInteger,
        ORADecimal,
        ORAChar,
        ORADate,
        ORATimeStamp,
        ORAOthers
    } _type;
    const char* _value;
};
typedef void* DAEResultHandle;
extern "C" {

    //DAE_SDK_API void    EnforcerCheck(
    //    DAEResultHandle result;
    //);
    DAE_SDK_API bool DAESqlServerInit(const char *worker_name);

    DAE_SDK_API bool DAEIsSqlTextFormat(const char *sql);

    DAE_SDK_API bool DAENewResult(DAEResultHandle* output_result);

    DAE_SDK_API bool DAEFreeResult(DAEResultHandle result);

    DAE_SDK_API bool DAEGetResult(DAEResultHandle result, DAE_ResultCode* output_code, const char** output_detail, const char** output_db);

    DAE_SDK_API bool    EnforceSQL_simple(
            const char *sql,   /*in*/
            const DAE_UserProperty* userattrs,   /*in*/
            const unsigned int userattrs_size,  /*in*/
            DAEResultHandle result /*in*/
    );

    DAE_SDK_API bool    EnforceSQL_simpleV2(
        const char* sql,   /*in*/
        const DAE_UserProperty* userattrs,   /*in*/
        const unsigned int userattrs_size,  /*in*/
        const DAE_BindingParam * bindingparams, /*in*/
        const unsigned int params_size,  /*in*/
        DAEResultHandle result /*in*/
    );

}

}

#endif