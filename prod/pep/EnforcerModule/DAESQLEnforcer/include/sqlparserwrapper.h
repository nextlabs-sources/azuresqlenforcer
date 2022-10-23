#ifndef SQLPARSER_WRAPPER_H
#define SQLPARSER_WRAPPER_H

#include "Interface.h"
#include "commfun.h"

typedef  IParseResult* (*ParseSqlFun)(const std::string& sql, EMDB_DB_TYPE dbms);
typedef  void (*DestroyParseResultFun)(IParseResult*);
typedef IPlan* (*CreatePlanFun)(const StmtVisit& stmtVisit,
                  const BaseTableColumnVisit& baseTableColumnVisit,
                  const StartNewStmt& startNewStmt,
                  const WhereClauseVisit& whereClauseVisit,
                  const ErrorOccur& errorOccur,
                  void* context, IParseResult*);
typedef void (*VisitPlanFun)(IPlan*);
typedef void (*DestroyPlanFun)(IPlan*);

typedef bool (*CompressSqlFun)(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms);
typedef bool (*BeautifySqlFun)(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms);

class CSqlparserWrapper
{
  public:
  static CSqlparserWrapper* Instance()
  {
      static CSqlparserWrapper* pInst = new CSqlparserWrapper();
      return pInst;
  }

  bool LoadParser();
  bool IsWell(){return m_bWell;}

  protected:
  CSqlparserWrapper();
  //CSqlparserWrapper(const CSqlparserWrapper&){}

  public:
  ParseSqlFun ParseSql;
  DestroyParseResultFun DestroyParseResult;
  CreatePlanFun  CreatePlan;
  VisitPlanFun VisitPlan;
  DestroyPlanFun DestroyPlan;
  CompressSqlFun CompressSql;
  BeautifySqlFun BeautifySql;

  private:
  bool m_bWell;
  
};

#endif //SQLPARSER_WRAPPER_H