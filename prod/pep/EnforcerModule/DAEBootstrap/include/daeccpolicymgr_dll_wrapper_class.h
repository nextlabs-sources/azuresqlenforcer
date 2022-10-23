#ifndef __ENFORCERMODULE_DAEBOOTSTRAP_DAECCPOLICYMGR_DLL_WRAPPER_CLASS_H__
#define __ENFORCERMODULE_DAEBOOTSTRAP_DAECCPOLICYMGR_DLL_WRAPPER_CLASS_H__

#include "PolicyExport.h"

namespace daebootstrap {

class DaeccpolicymgrDllWrapper {
public:
    static DaeccpolicymgrDllWrapper& Instance() {
        static DaeccpolicymgrDllWrapper wrapper{};
        return wrapper;
    }

    bool Load() noexcept;

    DaeccpolicymgrDllWrapper(const DaeccpolicymgrDllWrapper&) = delete;
    DaeccpolicymgrDllWrapper(DaeccpolicymgrDllWrapper&&) = delete;
    DaeccpolicymgrDllWrapper& operator=(const DaeccpolicymgrDllWrapper&) = delete;
    DaeccpolicymgrDllWrapper& operator=(DaeccpolicymgrDllWrapper&&) = delete;

    PolicyInit_Func PolicyInit;
    GetTablePolicyinfo_Func GetTablePolicyinfo;

private:
    DaeccpolicymgrDllWrapper() = default;
};

} // namespace daebootstrap

#endif//__ENFORCERMODULE_DAEBOOTSTRAP_DAECCPOLICYMGR_DLL_WRAPPER_CLASS_H__
