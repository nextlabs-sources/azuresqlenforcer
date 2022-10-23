#ifndef CPOLICY_RESOURCE_H
#define CPOLICY_RESOURCE_H

#include "IPolicyResource.h"
#include <map>
#include "iuserattr.h"
#include "IPolicyResource.h"
#include "commfun.h"
#include "EMDBType.h"
#include "EMMaskDef.h"
#include "keydef.h"


class CPolicyResource : public IPolicyResource
{
    public:
    CPolicyResource();
    ~CPolicyResource();
    public:
    void AddedAttribute(const char* szName, const char* szValue, CEAttributeType attrType=XACML_string);
    bool ValidAttribute(const char* szName, const char* szValue, const char* szOperator, CEAttributeType attrType=XACML_string);
    void CopyDictionary(EMResDictionary & dic);
    void CopyDictionary2(EMResDictionary2 & dic);

protected:
    const ResourceAttrValue* FindAttribute(const char* szName);

    private:
        EMResDictionary2 m_dicAttrValue;

};


//policy action define
#define ACTION_NONE   ""
#define ACTION_SELECT "SELECT"
#define ACTION_UPDATE "UPDATE"
#define ACTION_DELETE "DELETE"
#define ACTION_INSERT "INSERT"
#define ACTION_CREATE "CREATE"
#define ACTION_DROP   "DROP"
#define ACTION_DAE_CONFIG "DAE_CONFIG"
#define ACTION_CALL_SP "CALL_SP"


#define CONDITION_EQUAL       "="
#define CONDITION_NOT_EQUAL   "!="
#define CONDITION_SMALL       "<"
#define CONDITION_SMALL_EQUAL "<="
#define CONDITION_LARGE       ">"
#define CONDITION_LARGE_EQUAL ">="

#include <vector>
struct ConditionInfo {
    ConditionInfo(const uint64_t table_id,
        const std::string& real,
        const std::string& name,
        const std::string& act,
        EMDB_DB_TYPE type,
        CPolicyResource policyRes,
        IUserAttr* pU,
        const std::vector<std::string>& cols,
        NodeType e,
        const std::string & key) :
        table_id(table_id),
        real_tbname(real),
        tableName(name),
        _action(act),
        dbType(type),
        _resource(policyRes),
        pUserAttr(pU),
        _using_cols(cols),
        _join_tp(e),
        _cache_key(key){
    }

    uint64_t table_id;
    std::string real_tbname;
    std::string tableName;
    std::string _action;
    EMDB_DB_TYPE dbType;
    CPolicyResource _resource;
    IUserAttr* pUserAttr;
    std::vector<std::string> _using_cols;
    NodeType _join_tp;
    std::string _cache_key;
};


#endif