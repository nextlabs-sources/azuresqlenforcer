#ifndef QUERYCLOUDAZSDKCPPWRAPPER_H 
#define  QUERYCLOUDAZSDKCPPWRAPPER_H 

#include "QueryCloudAZExport.h"

typedef bool (* QueryCloudAZInitFun)(const char* wszPCHost, const char* wszPcPort,
                                   const char* wszOAuthServiceHost, const char* wszOAuthPort,
                                   const char* wszClientId, const char* wszClientSecret,
                                   int nConnectionCount,
const std::function<int(int lvl, const char* logStr)>& cb );//= [](int, const char*) { return 0; }

typedef QueryStatus  (* CheckSingleResourceFun)(const IPolicyRequest* pcRequest, IPolicyResult** pcResult);
typedef QueryStatus  (* CheckMultiResourceFun) (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult);

typedef IPolicyRequest* (*CreatePolicyRequestFun)();
typedef IAttributes* (* CreateCEAttrFun)();

typedef void  (* FreePolicyRequestFun)(IPolicyRequest* pRequest);
typedef void  (* FreePolicyResultFun)(IPolicyResult* pResult);
typedef void  (* FreeCEAttrFun)(IAttributes* pAttr);

class QueryCloudAZSDKCppWrapper
{
private:
    QueryCloudAZSDKCppWrapper();
    QueryCloudAZSDKCppWrapper(const QueryCloudAZSDKCppWrapper& a){}
public:
    static QueryCloudAZSDKCppWrapper* Instance()
    {
        static QueryCloudAZSDKCppWrapper* pInstance = new QueryCloudAZSDKCppWrapper();
        return pInstance;
    }

    bool LoadQueryCloudAZSDKCpp();

    QueryCloudAZInitFun QueryCloudAZInit = nullptr;
    CheckSingleResourceFun CheckSingleResource = nullptr;
    CheckMultiResourceFun CheckMultiResource = nullptr;
    CreatePolicyRequestFun CreatePolicyRequest = nullptr;
    CreateCEAttrFun CreateCEAttr = nullptr;
    FreePolicyRequestFun FreePolicyRequest = nullptr;
    FreePolicyResultFun FreePolicyResult = nullptr;
    FreeCEAttrFun FreeCEAttr = nullptr;

};









#endif