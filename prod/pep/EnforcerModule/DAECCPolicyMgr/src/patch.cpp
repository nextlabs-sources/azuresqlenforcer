#include "patch.h"
#include "TalkWithCC.h"
#include "PolicyModelList.h"
#include "policy_analyze.h"
#include "log.h"
#define KEY_ACTION_COMPONENTS           "actionComponents"
#define KEY_SUBJECT_COMPONENTS          "subjectComponents"
#define KEY_FROM_RESOURCE_COMPONENTS    "fromResourceComponents"
#define KEY_COMPONENTS                  "components"
#define KEY_ID                          "id"
#define KEY_ACTIONS                     "actions"
#define KEY_CONDITIONS                  "conditions"
#define KEY_POLICY_MODEL_ID             "policy_model_id"
#define KEY_TYPE                        "type"
#define KEY_CONDITIONS                  "conditions"
#define KEY_ATTRIBUTE                   "attribute"
#define KEY_ATTRIBUTE_TYPE              "attribute_type"

bool PolicyHelper::DownloadPolicys(const std::string& service, const std::string& port, const std::string& user, const std::string& pwd, const std::string& model_name, const std::string &tag,
                     std::vector<Json::Value>& rpolicys, PolicyModelList& rsymbols) {
    {

        TalkWithCC *talk = TalkWithCC::MakeTalk(service, port, user, pwd);
        PolicyModelList syms({}, talk);
        if (talk == nullptr) {
            //Log::WriteLog(log_error, "DownloadPolicys TalkWithCC::MakeTalk return false" );
            return false;
       } 

        std::vector<std::string> pids;
        bool r = talk->SearchPolicyIDsByTag(tag, pids);
        if (!r) {
            Log::WriteLog(log_info, "No policy was found by tag:%s", tag.c_str());
            delete (talk);
            return false;
        }
        std::vector<Json::Value> policys;
        for (auto it : pids) {
            std::string jstr;
            r = talk->SearchPolicyByID(it, jstr);
             if (!r) break; 
            Json::Value value ;
            get_policy_jsvalue_from_string(jstr, value);
            policys.push_back(value);
        }
        
        std::map<AID, std::vector<Json::Value *>> action_patch_list;
        r = PolicyPatchAction(policys,  action_patch_list);
        if (!r) {
            Log::WriteLog(log_info, "DownloadPolicys return false for PolicyPatchAction");
            delete (talk);
            return false;
        }
        
        std::vector<Json::Value> action_from_http;
        for (auto it : action_patch_list) {
            std::string jstr;
            r = talk->SearchComponentByID(std::to_string(it.first), jstr);
            if (!r) break;
            Json::Value value ;
            get_action_jsvalue_from_string(jstr, value);
            action_from_http.push_back(value);

        }
        if (action_from_http.size() != action_patch_list.size()) {
            delete (talk);
             Log::WriteLog(log_info, "DownloadPolicys return false for action_from_http.size:%d, action_patch_list.size:%d",
              action_from_http.size(), action_patch_list.size());
            return false;
        }
        ComponentApplyPatch(action_patch_list, action_from_http); // copy action value

        { //check policy model
            PolicyModel model;
            bool bfind = syms.AddPmByName(model_name,model);
            if(!bfind){
                delete (talk);
                 Log::WriteLog(log_info, "DownloadPolicys return false for AddPmByName, name:%s", model_name.c_str() );
                return false;
            }
            std::vector<Json::Value>::iterator itp = policys.begin();
            while(itp!=policys.end()){

                bool erase = false;
                Json::Value& act = (*itp)["actionComponents"];
                if(!act.isArray()){
                    erase = true;
                } else {
                    for (auto it = act.begin(); it != act.end(); ++it) {
                        Json::Value& com = (*it)["components"];
                        if(!com.isArray()){
                            continue;
                        }
                        for (auto itc = com.begin(); itc != com.end(); ++itc) {

                            uint64_t modl_id =  (*itc)["policy_model_id"].asInt();
                            if(model._id != modl_id){
                                erase = true;
                                break;
                            }
                        }
                    }
                }


                if(erase){//erase
                    itp = policys.erase(itp);
                }else{
                    ++itp;
                }
            }
        }

        std::map<CID, std::vector<Json::Value *>> component_patch_list;
        r = PolicyPatchResAndSub(policys,  component_patch_list);
        if (!r) {
            delete (talk);
            Log::WriteLog(log_info, "DownloadPolicys return false for PolicyPatchResAndSub" );
            return false;
        }
        std::vector<Json::Value> component_from_http;
        for (auto it : component_patch_list) {
            std::string jstr;
            r = talk->SearchComponentByID(std::to_string(it.first), jstr);
            if (!r) break;
            Json::Value value;
            get_component_jsvalue_from_string(jstr, value);
            component_from_http.push_back(value);
        }

        if (component_from_http.size() != component_patch_list.size()) {
            delete (talk);
            Log::WriteLog(log_info, "DownloadPolicys return false for component_from_http.size:%d, component_patch_list.size:%d", 
            component_from_http.size(),  component_patch_list.size());
            return false;
        }
        ComponentApplyPatch(component_patch_list, component_from_http);// copy from value
        

        std::set<uint64_t > pmids;
        ComponentAnalyzePolicyModel(component_from_http, pmids);
        for (auto& it : pmids) {
            PolicyModel pm;
            if (!syms.CheckExist(it, pm))  {
                bool bRet = syms.AddPmByID(it, pm);
                if (!bRet) break;
            }    
        }
        rpolicys = policys;
        rsymbols = syms;
        
        return true;
    }
//    TalkWithCC *talk = TalkWithCC::MakeTalk(service, port, user, pwd);
//    if (talk == nullptr) return false;
//    std::vector<std::string> pids;
//    bool r = talk->SearchPolicyIDsByTag(tag, pids);
//    if (!r) {
//        delete (talk);
//        return false;
//    }
//    std::vector<Json::Value> policys;
//    for (auto it : pids) {
//        std::string jstr;
//        r = talk->SearchPolicyByID(it, jstr);
//        assert(r);
//        Json::CharReaderBuilder builder;
//        Json::CharReader *pread = builder.newCharReader();
//        Json::Value root;
//        if (!pread->parse(jstr.c_str(), jstr.c_str() + jstr.length(), &root, nullptr)) {
//            delete (pread);
//            printf("json string is incorrect");
//            continue ;
//        }
//        delete (pread);
//        pread = nullptr;
//
//        rpolicys.push_back(root);
//
//    }
//    rsymbols.SetTalk(talk);
//
//    return true;
}

