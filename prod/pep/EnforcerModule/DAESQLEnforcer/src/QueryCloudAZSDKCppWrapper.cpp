#include "QueryCloudAZSDKCppWrapper.h"
#include "commfun.h"
#include "DAELog.h"
#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

QueryCloudAZSDKCppWrapper::QueryCloudAZSDKCppWrapper()
    {
        QueryCloudAZInit = NULL;
        CheckSingleResource = NULL;
        CheckMultiResource = NULL;
        CreatePolicyRequest = NULL;
        CreateCEAttr = NULL;
        FreePolicyRequest = NULL;
        FreePolicyResult = NULL;
        FreeCEAttr = NULL;
    }

    
bool load_lib(const std::string& module_name) {
    std::string module_path = CommonFun::GetSSLModule(module_name);
    void *module = CommonFun::LoadShareLibrary(module_path.c_str());
    if (module==NULL) {
        theLog->WriteLog(log_fatal, LOAD_MODULE_FAILED, module_path.c_str(), CommonFun::ShareLibraryError().c_str());
        return false;
    }
    return true;
}
bool QueryCloudAZSDKCppWrapper::LoadQueryCloudAZSDKCpp()
{
#ifdef WIN32
    if (!load_lib("libeay32"))
        return false;
    if (!load_lib("ssleay32"))
        return false;
#endif
    std::string strPath = CommonFun::GetQueryCloudAZSDKCppModule();
    //load library
    void* hModule = CommonFun::LoadShareLibrary(strPath.c_str());
    if (hModule == NULL) {
        theLog->WriteLog(log_fatal, LOAD_MODULE_FAILED, strPath.c_str(), CommonFun::ShareLibraryError().c_str());
        return false;
    }

    //get function address
    this->QueryCloudAZInit      = (QueryCloudAZInitFun)     CommonFun::GetProcAddress(hModule, "QueryCloudAZInit");
    this->CheckSingleResource   = (CheckSingleResourceFun)  CommonFun::GetProcAddress(hModule, "CheckSingleResource");
    this->CheckMultiResource    = (CheckMultiResourceFun)   CommonFun::GetProcAddress(hModule, "CheckMultiResource");
    this->CreatePolicyRequest   = (CreatePolicyRequestFun)  CommonFun::GetProcAddress(hModule, "CreatePolicyRequest");
    this->CreateCEAttr          = (CreateCEAttrFun)         CommonFun::GetProcAddress(hModule, "CreateCEAttr");
    this->FreePolicyRequest     = (FreePolicyRequestFun)    CommonFun::GetProcAddress(hModule, "FreePolicyRequest");
    this->FreePolicyResult      = (FreePolicyResultFun)     CommonFun::GetProcAddress(hModule, "FreePolicyResult");
    this->FreeCEAttr            = (FreeCEAttrFun)           CommonFun::GetProcAddress(hModule, "FreeCEAttr");

    return QueryCloudAZInit != nullptr &&
        CheckSingleResource != nullptr  &&
        CheckMultiResource != nullptr &&
        CreatePolicyRequest != nullptr &&
        CreateCEAttr != nullptr &&
        FreePolicyRequest != nullptr &&
        FreePolicyResult != nullptr &&
        FreeCEAttr != nullptr;
}