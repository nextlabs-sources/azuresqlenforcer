#ifndef SQL_COLLECT_H
#define SQL_COLLECT_H
#include <string>
#include "QueryCAZPolicyOpr.h"
enum EnforcerResultCode{ EF_BLOCK_THIS_TEXT, EF_USE_NEW_TEXT };

struct EnforcerResultData{
    std::string username;
    std::string oriSql;
    std::string newSql;
    EnforcerResultCode retCode = EF_BLOCK_THIS_TEXT;
    bool bParseSucceed = false;//
};






struct ISqlCollect {
    virtual ~ISqlCollect() {}
    virtual void collect_enforcer_result(const EnforcerResultData &data) = 0;
    virtual void collect_report_result(std::vector<std::shared_ptr<RequestInfo>>& datas, const std::string& old_sql, const std::string& new_sql) = 0;
    //virtual void collect_sql(const std::string & old_sql, const std::string & new_sql) = 0;
};



ISqlCollect *make_sql_collector(bool binit = true);

#endif