bool PolicyHelper::PolicyPatchAction(std::vector<Json::Value>& policy_roots, std::map<AID, std::vector<Json::Value *>> &action_patch_list){
    for (auto& it : policy_roots) {
        {
            assert(it.isMember(KEY_ACTION_COMPONENTS));
            auto &action = it[KEY_ACTION_COMPONENTS];
            for (unsigned i = 0; i < action.size(); ++i) {
                auto &action_it = action[i];
                assert(action_it.isMember(KEY_COMPONENTS));
                auto &components = action_it[KEY_COMPONENTS];
                for (unsigned j = 0; j < components.size(); ++j) {
                    auto &component = components[j];
                    assert(component.isMember(KEY_ID));
                    assert(component.isMember(KEY_ACTIONS));
                    action_patch_list[component[KEY_ID].asInt()].push_back(&component);
                }
            }
        }
    }
    return true;
}
bool PolicyHelper::PolicyPatchResAndSub(std::vector<Json::Value>& policy_roots, std::map<AID, std::vector<Json::Value *>> &component_patch_list){
    for (auto& it : policy_roots) {
        {
            assert(it.isMember(KEY_SUBJECT_COMPONENTS));
            auto &subject = it[KEY_SUBJECT_COMPONENTS];
            for (unsigned i = 0; i < subject.size(); ++i) {
                auto &action_it = subject[i];
                assert(action_it.isMember(KEY_COMPONENTS));
                auto &components = action_it[KEY_COMPONENTS];
                for (unsigned j = 0; j < components.size(); ++j) {
                    auto &component = components[j];
                    assert(component.isMember(KEY_ID));
                    assert(component.isMember(KEY_CONDITIONS));
                    component_patch_list[component[KEY_ID].asInt()].push_back(&component);
                }
            }
        }
        {
            assert(it.isMember(KEY_FROM_RESOURCE_COMPONENTS));
            auto &subject = it[KEY_FROM_RESOURCE_COMPONENTS];
            for (unsigned i = 0; i < subject.size(); ++i) {
                auto &action_it = subject[i];
                assert(action_it.isMember(KEY_COMPONENTS));
                auto &components = action_it[KEY_COMPONENTS];
                for (unsigned j = 0; j < components.size(); ++j) {
                    auto &component = components[j];
                    assert(component.isMember(KEY_ID));
                    assert(component.isMember(KEY_CONDITIONS));
                    component_patch_list[component[KEY_ID].asInt()].push_back(&component);
                }
            }
        }
    }
    return true;
}

