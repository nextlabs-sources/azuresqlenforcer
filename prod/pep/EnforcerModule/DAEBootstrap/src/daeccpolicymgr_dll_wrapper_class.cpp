#include "daeccpolicymgr_dll_wrapper_class.h"

#include "commfun.h"

namespace daebootstrap {

bool DaeccpolicymgrDllWrapper::Load() noexcept {
    const auto module_handle = CommonFun::LoadShareLibrary("DAECCPolicyMgr.dll");
    if (nullptr == module_handle) {
        return false;
    }

    this->PolicyInit = static_cast<PolicyInit_Func>(CommonFun::GetProcAddress(module_handle, "PolicyInit"));
    this->GetTablePolicyinfo = static_cast<GetTablePolicyinfo_Func>(CommonFun::GetProcAddress(module_handle, "GetTablePolicyinfo"));

    return nullptr != this->PolicyInit &&
           nullptr != this->GetTablePolicyinfo;
}

} // namespace daebootstrap