#ifndef ENFORCERMGR_H
#define ENFORCERMGR_H

#include <string>
#include <map>
#include <shared_mutex>
#include "Interface.h" //interface of sqlparse module
#include "sqlenforcer_export.h"
#include "SqlException.h"


#include <map>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

#include "table_column.h"

template <class T>
using SharedPtrVector = std::vector<std::shared_ptr<T>>;

template <class T>
using SharedPtrMap = std::map<std::string, std::shared_ptr<T>>;

class QueryPcResult;




class UserContextInfo;
class RequestInfo;
struct ConditionInfo;

struct SQLParseCtx
{
    UserContextInfo *pUserCtxInfo=NULL;
    VecBindSqlParams _sql_params;
    bool bError=false;
    std::string strErrorDetail;
 //   unsigned int blocked_stmt_cnt=0;
//    std::string strBlockDetail;
//    INode * _node=NULL;
//    std::string _old_sql;
    std::vector<std::shared_ptr<RequestInfo>> _vec_report;

    SqlException _exc;

    bool _sql_enforcer = false;
    unsigned int _filter_deny_times = 0;
    unsigned int _deny_times = 0;
    unsigned int _mask_times = 0;
    unsigned int _filter_times = 0;
};


class CEnforcerMgr
{
  public:
  static CEnforcerMgr* Instance()
  {
      static CEnforcerMgr* pInstance = new CEnforcerMgr();
      return pInstance;
  }

  protected:
  CEnforcerMgr();
  CEnforcerMgr(const CEnforcerMgr&){}

  public:
  //bool Init(const char* wszModuleName,const char* szCfgFile, SqlException * exc);
  USER_CONTEXT NewContext(const char* szUserName, const char* szPwd, SqlException * pExc);
  void FreeContext(USER_CONTEXT);
  void SetUserContextInfo(USER_CONTEXT ctx, USER_CONTEXT_INFO_TYPE infoType, const char *szInfo);
  std::wstring EvaluationSQL(USER_CONTEXT context, const wchar_t* sqltext, VecBindSqlParams& params, SqlException * pExc);
  std::string  EvaluationSQL(USER_CONTEXT context, const char* sqltext, VecBindSqlParams & params, SqlException * pExc);
  void SetMetadataFunc(USER_CONTEXT ctx, QueryMetadataFunc func);

  public:
  bool Init_SDK(const char* szWorkerName, SqlException * pExc);
  USER_CONTEXT NewContext_SDK( SqlException * pExc);
  void AddUserAttrValue_SDK(USER_CONTEXT ctx, const char* key,const char* value, SqlException * pExc);
  void ClearUserContext_SDK(USER_CONTEXT ctx);
  bool FreeUserContext_SDK(USER_CONTEXT ctx);

  
  private:
  //call back for sqlparser
  void StmtVisit(IPlan* plan, IStmt* tbi);
  void ColumnVisit(IPlan* plan, IColumnsRefItem* cli);
  void StartNewStmt(IPlan* plan, uint64_t lastStatID);
  void WhereCluase(IPlan* plan, IWhereCluase* wc);
  void ErrorOccur(IPlan* plan);

//  void ColumnVisitOnSelect(IPlan* plan, IColumnsRefItem* cli, IStmt* stmt, SQLParseCtx* pParseCtx,
//                           IPolicyResource* pResourceAttr);
// std::string WhereCluaseOnSelect(IPlan* plan, IWhereCluase* wc, IStmt* stmt, SQLParseCtx* pParseCtx, 
//                           IPolicyResource* pResourceAttr);
// std::string WhereCluaseOnDelete(IPlan* plan, IWhereCluase* wc, IStmt* stmt, SQLParseCtx* pParseCtx, 
//                           IPolicyResource* pResourceAttr);
// std::string WhereCluaseOnUpdate(IPlan* plan, IWhereCluase* wc, IStmt* stmt, SQLParseCtx* pParseCtx, 
//                           IPolicyResource* pResourceAttr);

// std::string DoFilterForTableItem(ITableItem* tbi,const std::string& strPolicyAction, 
//                                   UserContextInfo* pUserCtx, IPolicyResource* pResourceAttr, MaskConditionMap *  map_mask = NULL);

  //
  
  
  //std::string GetTableConditionFromPolicy(const ConditionInfo* pInfo, IEvalResult* pEvalResult);
public:
  static std::string GetPolicyActionByStmtType(StmtType stmtType);
  void GetLocalHostInfo(std::string & host_name, std::string & host_ip);
  void GetAppNameInfo(std::string & app_name, std::string & app_path);

  std::string GetDbServerName() { return _db_server_name; }

  static UserContextInfo* GetUserContextInfo(USER_CONTEXT ctx);

  private:
  std::string _worker_name;
  SqlException _exc;
  //use for query pc
  std::string _hostname;
  std::string _host_ip;
  std::string _app_name;
  std::string _app_path;
  std::string _db_server_name;
  std::string _odbc_connection_string;

public:
    void InsertQueryPCResultCaches(const std::string & key, std::shared_ptr<QueryPcResult>  result);
    bool GetQueryPCResultCaches(const std::string & key, std::shared_ptr<QueryPcResult> & result);
    void ClearQueryPCResultCaches_SyncFunc();//need notify
    void ClearQueryPCResultCaches_SyncFunc2();//fixed interval
    void UpdatePCResultCachesStatus();

    std::string QueryDbServerName();
    std::string QueryDefaultSchema(const std::string& dbname, const std::string& owner);
    std::vector<TableColumn> QueryTableColumns(const std::string& dbname, const std::string& schema, const std::string& table);

    SharedPtrMap<QueryPcResult>  _map_querypc_caches;
    std::shared_timed_mutex _querypc_caches_mutex;
    std::condition_variable _caches_cond;
    std::mutex _caches_mutex;
    bool _b_need_update_cache = false;
};


#endif 