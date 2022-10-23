#include "userattr.h"
#include "operator_between.h"
#include <thread>
#include "log.h"
#include "assert.h"
#include <algorithm>
#include "commfun.h"
#include "PolicyResource.h"

UserAttr::UserAttr(const char* szUserName):
m_strName(szUserName) {

}
UserAttr::~UserAttr() {
    std::unique_lock<std::shared_mutex> lockAttr(m_mutexAttr);
    Clear(m_dicAttr);
}

const std::string& UserAttr::GetUserNameAttr() const {
  return m_strName;
}

void  UserAttr::Clear(EMResDictionary & map){  // no need locked
    auto it = map.begin();
    for(; it != map.end();++it){
        delete(it->second);
    }
    map.clear();
}
void UserAttr::UpdateAttr(UserAttr& rhs) {
    //lock attribute for write
    std::unique_lock<std::shared_mutex> lockAttr(m_mutexAttr);
    Clear(m_dicAttr);
    for(auto it = rhs.m_dicAttr.begin(); it != rhs.m_dicAttr.end(); ++it){
        ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
        m_dicAttr[it->first]=uValue;
    }
    Clear(rhs.m_dicAttr);
}

void UserAttr::CopyDictionary(EMResDictionary  & dic){
    std::shared_lock<std::shared_mutex> lockReadAttr(m_mutexAttr);
    auto it = m_dicAttr.begin();
    for(;it != m_dicAttr.end(); ++it) {
        if(dic.find(it->first) == dic.end()){
            ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
            dic[it->first]=uValue;
        }
    }
}

void UserAttr::CopyDictionary2(EMResDictionary2  & dic){
    std::shared_lock<std::shared_mutex> lockReadAttr(m_mutexAttr);
    auto it = m_dicAttr.begin();
    for(;it != m_dicAttr.end(); ++it) {
        if(dic.find(it->first) == dic.end()){
            std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(it->second->strValue,it->second->attrType);
            //ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
            dic[it->first]=ptrb;
        }
    }
}

bool is_dec(char c) {
    return '0' <= c && c <= '9';
}
bool is_num(const std::string & ss){
    char* pEnd = nullptr;
    strtod(ss.c_str(), &pEnd);

    if (pEnd != nullptr && (pEnd - ss.c_str()) == ss.length())
        return true;
    else
        return false;
}

CEAttributeType get_value_type(const std::string &src,const std::string & val_separator){
    size_t index = src.find(val_separator);
    if(!val_separator.empty() && index != std::string::npos){
        if(is_num(src.substr(0, index))){
            return XACML_ArrInt;
        } else {
            return XACML_ArrStr;
        }

    } else if(is_num(src)){
        if(src.find('.') !=  std::string::npos) {
            return XACML_Double;
        } else {
            return XACML_int;
        }
    } else if(CommonFun::StrCaseCmp(src.c_str(),"TRUE") == 0
            ||CommonFun::StrCaseCmp(src.c_str(),"FALSE") == 0  )
    {
        return XACML_bool;
    } else {
        return XACML_string;
    }

}

void UserAttr::AddAttr(const char* szName1, const char* szValue) {
    std::string ori_name = std::string(szName1);
    std::string ori_value = std::string(szValue);
    std::string name = CommonFun::Trim(ori_name);
    std::string value = CommonFun::Trim(ori_value);

    // To fix bug69572 and bug 69658 comment 2 issue 1.
    // Maybe cause some problems when there are "mail" and "email" attributes with different values
    if (CommonFun::CaseInsensitiveEquals(name, "mail"))
    {
        AddAttr("email", value.c_str());
    }
    // Maybe cause some problems when there are "user_name", "name" and "subject-id" attributes with different values
    else if (CommonFun::CaseInsensitiveEquals(name, "user_name"))
    {
        AddAttr("name", value.c_str());
        AddAttr("subject-id", value.c_str());
    }
    else if (CommonFun::CaseInsensitiveEquals(name, "objectSid"))
    {
        AddAttr("subject-id", value.c_str());
    }

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    // auto itAttr = m_dicAttr.find(name);
    
    //lock attribute for write
    std::unique_lock<std::shared_mutex> lockAttr(m_mutexAttr);
    auto it = m_dicAttr.find(name);
    if (it != m_dicAttr.end()) {
        auto val =  it->second;
        val->strValue = value;
        val->attrType = get_value_type(value, _multi_val_separator);
        Log::WriteLog(log_info, "Alter UserAttr %s=%s(%d)", name.c_str(), value.c_str(), int(val->attrType));
    }
    else {
        ResourceAttrValue* uValue = new ResourceAttrValue(value.c_str(), get_value_type(value.c_str(), _multi_val_separator));
        m_dicAttr[name] = uValue;
        Log::WriteLog(log_info, "Add UserAttr %s=%s(%d)", name.c_str(), value.c_str(), int(uValue->attrType));
    }
    
    
}

