#ifndef DELETE_STMT_H
#define DELETE_STMT_H

#include "Interface.h"
#include "Stmt.h"

namespace resolve {
    class DeleteStmt : public Stmt, public IDeleteStmt {
    public:
        ~DeleteStmt();
        bool set_delete_table(
                ResultPlan* plan,
                NodeType type,
                const std::string& schema_name,
                const std::string& table_name,
                TableRef*& out_table_ref);
    private:
        TableRef*  delete_table_{nullptr};
        IWhereCluase * _where_clause{nullptr};
    public:
        virtual StmtType GetStmtType() override { return  E_STMT_TYPE_DELETE; }

        virtual bool IsBasicTableOrAlias() override;
        virtual ITableItem* GetDeleteTable() override;

        void SetWhereClause(IWhereCluase * where_clause) override {_where_clause = where_clause;}
        IWhereCluase * GetWhereClause()override { return _where_clause;}
    };
}



#endif