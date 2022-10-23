#ifndef IPOLICY_RESOURCE_H
#define IPOLICY_RESOURCE_H

#include "IQueryCloudAZ.h"

#define ATTR_NAME_SERVER    "server"
#define ATTR_NAME_DATABASE  "database"
#define ATTR_NAME_SCHEMA    "schema"
#define ATTR_NAME_TABLE     "table"
#define ATTR_NAME_PROCEDURE "procedure"

#include <string>
#include <map>
#include <memory>


class ResourceAttrValue
{
public:
    ~ResourceAttrValue(){}
    ResourceAttrValue(const std::string& str, CEAttributeType tp):strValue(str),attrType(tp){}
    std::string strValue;
    CEAttributeType attrType;
};

typedef std::map<std::string , ResourceAttrValue*> EMResDictionary;
typedef std::map<std::string , std::shared_ptr<ResourceAttrValue>> EMResDictionary2;


class IPolicyResource
{
    public:
    virtual ~IPolicyResource(){}
    virtual void AddedAttribute(const char* szName, const char* szValue, CEAttributeType attrType=XACML_string)=0;
    virtual bool ValidAttribute(const char* szName, const char* szValue, const char* szOperator, CEAttributeType attrType=XACML_string)=0;
    virtual void CopyDictionary(EMResDictionary & dic) = 0;
    virtual void CopyDictionary2(EMResDictionary2 & dic) = 0;

};
#endif 