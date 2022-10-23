#include "PolicyModelList.h"
#include "NXLHttpClient.h"
#include "TalkWithCC.h"
#include "tool.h"
#include "log.h"

PolicyModel::PM_TYPE PolicyModelList::GetPMTypeByID(uint64_t pmid) {
    PolicyModel pm1;
    if (!CheckExist(pmid, pm1)) {
        PolicyModel pm;
        bool r = AddPmByID(pmid, pm);
        if (!r) {
            return PolicyModel::PM_ERR;
        }
        return pm._type;
    } else return pm1._type;
}
PolicyModel::PM_TYPE PolicyModelList::GetPMTypeByPmname(const std::string & pm_name) {
    std::string temp = pm_name;
    transform(temp.begin(), temp.end(), temp.begin(), tolower);
    PolicyModel pm1;
    if (!CheckExist(temp, pm1)) {
        PolicyModel pm;
        bool r = AddPmByName(pm_name, pm);
        if (!r) {
            return PolicyModel::PM_ERR;
        }
        return pm._type;
    } else return pm1._type;
}
AttributeInfo::ATTR_TYPE PolicyModelList::GetAttrTypeByPmidAttrName(uint64_t pmid, const std::string& attr_name) {
    PolicyModel pm1;
    if (!CheckExist(pmid, pm1)) {
        PolicyModel pm;
        bool r = AddPmByID(pmid, pm);
        if (!r) {
            return AttributeInfo::A_ERR;
        }
        return pm.GetTypeByName(attr_name);
    } else return pm1.GetTypeByName(attr_name);
}

AttributeInfo::ATTR_TYPE PolicyModelList::GetAttrTypeByPmnameAttrName(const std::string& pm_name, const std::string& attr_name) {
    std::string temp = pm_name;
    transform(temp.begin(), temp.end(), temp.begin(), tolower);
    PolicyModel pm1;
    if (!CheckExist(temp, pm1)) {
        PolicyModel pm;
        bool r = AddPmByName(pm_name, pm);
        if (!r) {
            return AttributeInfo::A_ERR;
        }
        return pm.GetTypeByName(attr_name);
    } else return pm1.GetTypeByName(attr_name);
}

bool PolicyModelList::CheckExist(uint64_t pmid, PolicyModel& out) {
    for (auto it : _models) {
        if (it._id == pmid) {
            out = it;
            return true;
        }
    }
    return false;
}

bool PolicyModelList::CheckExist(const std::string& pm_name, PolicyModel& out) {
    for (auto it : _models) {
        if (CommonFun::StrCaseCmp(it._name.c_str(), pm_name.c_str()) == 0) {
            out = it;
            return true;
        }
    }
    return false;
}

bool PolicyModelList::AddPmByID(uint64_t pmid, PolicyModel& out) {
    if (_talk == NULL) {
        return false;
    }
    std::string value;
    bool r = _talk->SearchPolicyModelByID(std::to_string(pmid), value);
    if (!r) return false;
    bool bJson = out.ParseFromJson(value.c_str());
    if(!bJson){
        return false;
    }

    switch (out._type) {
        case PolicyModel::PM_SUB_USER:
        case PolicyModel::PM_SUB_HOST:
        case PolicyModel::PM_SUB_APP:{
            std::string jsonstr;
            _talk->SearchPolicyModelPreAttrByName(out._name, jsonstr);
            out.AddPreAttribute(jsonstr.c_str());
        } break;
        default:
            break;
    }
    if (_name2id.find(out._name) == _name2id.end()) {
        _name2id[out._name] = out._id;
    }
    _models.push_back(out);
    return true;
}

bool PolicyModelList::AddPmByName(const std::string& name, PolicyModel& out) {
    if (_talk == NULL) {
        return false;
    }
    std::string value;

    auto it = _name2id.find(name);
    if (_name2id.find(name) == _name2id.end()) {
        _talk->SearchPolicyModellist(_name2id);
        it = _name2id.find(name);
    }

    if (it != _name2id.end()) {
        uint64_t id = it->second;
        if (!CheckExist(id, out))
            AddPmByID(id, out);
    }  else {
        //name is incorrect.
        return  false;
    }
    return true;
}