std::string UserAttr::GetUserAttr(const char* szAttrName1) {
    std::string szAttrName = szAttrName1;
    std::transform(szAttrName.begin(), szAttrName.end(), szAttrName.begin(), ::tolower);

    //lock attribute
    std::shared_lock<std::shared_mutex> lockReadAttr(m_mutexAttr);
     auto itAttr = m_dicAttr.find(szAttrName);
     if(itAttr!=m_dicAttr.end())
         return itAttr->second->strValue;

      return "";  
}
enum EMDBUserAttrType{ 
    UserAttr_TimeStamp, 
    UserAttr_Time,
    UserAttr_IP,
    UserAttr_Application,
    UserAttr_Other,
    UserAttr_Err 
    };
EMDBUserAttrType GetUserAttributeNameType(const char* szAttrName, std::string& ref) {
    ref.clear();
    int i = 0 ; 
    while (szAttrName[i] != '\0') {
        
        if (szAttrName[i] == '@'  ) {
            //is special attr
            if (szAttrName[i+1] == '\0') {
                
                if (CommonFun::StrCaseCmp(szAttrName, TIMESTAMP_CHAR) == 0)
                    return UserAttr_TimeStamp;
                else if(CommonFun::StrCaseCmp(szAttrName, TIME_CHAR) == 0)
                    return UserAttr_Time;
                else if (CommonFun::StrCaseCmp(szAttrName, IP_CHAR) == 0)
                    return UserAttr_IP;
                else if(CommonFun::StrCaseCmp(szAttrName,APPLICATION_CHAR) == 0)
                    return UserAttr_Application;
                else
                    return UserAttr_Err;
            }
            else if(szAttrName[i+1] == '@')
                i++;
            else
                return UserAttr_Err;

        }
        ref += szAttrName[i];
        i++;
    }
    return UserAttr_Other;
}
bool ValidDynamicAttr(EMDBUserAttrType type, const char* szAttrValue, const char* szOperator) {
    BETWEEN_OP_RESULT ret = BETWEEN_OP_ERR;
    bool bRet = false;
    if ( CommonFun::StrCaseCmp(szOperator, CONDITION_NOT_EQUAL)==0) {
        switch (type) {
            case UserAttr_TimeStamp: {
                ret = cur_timestamp_between(szAttrValue);
                if (ret == BETWEEN_OP_FL)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "Timestamp initialization error of policy value(%s).", szAttrValue);
            } break;            
            case UserAttr_Time: {
                ret = cur_time_between(szAttrValue);
                if (ret == BETWEEN_OP_FL)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "Time initialization error of policy value(%s).", szAttrValue);                
            } break;
            case UserAttr_IP: {
                ret = cur_ip_between(szAttrValue);
                if (ret == BETWEEN_OP_FL)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "IP initialization error of policy value(%s).", szAttrValue);
                
            } break;
            case UserAttr_Application: {
               
                if (!compare_with_cur_application(szAttrValue)) { 
                    bRet = true;                    
                }    
            } break;            
        default:
            assert(false);
            break;
        }
    }
    else if(CommonFun::StrCaseCmp(szOperator, CONDITION_EQUAL)==0 ) {
        switch (type) {
            case UserAttr_TimeStamp:{
                ret = cur_timestamp_between(szAttrValue);
                if (ret == BETWEEN_OP_TR)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "Timestamp initialization error of policy value(%s).", szAttrValue);
                
            }
            break;
            case UserAttr_Time:{
                ret = cur_time_between(szAttrValue);
                if (ret == BETWEEN_OP_TR)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "Time initialization error of policy value(%s).", szAttrValue);
            }
            break;
            case UserAttr_IP:{
                ret = cur_ip_between(szAttrValue);
                if (ret == BETWEEN_OP_TR)
                    bRet = true;
                else if (ret == BETWEEN_OP_ERR)
                    Log::WriteLog(log_warning, "IP initialization error of policy value(%s).", szAttrValue);
            }
            break;
            case UserAttr_Application:{
                if (compare_with_cur_application(szAttrValue))
                    bRet = true;
            }
            break;
        
        default:
            assert(false);
            break;
        }
    }
    
    
    return bRet;
}


