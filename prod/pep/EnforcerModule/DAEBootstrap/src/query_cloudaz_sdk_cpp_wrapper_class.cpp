#include "query_cloudaz_sdk_cpp_wrapper_class.h"

#include "commfun.h"

namespace daebootstrap {

bool QueryCloudAzSdkCppWrapper::Load() {
    if (nullptr == CommonFun::LoadShareLibrary("LIBEAY32.dll")) {
        return false;
    }

    if (nullptr == CommonFun::LoadShareLibrary("SSLEAY32.dll")) {
        return false;
    }

    const auto path = CommonFun::GetQueryCloudAZSDKCppModule();

    const auto module_handle = CommonFun::LoadShareLibrary(path.c_str());

    if (nullptr == module_handle) {
        return false;
    }

    this->QueryCloudAZInit = static_cast<QueryCloudAZInitFun_t*>(CommonFun::GetProcAddress(module_handle, "QueryCloudAZInit"));
    this->CheckSingleResource = static_cast<CheckSingleResourceFun_t*>(CommonFun::GetProcAddress(module_handle, "CheckSingleResource"));
    this->CheckMultiResource = static_cast<CheckMultiResourceFun_t*>(CommonFun::GetProcAddress(module_handle, "CheckMultiResource"));
    this->CreatePolicyRequest = static_cast<CreatePolicyRequestFun_t*>(CommonFun::GetProcAddress(module_handle, "CreatePolicyRequest"));
    this->CreateCEAttr = static_cast<CreateCEAttrFun_t*>(CommonFun::GetProcAddress(module_handle, "CreateCEAttr"));
    this->FreePolicyRequest = static_cast<FreePolicyRequestFun_t*>(CommonFun::GetProcAddress(module_handle, "FreePolicyRequest"));
    this->FreePolicyResult = static_cast<FreePolicyResultFun_t*>(CommonFun::GetProcAddress(module_handle, "FreePolicyResult"));
    this->FreeCEAttr = static_cast<FreeCEAttrFun_t*>(CommonFun::GetProcAddress(module_handle, "FreeCEAttr"));

    return nullptr != QueryCloudAZInit &&
           nullptr != CheckSingleResource &&
           nullptr != CheckMultiResource &&
           nullptr != CreatePolicyRequest &&
           nullptr != CreateCEAttr &&
           nullptr != FreePolicyRequest &&
           nullptr != FreePolicyResult &&
           nullptr != FreeCEAttr;
}

} // namespace daebootstrap
