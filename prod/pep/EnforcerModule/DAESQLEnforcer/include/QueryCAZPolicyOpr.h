//
// Created by jeff on 2020/4/24.
//

#ifndef EMDB_QUERYCAZPOLICYOPR_H
#define EMDB_QUERYCAZPOLICYOPR_H

#include <string>
#include <vector>
#include "IQueryCloudAZ.h"
#include "EMMaskDef.h"
#include "PolicyResource.h"
//
#include "SqlException.h"

enum EMOperator{
    EMOP_EQ,
    EMOP_NEQ,
    EMOP_LE,
    EMOP_LT,
    EMOP_GE,
    EMOP_GT,
    EMOP_LIKE,
    EMOP_NOT_LIKE,
    EMOP_IN

};

struct EMCondVal{
    enum EmvalType{
        EMVAL_STR,
        EMVAL_SUBQUERY,
        EMVAL_NUM,
        EMVAL_IDENT
    } _type;
    std::string _val;
};


class EMCondition {
public:
    static EMOperator TransformOp(const std::string & sop, SqlException & e) ;
    static std::string Serialize(std::vector<EMCondition*> &conds, ConditionInfo & condinfo); // OR LINK
    std::string                      _col;
    EMOperator                       _op;
    EMCondVal                        _const_val;
};



enum PCResultType{
    PCResult_Error,
    PCResult_Config,
    PCResult_Enforcer
};

class QueryPcResult{
public:
    virtual ~QueryPcResult(){}
    QueryPcResult();
    QueryPcResult(PCResultType type):_type(type), _deny(false){ }
    QueryPcResult(const QueryPcResult& qpr);
    virtual bool IsDeny(){ return _deny; };
    virtual void SetDeny(bool b) {_deny = b; };
    virtual PCResultType GetType(){ return _type; }
    virtual void SetTableId(uint64_t table_id) { _table_id = table_id; }
    virtual uint64_t GetTableId() { return _table_id; }

    PCResultType _type;
    bool _deny ;
    uint64_t _table_id=0;

};

class RequestInfo{
public:
    RequestInfo(){}
    //RequestInfo(const RequestInfo& info);
    //RequestInfo & operator=(const RequestInfo& info);
    ~RequestInfo();
    std::string _action;
    //EMResDictionary _users;
    //EMResDictionary _rescs;
    EMResDictionary2 _users2;
    EMResDictionary2 _rescs2;

    std::string _client_app_name;
    std::string _client_host_name;
    std::string _client_ip;

    std::string GenerateCacheKey();

};

struct EMAuditlogInfo{
    std::vector<std::shared_ptr<RequestInfo>> _reqs;
    std::string _ori;
    std::string _new;
};


class EnforcerPcResult: public  QueryPcResult {
public:
    EnforcerPcResult():QueryPcResult(PCResult_Enforcer){}
    ~EnforcerPcResult();
    EnforcerPcResult(const EnforcerPcResult& eps);
    //std::vector<EMCondition*> _filter_cond;
    std::string _filter_cond;
    MaskConditionMap _mask_map;
    std::string _table_name;

};

class ConfigPcResult: public  QueryPcResult {
public:
    ConfigPcResult():QueryPcResult(PCResult_Config){}
    ~ConfigPcResult(){}

    std::vector<std::string> _fields;
    std::vector<std::string> _tables;
    std::vector<std::string> _schemas;

};

bool get_host(std::string *phost,std::string  *pip);
bool get_app(std::string *app_name,std::string  *app_path);

IPolicyRequest * create_request(const RequestInfo & req, const std::string & user_id);
QueryStatus  single_query_pc(const IPolicyRequest* pcRequest, IPolicyResult** pcResult);
QueryStatus  multi_query_pc (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult);
std::shared_ptr<QueryPcResult> parser_result_array(IPolicyResult * result, ConditionInfo & condinfo, SqlException & e);
void free_pc_request(IPolicyRequest* p);
void free_pc_result(IPolicyResult* p);
void free_pc_result(std::vector<QueryPcResult *> & vp);

// std::string parser_condition(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields);
std::string parser_condition_v2(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields);
std::string get_field_qualify( EMDB_DB_TYPE dbtype, const std::string & tablename);  /* get field qualify by table name ,special for bigquery*/

#endif //EMDB_QUERYCAZPOLICYOPR_H