//bool PolicyHelper::PolicyAnalyzePatch(std::vector<Json::Value>& policy_roots,
//                                      std::map<CID, std::vector<Json::Value *>> &component_patch_list, std::map<AID, std::vector<Json::Value *>> &action_patch_list) {
//    for (auto& it : policy_roots) {
//        {
//            assert(it.isMember(KEY_ACTION_COMPONENTS));
//            auto &action = it[KEY_ACTION_COMPONENTS];
//            for (unsigned i = 0; i < action.size(); ++i) {
//                auto &action_it = action[i];
//                assert(action_it.isMember(KEY_COMPONENTS));
//                auto &components = action_it[KEY_COMPONENTS];
//                for (unsigned j = 0; j < components.size(); ++j) {
//                    auto &component = components[j];
//                    assert(component.isMember(KEY_ID));
//                    assert(component.isMember(KEY_ACTIONS));
//                    action_patch_list[component[KEY_ID].asInt()].push_back(&component);
//                }
//            }
//        }
//        {
//            assert(it.isMember(KEY_SUBJECT_COMPONENTS));
//            auto &subject = it[KEY_SUBJECT_COMPONENTS];
//            for (unsigned i = 0; i < subject.size(); ++i) {
//                auto &action_it = subject[i];
//                assert(action_it.isMember(KEY_COMPONENTS));
//                auto &components = action_it[KEY_COMPONENTS];
//                for (unsigned j = 0; j < components.size(); ++j) {
//                    auto &component = components[j];
//                    assert(component.isMember(KEY_ID));
//                    assert(component.isMember(KEY_CONDITIONS));
//                    component_patch_list[component[KEY_ID].asInt()].push_back(&component);
//                }
//            }
//        }
//        {
//            assert(it.isMember(KEY_FROM_RESOURCE_COMPONENTS));
//            auto &subject = it[KEY_FROM_RESOURCE_COMPONENTS];
//            for (unsigned i = 0; i < subject.size(); ++i) {
//                auto &action_it = subject[i];
//                assert(action_it.isMember(KEY_COMPONENTS));
//                auto &components = action_it[KEY_COMPONENTS];
//                for (unsigned j = 0; j < components.size(); ++j) {
//                    auto &component = components[j];
//                    assert(component.isMember(KEY_ID));
//                    assert(component.isMember(KEY_CONDITIONS));
//                    component_patch_list[component[KEY_ID].asInt()].push_back(&component);
//                }
//            }
//        }
//    }
//    return true;
//}

bool PolicyHelper::ComponentApplyPatch(const std::map<CID, std::vector<Json::Value *>> &component_patch_list, const std::vector<Json::Value>& from_http) {
    assert(from_http.size() == component_patch_list.size());
    for (auto& it : from_http) {
        assert(it.isMember(KEY_ID));
        CID cid = it[KEY_ID].asInt();
        auto fd = component_patch_list.find(cid);
        assert(fd != component_patch_list.end());
        for (auto patch : fd->second) {
            *patch = it;
        }
    }
    return true;
}

bool PolicyHelper::ComponentAnalyzePolicyModel(const std::vector<Json::Value>& components, std::set<uint64_t >& pmids) {
    for (auto& component : components) {
        assert(component.isMember(KEY_POLICY_MODEL_ID));
        pmids.insert(component[KEY_POLICY_MODEL_ID].asInt());
    }
    return true;
}
