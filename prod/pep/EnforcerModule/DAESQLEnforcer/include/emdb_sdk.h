#ifndef EMDBEXPORT_H
#define EMDBEXPORT_H
#include "stdint.h"

#include <stdio.h>
#include <string>

#ifndef WIN32
#define EMDB_SDK_API __attribute__((visibility("default")))
#else
#define  EMDB_SDK_EXPORT
#ifdef EMDB_SDK_EXPORT
#define EMDB_SDK_API  __declspec(dllexport)
#else 
#define EMDB_SDK_API  __declspec(dllimport)
#endif //EMDB_SDK_EXPORT
#endif //WIN32

typedef void* EMDBResultHandle;
typedef void* EMDBUserCtxHandle;

enum EMDBReturn{ EMDB_SUCCESS, EMDB_ERROR /* todo */, EMDB_INVALID_HANDLE };

enum EMDBResultCode{ EMDB_BLOCK_THIS_TEXT, EMDB_USE_NEW_TEXT, EMDB_DENY_EXECUTE };

extern "C"{

    EMDB_SDK_API EMDBReturn EMDBInit(const char *module_name);

    EMDB_SDK_API EMDBReturn EMDBNewUserCtx(const char *server_name, const char *database_name, const char *schema_name, const char *dbtype, EMDBUserCtxHandle *output_user_ctx);

    EMDB_SDK_API EMDBReturn EMDBFreeUserCtx(EMDBUserCtxHandle user_ctx);

    EMDB_SDK_API EMDBReturn EMDBSetUserCtxProperty(EMDBUserCtxHandle user_ctx, const char *key, const char *value);

    EMDB_SDK_API EMDBReturn EMDBClearUserCtxProperty(EMDBUserCtxHandle user_ctx);

    EMDB_SDK_API EMDBReturn EMDBEvalSql(const char *sql, EMDBUserCtxHandle user_ctx, EMDBResultHandle result);

    EMDB_SDK_API EMDBReturn EMDBNewResult(EMDBResultHandle *output_result);

    EMDB_SDK_API EMDBReturn EMDBFreeResult(EMDBResultHandle result);

    EMDB_SDK_API EMDBReturn EMDBResultGetCode(EMDBResultHandle result, EMDBResultCode *output_code);

    EMDB_SDK_API EMDBReturn EMDBResultGetDetail(EMDBResultHandle result, const char **output_detail);

}




#include "EMMaskDef.h"

extern "C" {

EMDB_SDK_API EMDBReturn EMDBQueryMetadata_cb(EMDBUserCtxHandle user_ctx, QueryMetadataFunc func);

#ifdef _WIN32
// TODO: Change into use this one, not "EMDBQueryMetadata_cb" which is not C function.
EMDB_SDK_API EMDBReturn EMDBQueryMetadata_cb_c(EMDBUserCtxHandle user_ctx, QueryMetadataFunc_c func);
#endif

}

extern "C" {
    EMDB_SDK_API bool DAEEncryptString(const char * src, char* target, size_t &len );
    EMDB_SDK_API bool DAEDecryptString(const char * src, char* target, size_t &len );
}


extern "C" {
    EMDB_SDK_API int GetCfgUsermodeMode();
    EMDB_SDK_API unsigned GetCfgUsermodeSyncTime();
    EMDB_SDK_API void GetCfgUsermodeAdonetSqlclientConnString(char* buff, int buff_size);
    EMDB_SDK_API void GetCfgUsermodeUserInfo(char* buff, int buff_size);
}

#endif // !EMDBEXPORT_H



