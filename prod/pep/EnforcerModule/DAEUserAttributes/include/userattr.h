#ifndef CUSER_ATTR_H
#define CUSER_ATTR_H
#include "iuserattr.h"
#include <map>
#include <string>
#include <shared_mutex>

//struct UserAttrValue
//{
//    std::string strValue;
//    int nType;
//};

class UserAttr : public IUserAttr
{
    public:
    UserAttr(const char* szUserName);
    ~UserAttr();

    void SetSeparator(const std::string & separator_){_multi_val_separator = separator_;}
    void AddAttr(const char* szName, const char* szValue);
    void UpdateAttr(UserAttr& rhs);
    //interface for IUserAttr
    public:
    const std::string& GetUserNameAttr() const ;
    std::string GetUserAttr(const char* szAttrName) ;
    bool ValidUserAttr(const char* szAttrName, const char* szAttrValue, const char* szOperator);
    void CopyDictionary(EMResDictionary  & dic);
    void CopyDictionary2(EMResDictionary2  & dic);
    virtual uint64_t GetAttributeCount() { return m_dicAttr.size(); }

    std::string Print();
    void Clear(EMResDictionary & map);
    virtual void AddAttrWithType(const char* szName, const char* szValue, CEAttributeType attrType = XACML_string);

    private:
    std::string m_strName;
    EMResDictionary m_dicAttr;
    std::shared_mutex m_mutexAttr;
    std::string _multi_val_separator;
};


#endif 
