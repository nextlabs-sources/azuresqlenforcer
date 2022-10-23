
#include "PolicyEngine.h"
#include "Policy.h"
#include "Handle.h"
#include <shared_mutex>
#include <thread>
#include <log.h>
#include <chrono>
#include "patch.h"
#include "TalkWithCC.h"
#include "log.h"
#include "PolicyModelList.h"
#include <windows.h>
#include <exception>
#include <boost/algorithm/string.hpp>
#include "nanodbc/nanodbc.h"

#pragma warning(push)
#pragma warning(disable: 5045)

PolicyEngine *PolicyEngine::_ins = nullptr;
bool PolicyEngine::_running_flag = false;

std::shared_timed_mutex __mutex;

PolicyEngine *PolicyEngine::Ins() {
    if (_ins == nullptr) _ins = new PolicyEngine();
    return _ins;
}

class MutexMgr{
    public:
    MutexMgr(HANDLE hMutex){
        _mutex = hMutex;
        WaitForSingleObject(_mutex,  INFINITE);  
    }

    ~MutexMgr(){
        ReleaseMutex(_mutex);
    }

    private:
    HANDLE _mutex;
};

PolicyEngineReturn PolicyEngine::Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& model, const std::string& tag,
                        unsigned int sync_interval_seconds) {
    if (cchost.empty() || ccport.empty() || ccuser.empty() || ccpwd.empty() || model.empty()) {
        Log::WriteLog(log_error, "PolicyEngine::Init, cc config is empty!");
        return POLICY_ENGINE_FAIL;
    }
    if (Ins()->_running_flag) return POLICY_ENGINE_SUCCESS;
    Ins()->_cchost = cchost;
    Ins()->_ccport = ccport;
    Ins()->_ccuser = ccuser;
    Ins()->_ccpwd = ccpwd;
    //Ins()->_tag = tag;
    Ins()->_model_name = model;
    Ins()->_tag = tag;
    if (sync_interval_seconds < 60) 
        Ins()->_sync_interval_seconds = 60;
    else 
        Ins()->_sync_interval_seconds = sync_interval_seconds;
    Ins()->_policy_hash = 0;
    //if (POLICY_ENGINE_SUCCESS != Ins()->Update()) return POLICY_ENGINE_CCCONNECT_ERROR;

    //get policy save file
    if (Ins()->_bfile) {
        Ins()->_MutexPolicyFile = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            "NXL_MUTEX_POLICYFILE");             // named mutex
        if (NULL == Ins()->_MutexPolicyFile) {
            Log::WriteLog(log_error, "PolicyEngine::Init, create mutex failed. lasterror:%d", GetLastError());
            return POLICY_ENGINE_FAIL;
        }

        const std::string strProgDataFolder = CommonFun::GetProgramDataFolder();
        Ins()->_strPolicySaveFile = strProgDataFolder + "policy";
        Ins()->_strPolicyModelSaveFile = strProgDataFolder + "policy_models";
        Log::WriteLog(log_info, "PolicyEngine::Init policy file:%s", Ins()->_strPolicySaveFile.c_str());
        Log::WriteLog(log_info, "PolicyEngine::Init policy file:%s", Ins()->_strPolicyModelSaveFile.c_str());
    }

       
    if (Ins()->Update(false) != POLICY_ENGINE_SUCCESS) {
        return POLICY_ENGINE_FAIL;
    }

    Ins()->_running_flag = true;
    std::thread thread(PolicyEngine::Sync);
    thread.detach();
    return POLICY_ENGINE_SUCCESS;
}

bool PolicyEngine::GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc){
    UNREFERENCED_PARAMETER(exc);
    std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
    tbs_map.insert(_tbs_map.begin(), _tbs_map.end());
     return true;
}

bool PolicyEngine::IsNeedEnforcer(const std::string& dbname, const std::string& schema, const std::string& table, EnumAction act, S4HException& exc) {
    UNREFERENCED_PARAMETER(exc);
    std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);

    if (_tbs_map.empty()) {
      return true;
    }

    auto it = _tbs_map.find(table);
    if (it != _tbs_map.end()) {
        bool bret = (0 != (it->second._actions & act));
        if (bret == false && act == EnumAction::ACT_UPDATE) {// if select hav mask field  need to deny update this field
            bret = (0 != (it->second._actions & EnumAction::ACT_SELECT));
        }
        return bret;
    }
    return false;
}

