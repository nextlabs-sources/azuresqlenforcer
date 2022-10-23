#ifndef UPDATE_STMT_H
#define UPDATE_STMT_H

#include "Interface.h"
#include "Stmt.h"

namespace resolve {


    class TableRef;
    class UpdateStmt : public Stmt, public IUpdateStmt {
    public:
        ~UpdateStmt();
        const std::vector<IUpdateItem*>& get_update_items() override;
        void add_update_item(IUpdateItem* item);
        bool set_update_table(
                ResultPlan* plan,
                NodeType type,
                const std::string& schema_name,
                const std::string& table_name,
                TableRef*& out_table_ref);
        void SetWhereClause(IWhereCluase * where_clause) override {_where_clause = where_clause;}
        IWhereCluase * GetWhereClause()override { return _where_clause;}
    private:
        std::vector<IUpdateItem*> update_items_;
        TableRef*  update_table_{nullptr};
        IWhereCluase * _where_clause{nullptr};
    public:
        virtual StmtType GetStmtType() override { return  E_STMT_TYPE_UPDATE; }
        virtual bool IsBasicTableOrAlias() override;
        virtual ITableItem* GetUpdateTable() override;
    };
}

#endif