bool UserAttr::ValidUserAttr(const char* szAttrName, const char* szAttrValue, const char* szOperator) {
    std::string strNewName;
    EMDBUserAttrType type = GetUserAttributeNameType(szAttrName, strNewName);
    bool bValid = false;
    if (UserAttr_Err == type  ) {
        Log::WriteLog(log_warning, "User attribute name '%s' produced a error in the policy.", szAttrName);
    }
    else if (UserAttr_Other == type) {
        std::string value = GetUserAttr(strNewName.c_str());

        if(CommonFun::StrCaseCmp(szOperator, CONDITION_EQUAL)==0 || CommonFun::StrCaseCmp(szOperator, CONDITION_NOT_EQUAL)==0) {
            //compare value as string
            bool bEqual = CommonFun::StrCaseCmp(szAttrValue, value.c_str() ) ==0 ;
            bValid = (bEqual && CommonFun::StrCaseCmp(szOperator, CONDITION_EQUAL)==0 ) ||
                        (!bEqual && CommonFun::StrCaseCmp(szOperator, CONDITION_NOT_EQUAL)==0 );

        }
        else {//compare value as number
			double attrvalue = atof(value.c_str());
			double inValue = atof(szAttrValue);

			if (CommonFun::StrCaseCmp(szOperator, CONDITION_SMALL) == 0)
                bValid = attrvalue < inValue;
			else if (CommonFun::StrCaseCmp(szOperator, CONDITION_SMALL_EQUAL) == 0)
                bValid = attrvalue <= inValue;
			else if (CommonFun::StrCaseCmp(szOperator, CONDITION_LARGE) == 0)
                bValid = attrvalue > inValue;
			else if (CommonFun::StrCaseCmp(szOperator, CONDITION_LARGE_EQUAL) == 0)
                bValid = attrvalue >= inValue;
			else {
				Log::WriteLog(log_warning, "UserAttr::ValidUserAttr, unsupport operator:%s", szOperator);
				bValid = false;
			}
        }
    }
    else
        bValid = ValidDynamicAttr(type, szAttrValue, szOperator);

    return bValid;
}

std::string UserAttr::Print(){
    std::string ret;
    ret += "USER name=";
    ret+= m_strName;
    ret+= "(";
    std::shared_lock<std::shared_mutex> lockReadAttr(m_mutexAttr);
    for(auto it=m_dicAttr.begin(); it != m_dicAttr.end(); ++it) {
        ret+=it->first;
        ret+= "=";
        ret+= it->second->strValue;
        ret+= ", ";
    }
    ret+= ")";
    return ret;
}

void UserAttr::AddAttrWithType(const char* szName1, const char* szValue, CEAttributeType attrType /*= XACML_string*/) {
    std::string szName = szName1;
    std::transform(szName.begin(), szName.end(), szName.begin(), ::tolower);
    // auto itAttr = m_dicAttr.find(szName);

    //lock attribute for write
    std::unique_lock<std::shared_mutex> lockAttr(m_mutexAttr);
    auto it = m_dicAttr.find(szName);
    if (it != m_dicAttr.end()) {
        auto val = it->second;
        val->strValue = szValue;
        val->attrType = attrType;
        Log::WriteLog(log_info, "Alter UserAttr %s=%s(%d)", szName.c_str(), szValue, int(attrType));
    }
    else {
        ResourceAttrValue* uValue = new ResourceAttrValue(szValue, attrType);
        m_dicAttr[szName] = uValue;
        Log::WriteLog(log_info, "Add UserAttr %s=%s(%d)", szName.c_str(), szValue, int(attrType));
    }
}