void PolicyEngine::BindUpdateNotification(const NotificationFunc& func) {
    if (func) {
        _notification = func;
    }
}

PolicyEngineReturn PolicyEngine::Exit() {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    Ins()->_running_flag = false;
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngine::~PolicyEngine() {

}

PolicyEngine::PolicyEngine():_sync_interval_seconds(0), _policy_hash(0){
#ifdef _WIN32
    _MutexPolicyFile = nullptr;
#endif
}



PolicyEngineReturn PolicyEngine::DownloadPolicies() {
    std::vector<Json::Value> jsons;
    _pmodels.ClearModels();
    _pmodels.ClearTalk();
    _policys.clear();

    bool r = PolicyHelper::DownloadPolicys(_cchost, _ccport, _ccuser, _ccpwd, _model_name, _tag, jsons, _pmodels);
    if (!r) {
        //Log::WriteLog(log_error, "PolicyEngine::DownloadPolicies return for download policy failed.");
        return POLICY_ENGINE_FAIL;
    }

    //get target policy model
    PolicyModel model;
    bool bfindModel = _pmodels.AddPmByName(_model_name, model);
    if (!bfindModel) {
        //free talk
        delete(_pmodels.GetTalk());
        _pmodels.SetTalk(NULL);
        Log::WriteLog(log_error, "Not find policy model by model name in configuration");
        return POLICY_ENGINE_FAIL;
    }

    Log::WriteLog(log_info, "PolicyEngine::DownloadPolicies: json size:%u", jsons.size());



    auto itJson = jsons.begin();
    while (itJson != jsons.end()) {
        Json::Value& jsonValue = *itJson;

        //get obligation
        Json::Value Obligations;
        //        if(!jsonValue["data"].isNull()){
        //            if(!jsonValue["data"]["allowObligations"].isNull()){
        //                Obligations.append( jsonValue["data"]["allowObligations"]);
        //            }
        //            if(!jsonValue["data"]["denyObligations"].isNull()){
        //                Obligations.append(jsonValue["data"]["denyObligations"]);
        //            }
        //        }
        if (!jsonValue["allowObligations"].isNull()) {
            Obligations.append(jsonValue["allowObligations"]);
        }
        if (!jsonValue["denyObligations"].isNull()) {
            Obligations.append(jsonValue["denyObligations"]);
        }


        bool bfindObs = false;

        for (unsigned int i = 0; i < Obligations.size(); ++i)
        {

            Json::Value& json_obg = Obligations[i];
            for (unsigned int j = 0; j < json_obg.size(); ++j)
            {
                Json::Value& sub = json_obg[j];
                int modelid = sub["policyModelId"].asInt();
                if (model._id == modelid)
                {
                    bfindObs = true;
                    break;
                }
            }
            if (bfindObs) {
                break;
            }
        }
        std::string sallow = jsonValue["effectType"].asString();
        if (sallow.compare("deny") == 0)
            bfindObs = true;

        if (bfindObs) {
            //contains obligation what we want, //save policy to file
            Json::StreamWriterBuilder builder;
            builder.settings_["indentation"] = "";
            std::string strJString = Json::writeString(builder, jsonValue);
            strJString += "\n";

            _policys.push_back(strJString);

            //Log::WriteLog(log_error, "PolicyEngine::DownloadPolicyToFile: write to file:%u, errno:%d", nWrite, errno );
        }

        itJson++;
    }
    {
        {
            //get all needed policy model
            PolicyModel model;
            bool bfind = _pmodels.AddPmByName(_model_name, model);
            if (!bfind) {
                Log::WriteLog(log_error, "DownloadPolicies not find policy model by model name in configuration");
                //free
                delete(_pmodels.GetTalk());
                _pmodels.SetTalk(NULL);
                return POLICY_ENGINE_FAIL;
            }
        }

        {
            PolicyModel model;
            bool bfind = _pmodels.AddPmByName("host", model);
            if (!bfind) {
                Log::WriteLog(log_error, "DownloadPolicies not find policy model host ");
                //free
                delete(_pmodels.GetTalk());
                _pmodels.SetTalk(NULL);
                return POLICY_ENGINE_FAIL;
            }
        }

        {
            PolicyModel model;
            bool bfind = _pmodels.AddPmByName("user", model);
            if (!bfind) {
                Log::WriteLog(log_error, "DownloadPolicies not find policy model user ");
                //free
                delete(_pmodels.GetTalk());
                _pmodels.SetTalk(NULL);
                return POLICY_ENGINE_FAIL;
            }
        }

        //{
        //    PolicyModel model;
        //    bool bfind = _pmodels.AddPmByName("application", model);
        //    if (!bfind) {
        //        Log::WriteLog(log_error, "DownloadPolicies not find policy model application ");
        //        //free
        //        delete(_pmodels.GetTalk());
        //        _pmodels.SetTalk(NULL);
        //        return POLICY_ENGINE_FAIL;
        //    }
        //}
    }

    //free
    delete(_pmodels.GetTalk());
    _pmodels.SetTalk(NULL);
    return POLICY_ENGINE_SUCCESS;

}


void PolicyEngine::SavePolicyToFile(HANDLE hFilePolicy)
{
    if (_bfile) {
        //save policy models
        SavePolicyModelsToFile(_pmodels);

        //clear file when we download policy success.
        LARGE_INTEGER nNewPos = { 0 };
        SetFilePointerEx(hFilePolicy, nNewPos, &nNewPos, FILE_BEGIN);
        SetEndOfFile(hFilePolicy);
        for (auto& it : _policys) {
            DWORD dwWritten = 0;
#pragma warning(push)
#pragma warning(disable : 4267) // conversion from 'size_t' to 'DWORD', possible loss of data           
            WriteFile(hFilePolicy, it.c_str(), it.size(), &dwWritten, NULL);
#pragma warning(pop)
        }
        FlushFileBuffers(hFilePolicy);
    }
}

void PolicyEngine::SavePolicyModelsToFile(PolicyModelList& pmodels)
{
    //save
    FILE* pFilePM = fopen(_strPolicyModelSaveFile.c_str(), "w");
    if (NULL==pFilePM){
        Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile open save file failed. file:%s, errno:%d", _strPolicyModelSaveFile.c_str(), errno);
        return;
    }


    const std::vector<PolicyModel>& allPolicyModel = pmodels.GetAllPolicyModels();
    for (auto itPM = allPolicyModel.begin(); itPM!=allPolicyModel.end(); itPM++){

        //write JSON
        fwrite(itPM->_strJson.c_str(), itPM->_strJson.length(), 1, pFilePM);
        fwrite("\n", 1, 1, pFilePM);

        //write PreAttributeJSON
        fwrite(itPM->_strPreAttributeJson.c_str(), itPM->_strPreAttributeJson.length(), 1, pFilePM);
        fwrite("\n", 1, 1, pFilePM);
    }


    //close
    fflush(pFilePM);
    fclose(pFilePM);
    pFilePM = NULL;
}

void PolicyEngine::ReadPolicyModelsFromFile(PolicyModelList& pmList)
{
    //open file
    FILE* pFilePM = fopen(_strPolicyModelSaveFile.c_str(), "r");
    if (NULL==pFilePM){
        Log::WriteLog(log_error, "PolicyEngine::ReadPolicyModelsFromFile open save file failed. file:%s, errno:%d", _strPolicyModelSaveFile.c_str(), errno);
        return;
    }

    //read line by line
    const int nMaxBufSize = 1 * 1024 * 1024; //we asssume the max size of a policy model is 1M 
    char* pLinePolicyModel= new char[nMaxBufSize];
    char* pLinePreAttribute= new char[nMaxBufSize];
    while (!feof(pFilePM)){

        char* pDataPolicyModel = fgets(pLinePolicyModel, nMaxBufSize-1, pFilePM);//A terminating null character is automatically appended after the characters copied to pLine.
        if (pDataPolicyModel)  {
           
            //read pre-attribute line
            char* pDataPreAttribute = NULL;
            if (!feof(pFilePM)) {
               pDataPreAttribute = fgets(pLinePreAttribute, nMaxBufSize-1, pFilePM);
            }
             
            //construct policy model object
            PolicyModel pm;
            bool bValidJson = pm.ParseFromJson(pDataPolicyModel);
            if (bValidJson){
                if(pDataPreAttribute && (pm._type==PolicyModel::PM_SUB_USER ||
                                         pm._type==PolicyModel::PM_SUB_HOST || 
                                         pm._type==PolicyModel::PM_SUB_APP)){
                   Log::WriteLog(log_info, "PolicyEngine::ReadPolicyModelsFromFile, added pre-attribute to policy model, name:%s", pm._name.c_str() );
                   pm.AddPreAttribute(pDataPreAttribute);
                }
                
                //added to list
                Log::WriteLog(log_info, "PolicyEngine::ReadPolicyModelsFromFile, added Policy model to list, name:%s", pm._name.c_str() );
                pmList.AddPMStatic(pm);
            }     
        }
        
    }
    
    //free
    delete[] pLinePolicyModel; pLinePolicyModel=NULL;
    delete[] pLinePreAttribute; pLinePreAttribute=NULL;

    fclose(pFilePM);
    pFilePM = NULL;
}

void PolicyEngine::ParsePolicies(const std::vector<std::string> & vec_policys,  PolicyModelList& pmList) {

    TablePolicyInfoMap tbs_map;

    for (auto& strJson : vec_policys) {
        //construct json
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        Json::Value jsonValue;
        std::string strErrors;
        bool parsingSuccessful = reader->parse(strJson.c_str(), strJson.c_str() + strJson.size(), &jsonValue, &strErrors);
        if (!parsingSuccessful) {
            Log::WriteLog(log_info, "PolicyEngine::ParsePolicies: parse json failed.");
            continue;
        }

        Policy policy;
        if (policy.ParseFromJson(jsonValue, &pmList) == POLICY_ENGINE_SUCCESS)
        {
            S4HException ex;
            //                policy.GetMaskInfos(map, ex);
            //                policy.GetResTables(tables,ex);
            //                for(auto it:tables){
            //                     if(dup_map.end()==dup_map.find(it)){
            //                        DenyUpdateItemMap * dupi_map = new DenyUpdateItemMap;
            //                        dup_map.insert(std::make_pair(it, dupi_map));
            //                        policy.GetUpdateControl(*dupi_map, ex);
            //                    }
            //                }
            policy.GetPolicyBasicInfo(tbs_map, ex);

        }
    }

    {
        std::lock_guard<std::shared_timed_mutex> writerLock(__mutex);
        _tbs_map.swap(tbs_map);
    }

    //printf
    char szLog[1024];
    std::string strLog = "AllTablePolicyInfo:\r\n";
    for (auto itTable = _tbs_map.begin(); itTable != _tbs_map.end(); itTable++)
    {
        TablePolicyInfo& refPolicyInfo = itTable->second;
        sprintf_s(szLog, 1024, "table:%s, actions:%d,refPolicyInfo._bfilter=%d, refPolicyInfo._bmask=%d, refPolicyInfo._bdup=%d\r\n",
            itTable->first.c_str(), refPolicyInfo._actions, refPolicyInfo._bfilter, refPolicyInfo._bmask, refPolicyInfo._bdup);
        strLog += szLog;
    }
    Log::WriteLog(log_info, strLog.c_str());

}

void PolicyEngine::ReadPolicyFromFile(HANDLE hFilePolicy)
{
    std::vector<std::string> vec_policys;
   //seek to the begining of the file
   LARGE_INTEGER nNewPos = { 0 };
   SetFilePointerEx(hFilePolicy, nNewPos, &nNewPos, FILE_BEGIN);

    //get file size
    DWORD dwFileSizeLow = GetFileSize(hFilePolicy, NULL);
    const long lMaxFileSize = 10485760;//10*1024*1024 ;
    off_t bufSize = dwFileSizeLow<lMaxFileSize ?  dwFileSizeLow : lMaxFileSize;  //one policy is about 5k. so limit the file size to 10M is enough.

    if (bufSize>0){
        char* pContent = new char[(size_t)bufSize+(size_t)1];
        DWORD readCount = 0;
        BOOL bRead = ReadFile(hFilePolicy, pContent, bufSize, &readCount, NULL);
        if(!bRead){
             Log::WriteLog(log_info, "PolicyEngine::ReadPolicyFromFile failed. error=%d", GetLastError() );
        }
        Log::WriteLog(log_info, "PolicyEngine::ReadPolicyFromFile: read file count:%u", readCount);
        if (readCount>=0){
            pContent[readCount]=0;
        }

        //{//check hash value
        //    std::hash<std::string> hash_str;
        //    size_t hash_val = hash_str(std::string(pContent));
        //    if(_policy_hash == hash_val){
        //        delete[] pContent;
        //        pContent = NULL;
        //        return;
        //    } else {
        //        _policy_hash = hash_val;
        //        if (_notification) 
        //            _notification();
        //    }
        //}
        
        char* pData = pContent;
        bool bHaveNewLine = true;
        
        while (bHaveNewLine) {
            //get the line data
            std::string strJson;
            char* pNewLine = strchr(pData, '\n');
            if (pNewLine){
                pNewLine[0]=0;

                strJson = pData;

                //next line
                pData = pNewLine;
                pData++;
            }
            else{
                bHaveNewLine = false;
                strJson = pData;
            }

            if (strJson.empty()) {
                continue; //may be a emtpy line
            }
            vec_policys.push_back(strJson);

        }
        delete[] pContent;
        pContent = NULL;

    }

    PolicyModelList pmList;
    ReadPolicyModelsFromFile(pmList);
    ParsePolicies(vec_policys, pmList);
   
}



PolicyEngineReturn PolicyEngine::Update(bool anyway /*=false*/ ) {
  //  Log::WriteLog(log_trace, "PolicyEngine::Update enter.");

    std::chrono::seconds ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
    long long cur_time = ms.count();

    long long time_ = cur_time - _last_time;
    _last_time = cur_time;

    if (_sync_interval_seconds < (unsigned int)time_) {

        PolicyEngineReturn ret = DownloadPolicies();
        if (ret != POLICY_ENGINE_SUCCESS) {
            return ret;
        }

        std::string alljson;
        for (auto& strJson : _policys) {
            alljson += strJson;
            alljson += "\n";
        }

        {//check hash value
            std::hash<std::string> hash_str;
            size_t hash_val = hash_str(alljson);
            if (_policy_hash == hash_val) {
                return POLICY_ENGINE_SUCCESS;
            }
            else {
                _policy_hash = hash_val;
                if (_notification)
                    _notification();
            }
        }

        if (_bfile) {

            MutexMgr lockPolicyFile(_MutexPolicyFile);

            //open policy file
            HANDLE hFilePolicy = CreateFileA(_strPolicySaveFile.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE == hFilePolicy) {
                Log::WriteLog(log_error, "PolicyEngine::Update open policy save file failed, file:%s errno:%d", _strPolicySaveFile.c_str(), GetLastError());
                return POLICY_ENGINE_FAIL;
            }

            //check if we need to download policy
            DWORD dwFileSize = GetFileSize(hFilePolicy, NULL);
            if (INVALID_FILE_SIZE == dwFileSize) {
                Log::WriteLog(log_error, "PolicyEngine::Update get file size of policy save file failed, file:%s errno:%d", _strPolicySaveFile.c_str(), errno);
                return POLICY_ENGINE_FAIL;
            }

            SavePolicyToFile(hFilePolicy);

            //read policy from file
            ReadPolicyFromFile(hFilePolicy);
            //close file
            CloseHandle(hFilePolicy);
            hFilePolicy = INVALID_HANDLE_VALUE;
        }
        else {
            ParsePolicies(_policys, _pmodels);
        }
        return POLICY_ENGINE_SUCCESS;
    }
    else {
        return POLICY_ENGINE_SUCCESS;
    }

}

void PolicyEngine::Sync() {
    
    while (_running_flag ) {
        
        std::this_thread::sleep_for(std::chrono::seconds(Ins()->_sync_interval_seconds+5/*+5s to guarantee the policy cache file be updated next time*/));
        Ins()->Update();
    }
    delete (_ins);
    _ins = nullptr;
}

void PolicyEngine::UpdateSyncInterval(unsigned int iseconds){
    _sync_interval_seconds = iseconds;
}

#pragma warning(pop)

