#ifndef __ENFORCER_MODULE_DAE_BOOTSTRAP_QUERY_CLOUDAZ_SDK_CPP_WRAPPER_CLASS_H__
#define __ENFORCER_MODULE_DAE_BOOTSTRAP_QUERY_CLOUDAZ_SDK_CPP_WRAPPER_CLASS_H__

#include <functional>

#include "IQueryCloudAZ.h"

namespace daebootstrap {

class QueryCloudAzSdkCppWrapper {
public:
	using QueryCloudAZInitFun_t = bool(const char* wszPCHost, const char* wszPcPort,
                                       const char* wszOAuthServiceHost, const char* wszOAuthPort,
                                       const char* wszClientId, const char* wszClientSecret,
                                       int nConnectionCount,
                                       const std::function<int(int lvl, const char* logStr)> &cb);
    using CheckSingleResourceFun_t = QueryStatus(const IPolicyRequest* pcRequest, IPolicyResult** pcResult);
    using CheckMultiResourceFun_t = QueryStatus(const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult);

    using CreatePolicyRequestFun_t = IPolicyRequest*();
    using CreateCEAttrFun_t = IAttributes*();

    using FreePolicyRequestFun_t = void(IPolicyRequest* pRequest);
    using FreePolicyResultFun_t = void(IPolicyResult* pResult);
    using FreeCEAttrFun_t = void(IAttributes* pAttr);

    static QueryCloudAzSdkCppWrapper& Instance() {
        static QueryCloudAzSdkCppWrapper instance{};
        return instance;
    }

    QueryCloudAzSdkCppWrapper(const QueryCloudAzSdkCppWrapper&) = delete;
    QueryCloudAzSdkCppWrapper(QueryCloudAzSdkCppWrapper&&) = delete;
    void operator=(const QueryCloudAzSdkCppWrapper&) = delete;
    void operator=(QueryCloudAzSdkCppWrapper&&) = delete;

    bool Load();

    QueryCloudAZInitFun_t* QueryCloudAZInit = nullptr;
    CheckSingleResourceFun_t* CheckSingleResource = nullptr;
    CheckMultiResourceFun_t* CheckMultiResource = nullptr;
    CreatePolicyRequestFun_t* CreatePolicyRequest = nullptr;
    CreateCEAttrFun_t* CreateCEAttr = nullptr;
    FreePolicyRequestFun_t* FreePolicyRequest = nullptr;
    FreePolicyResultFun_t* FreePolicyResult = nullptr;
    FreeCEAttrFun_t* FreeCEAttr = nullptr;

private:
    QueryCloudAzSdkCppWrapper() = default;
};

} // namespace daebootstrap

#endif __ENFORCER_MODULE_DAE_BOOTSTRAP_QUERY_CLOUDAZ_SDK_CPP_WRAPPER_CLASS_H__