void PolicyModelList::AddPMStatic(PolicyModel& pm)
{
    _name2id[pm._name] = pm._id;

    _models.push_back(pm);

}

#include <json/json.h>
#include "tool.h"
AttributeInfo::ATTR_TYPE get_attribute_type(const std::string & strtype) {
    if (CommonFun::StrCaseCmp(strtype.c_str(), "STRING") == 0) {
        return AttributeInfo::A_STRING;
    } else if (CommonFun::StrCaseCmp(strtype.c_str(), "NUMBER") == 0) {
        return AttributeInfo::A_NUMBER;
    } else if (CommonFun::StrCaseCmp(strtype.c_str(), "MULTIVAL") == 0) {
        return AttributeInfo::A_MULTI;
    } else  {
        return AttributeInfo::A_ERR;
    }
}
PolicyModel::PM_TYPE get_pm_type(const std::string & strtype,const std::string & name, int id) {
    //PM_RES, PM_SUB_USER, PM_SUB_APP, PM_SUB_HOST, PM_ERR
    if (CommonFun::StrCaseCmp(strtype.c_str(), "SUBJECT") == 0) {
        if (id == PM_USER_ID) {
            return PolicyModel::PM_SUB_USER;
        } else if (id == PM_HOST_ID) {
            return PolicyModel::PM_SUB_HOST;
        } else if (id ==PM_APPLICATION_ID) {
            return PolicyModel::PM_SUB_APP;
        } else {
            if(name.compare("user") == 0){
                return PolicyModel::PM_SUB_USER;
            } else if(name.compare("host") == 0) {
                return PolicyModel::PM_SUB_HOST;
            } else if(name.compare("application") == 0) {
                return PolicyModel::PM_SUB_APP;
            } else{
                return PolicyModel::PM_ERR;
            }
        }
    } else if (CommonFun::StrCaseCmp(strtype.c_str(), "RESOURCE") == 0) {
        return PolicyModel::PM_RES;
    } else {
        return PolicyModel::PM_ERR;
    }
}

bool PolicyModel::ParseFromJson(const char* json) {
    _strJson = json;

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(json, json + strlen(json), &root, nullptr)) {
        delete (pread);
       // printf("json string is incorrect");
        return false;
    }
    delete (pread);
    pread = nullptr;

    //check status
    std::string strStatus=root["statusCode"].asString();
    if (strStatus!="1003"){
        Log::WriteLog(log_error, "PolicyModel::ParseFromJson return status!=1003. s=%s", json);
        return false;
    }
    
    Json::Value &jsdata = root["data"];
    _id = jsdata["id"].asInt();
    _name = jsdata["shortName"].asString();
    _type = get_pm_type(jsdata["type"].asString(),_name, (int)_id);

    transform(_name.begin(), _name.end(), _name.begin(), tolower);

    Json::Value &jsattr = jsdata["attributes"];
    assert(jsattr.isArray());
    for (auto it = jsattr.begin(); it != jsattr.end(); ++it) {
        std::string name =(*it)["shortName"].asString();
        transform(name.begin(), name.end(), name.begin(), tolower);
        AttributeInfo info;
        info._attribute = name;
        info._type = get_attribute_type((*it)["dataType"].asString());
        _attributes[name] = info;
    }

    return true;
}

void PolicyModel::AddPreAttribute(const char* json) {
    _strPreAttributeJson = json;

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(json, json + strlen(json), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return ;
    }
    delete (pread);
    pread = nullptr;

    Json::Value &jsdata = root["data"];
    assert(jsdata.isArray());
    for (auto it = jsdata.begin(); it != jsdata.end(); ++it) {
        std::string name =(*it)["shortName"].asString();
        transform(name.begin(), name.end(), name.begin(), tolower);
        AttributeInfo info;
        info._attribute = name;
        info._type = get_attribute_type((*it)["dataType"].asString());
        _attributes[name] = info;

    }
}