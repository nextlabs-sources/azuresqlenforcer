#ifndef WHERE_CLAUSE_H
#define WHERE_CLAUSE_H

#include "Interface.h"
#include "EMColumnRef.h"
class Node;

namespace resolve {
    class WhereCluase : public IWhereCluase {
    public:
        virtual std::string GetCondition();
        virtual bool AddCondition(const std::string& condition, SqlException & e);
        virtual uint64_t GetQueryID() { return query_id_; }
        void bind(Node* where, Node* select);
        virtual std::string GetModifyedCondition(const std::string & field_qualify, const std::vector<std::string> & using_cols, std::set<std::string> & fields); // use to mask condition/filter condition
        Node* where_ = NULL;
        Node* select_ = NULL;
        uint64_t query_id_=0;
        EMDB_DB_TYPE _dbtype{ EMDB_DB_SQLSERVER };
    };

    class EMColumnsRef : public  IColumnsRefItem {
    public:
        EMColumnsRef(RawExpr * col):_stmt_id(0), _stmt(NULL), _column(col),_node(NULL){}
        virtual ~EMColumnsRef(){delete(_column);}
    public:
        void SetStmtId(int id){ _stmt_id = id; }
        void SetStmt(ISelectStmt * stmt) { _stmt = stmt;}
        virtual int GetStmtId(){ return  _stmt_id; }
        ISelectStmt * GetStmt(){ return _stmt;}
        void SetDBType(EMDB_DB_TYPE type){_dbtype = type;}

        virtual bool MaskOpr(MaskConditionMap *  map_mask, SqlException & e);

    private:
      //  ITableItem * _tbitem;
        int _stmt_id=0;
        ISelectStmt * _stmt = NULL;
        RawExpr * _column = NULL;
        Node* _node = NULL;
        EMDB_DB_TYPE _dbtype{EMDB_DB_SQLSERVER};
    };

    class UpdateItem :public IUpdateItem{
    public:
        Node * _node = NULL;
        std::string _field;
        EMColumnInfo _col;
        virtual ~UpdateItem() override;
        std::string GetFieldName() override;
        void DenyUpdate(const std::string & condition,const std::string & field_qualify, EMDataType e, EMDB_DB_TYPE db) override;
    };
}

#endif