#ifndef INTERFACE_H
#define INTERFACE_H

#include "keydef.h"
#include <functional>
#include <vector>
#include "EMMaskDef.h"
#include "EMDBType.h"

#ifndef WIN32
#define SQLPARSER_PUBLIC_API __attribute__((visibility("default")))
#define SQLPARSER_LOCAL_API  __attribute__((visibility("hidden")))
#else

#ifdef  SQLPARSER_EXPORT
#define SQLPARSER_PUBLIC_API  __declspec(dllexport)
#else
#define SQLPARSER_PUBLIC_API  __declspec(dllimport)
#endif //SQLPARSER_EXPORT

#pragma warning(push)
#pragma warning(disable:26812 5045 4996 )

#endif //WIN32

#include "EMMaskDef.h"
#include "SqlException.h"

class INode {
protected:
    virtual ~INode(){}
public:
    virtual NodeType GetType() = 0;
    virtual INode* GetParent() = 0;
    virtual bool SetParent(INode* node) = 0;
    virtual INode* GetChild(int key) = 0;
    virtual bool SetChild(int key, INode* node) = 0;
    virtual size_t GetChildrenCount() const = 0;
    virtual std::string Text() = 0;
    virtual bool IsTerminal() const = 0;
    virtual std::string Serialize() = 0;
    virtual void Print() = 0;
};
class Node;
class ITableItem;
class IWhereCluase;

class IJoinedTable {
 public:
    virtual ~IJoinedTable() {}
 public:
    virtual bool IsJoinedTable(uint64_t table_id) = 0;
    virtual bool ResetJoinPredicate(uint64_t table_id, std::string filter_cond) = 0;
};

class IStmt {
public:
    virtual ~IStmt(){}
public:
    virtual StmtType GetStmtType() = 0;
    virtual uint64_t GetQueryID() = 0;
    virtual size_t GetTableItemCount() const = 0;
    virtual size_t GetFromItemCount() const = 0;
    virtual ITableItem* GetTableItem(size_t index) = 0;
    virtual IStmt* GetParent() = 0;
    virtual INode * GetNode() = 0;
    virtual void SetNode(INode * node) = 0;
    virtual IJoinedTable* GetJoinedTable(uint64_t table_id) = 0;
    virtual bool DenyTables(std::vector<ITableItem*> & tbis, const std::string& denied_schema_name = "DENIED_BY_POLICY") = 0;
    //virtual unsigned int GetSessionContextParamId() = 0;
    //virtual std::string GetSessionContextValue() = 0;


    struct SQL_USER_VALUE {
        enum class ValueType {
            TYPE_UNKNOW,
            TYPE_PARAM_ID,
            TYPE_STRING,
            TYPE_NUMBER
        } _type = ValueType::TYPE_UNKNOW;

        std::string _val;
    };
    virtual const SQL_USER_VALUE& GetSqlUserValue() = 0;
};

class IParseResult {
protected:
    virtual ~IParseResult(){}
public:
    virtual bool IsAccept() const = 0;
    virtual INode* GetParseTree() = 0;
    virtual size_t GetErrorLine() const = 0;
    virtual size_t GetErrorColumn() const = 0;
    virtual std::string GetErrorDetail() const = 0;
    virtual IWhereCluase * CreateSimpleSelectWhereClause() const = 0;
    virtual void ReleaseWhereClause(IWhereCluase * pw) const =0;
};

class ITableItem;
class IColumnsRefItem;
class IPlan;
typedef std::function<void(IPlan*, IStmt*)> StmtVisit;
typedef std::function<void(IPlan*, IColumnsRefItem*)> BaseTableColumnVisit;
typedef std::function<void(IPlan*, uint64_t last_stmt)> StartNewStmt;
typedef std::function<void(IPlan*, IWhereCluase*)> WhereClauseVisit;
typedef std::function<void(IPlan*)> ErrorOccur;

class IWhereCluase {
public:
    virtual ~IWhereCluase(){}

    virtual std::string GetCondition() = 0;
    virtual std::string GetModifyedCondition(const std::string & field_qualify, const std::vector<std::string> & using_cols, std::set<std::string> & fields) = 0;
    virtual bool AddCondition(const std::string& condition, SqlException& exc) = 0;
    virtual uint64_t GetQueryID() = 0;
};

class IUpdateItem{
public:
    virtual ~IUpdateItem(){}
    virtual std::string GetFieldName() = 0;
    virtual void DenyUpdate(const std::string & condition, const std::string & field_qualify, EMDataType e, EMDB_DB_TYPE db) = 0;
};

class IPlan {
protected:
    virtual ~IPlan() {}
public:
    virtual void* GetContext() = 0;
    virtual IStmt* GetQuery(uint64_t query_id) = 0;
    virtual void SetDefaultSchema(const std::string& default_schema) = 0;
    virtual void SetDefaultDatabase(const std::string& default_database) = 0;
    virtual EMDB_DB_TYPE GetDatabaseType() = 0;
    virtual std::string GetDefaultDatabase() = 0;
    virtual void AddTableStructure(
            const std::string& schema,
            const std::string& table,
            const std::list<std::string>& columns) = 0;
    virtual std::string GetErrorDetail() = 0;
};

class ITableItem {
public:
    virtual ~ITableItem(){}

    virtual TableItemType GetTableItemType() const = 0;
    virtual uint64_t GetTableID() const = 0;
    virtual uint64_t get_table_id() const = 0;
    virtual uint64_t GetQueryID() const = 0;

