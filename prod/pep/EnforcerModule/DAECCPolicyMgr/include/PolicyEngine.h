#ifndef CLASS_POLICY_ENGINE_H
#define CLASS_POLICY_ENGINE_H

#include <vector>
#include <string>
#include "StructDefine.h"
#include "Handle.h"
#include "PolicyExport.h"
#include "PolicyModelList.h"

class Policy;

class PolicyEngine {
public:
    static PolicyEngine *Ins();
    static PolicyEngineReturn Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& model, const std::string& tag,
                            unsigned int sync_interval_seconds);
    static PolicyEngineReturn Exit();
    void UpdateSyncInterval(unsigned int iseconds);
    bool GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc);
    bool IsNeedEnforcer(const std::string& dbname, const std::string& schema, const std::string& table, EnumAction act, S4HException& exc);
    void BindUpdateNotification(const NotificationFunc& func);//update jpc result caches
    ~PolicyEngine();
private:
    PolicyEngine();
    PolicyEngineReturn Update(bool anyway = false);
    static void Sync();
#ifdef _WIN32
    PolicyEngineReturn DownloadPolicies();
    void SavePolicyToFile(HANDLE fd);
    void ReadPolicyFromFile(HANDLE fd);
    void ParsePolicies(const std::vector<std::string>& vec_policys, PolicyModelList & pmList);
#else
    void DownloadPolicyToFile(int fd);
    void ReadPolicyFromFile(int fd);
#endif

    void SavePolicyModelsToFile(PolicyModelList& pmList);

    void ReadPolicyModelsFromFile(PolicyModelList& pmList);

private:
    std::vector<std::string> _policys;
    bool _bfile = false;
    PolicyModelList _pmodels;
    long long _last_time = 0;


private:
    std::string _cchost;
    std::string _ccport;
    std::string _ccuser;
    std::string _ccpwd;
    std::string _tag;
    std::string _model_name;
    unsigned int _sync_interval_seconds;

    std::string _strPolicySaveFile;
    std::string _strPolicyModelSaveFile;

    TablePolicyInfoMap _tbs_map;

    static PolicyEngine *_ins;
    static bool _running_flag;

    size_t  _policy_hash ; // to update parser json from string

    #ifdef _WIN32
    HANDLE _MutexPolicyFile;
    #endif

    NotificationFunc _notification = NULL;
};


#endif