    virtual std::string GetTableName() const = 0;
    virtual std::string GetSchemaName() const  = 0;
    virtual std::string GetDatabaseName() const  = 0;
    virtual std::string GetServerName() const  = 0;


    virtual std::string GetTableAliasName() const = 0;
    virtual std::string GetTableAliasReal() const = 0;
    virtual std::string GetTableHint() const = 0;
    virtual std::string GetTableObject() const = 0;
    virtual bool SetText(
            TableItemType tp,
            const std::string& newtable,
            const std::string& alias) = 0;
    virtual int GetLine() const = 0;
    virtual int GetColumn() const = 0;

    virtual void SetMetadata(const MetadataVec & vec)=0;
    virtual  const MetadataVec & GetMetadata()const=0;

public:
    virtual bool IsMatch() const =0;
    virtual void SetMatch(bool)=0;
    virtual void SetFilterCondition(const std::string &cond)=0;
    virtual const std::string & GetFilterCondition() const =0;
    virtual bool IsDeny() const =0;
    virtual void SetDeny(bool)=0;
    virtual void AddUsingColumns(const std::vector<std::string> & cols) = 0;
    virtual const std::vector<std::string> & GetUsingColumns() = 0;
    virtual NodeType GetJoinType()=0;

};

class IStoreProcedure {
public:
    virtual ~IStoreProcedure() {}
public:
    virtual std::string GetStoreProcedureName() = 0;
    virtual std::string GetSchemaName() = 0;
    virtual std::string GetDatabaseName() = 0;
    virtual std::string GetServerName() = 0;
};

class IColumnsRefItem {
public:
    virtual ~IColumnsRefItem(){}
public:
    
//    virtual ITableItem* GetTableItem() = 0;
//    virtual std::string GetColumnName() = 0;
//    virtual std::string GetColumnObject() = 0;
//    virtual bool SetText(const std::string& columnref) = 0;
//    virtual int GetLine() = 0;
//    virtual int GetColumn() = 0;
    virtual void SetStmtId(int id) = 0;
    virtual int GetStmtId() = 0;
    virtual bool MaskOpr(MaskConditionMap *  map_mask, SqlException & e) = 0;
};

class EMColumnInfo;
class ISelectStmt {
public:
    virtual ~ISelectStmt(){}
    virtual void SetGroupByItems(std::vector<EMColumnInfo*> & group_by)=0;
    virtual const std::vector<EMColumnInfo*> & GetGroupByItems()=0;
    virtual void SetSelectExpr(IColumnsRefItem * expr)=0;
    virtual  IColumnsRefItem * GetSelectExpr()=0;
    virtual void SetWhereClause(IWhereCluase * where_clause)=0;
    virtual  IWhereCluase * GetWhereClause()=0;
   // virtual void AddRawExpr(RawExpr*)=0;

};

class IUpdateStmt {
public:
    virtual ~IUpdateStmt(){}
public:
    virtual bool IsBasicTableOrAlias() = 0;
    virtual ITableItem* GetUpdateTable() = 0;

    virtual const std::vector<IUpdateItem*>& get_update_items()   = 0;
    virtual void add_update_item(IUpdateItem* item) = 0;

    virtual void SetWhereClause(IWhereCluase * where_clause)=0;
    virtual  IWhereCluase * GetWhereClause()=0;
};

class IDeleteStmt {
public:
    virtual ~IDeleteStmt() {}
public:
    virtual bool IsBasicTableOrAlias() = 0;
    virtual ITableItem* GetDeleteTable() = 0;

    virtual void SetWhereClause(IWhereCluase * where_clause)=0;
    virtual  IWhereCluase * GetWhereClause()=0;
};

class ICreateTableStmt {
public:
    virtual ~ICreateTableStmt() {}
};

class IDropTableStmt {
public:
    virtual ~IDropTableStmt() {}
};

class IInsertStmt {
public:
    virtual ~IInsertStmt() {}
public:
    virtual bool IsBasicTableOrAlias() = 0;
    virtual ITableItem* GetInsertTable() = 0;
};

class ICallStmt {
public:
    virtual ~ICallStmt() {}
public:
    virtual IStoreProcedure *GetStoreProcedure() = 0;
};

class IUseStmt {
public:
    virtual ~IUseStmt() {}
public:
    virtual const char * GetSchema() = 0;
    virtual const char * GetDbname() = 0;
};

extern "C" {
SQLPARSER_PUBLIC_API IParseResult* ParseSql(const std::string& sql, EMDB_DB_TYPE dbms);
SQLPARSER_PUBLIC_API void DestroyParseResult(IParseResult*);
SQLPARSER_PUBLIC_API IPlan* CreatePlan(const StmtVisit& baseTableVisit,
                  const BaseTableColumnVisit& baseTableColumnVisit,
                  const StartNewStmt& startNewStmt,
                  const WhereClauseVisit& whereClauseVisit,
                  const ErrorOccur& errorOccur,
                  void* context, IParseResult*);
SQLPARSER_PUBLIC_API void VisitPlan(IPlan*);
SQLPARSER_PUBLIC_API void DestroyPlan(IPlan*);

SQLPARSER_PUBLIC_API bool CompressSql(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms);
SQLPARSER_PUBLIC_API bool BeautifySql(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms);
}

#ifdef WIN32
#pragma warning(pop)
#endif

#endif