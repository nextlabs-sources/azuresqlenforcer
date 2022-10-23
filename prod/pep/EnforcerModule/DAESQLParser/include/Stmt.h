#ifndef STMT_H
#define STMT_H

#include "Interface.h"
#include "EMColumnRef.h"

class Node;

namespace resolve {
    class CteDef;
    class TableRef;
    class ResultPlan;
    struct FromItem {
        enum class FromItemType {
            E_JOINED,
            E_NORMAL
        };
        uint64_t tid_;
        FromItemType fromItemType_;
    };
    class JoinedTable;

    class Stmt : public IStmt {
    public:
        virtual ~Stmt();
        uint64_t get_query_id() const { return query_id_; }
        void set_query_id(uint64_t query_id) { query_id_ = query_id; }
        Stmt* get_parent() const { return parent_; }
        void set_parent(Stmt* parent) { parent_ = parent; }
        CteDef* get_cte_def_by_index(size_t index)
            { return 0 <= index && index < cte_defs_.size() ? cte_defs_[index] : nullptr; }
        const std::vector<TableRef*>& get_table_items() const { return table_items_; }

        /*
         * these api descripe a name resolve algorithm
         *
         * */
        bool check_in_cte(
                const std::string& name,
                uint64_t& out_query_id,
                size_t& out_index);



        TableRef* add_table_item(
                ResultPlan* plan,
                const std::string& schema_name,
                const std::string& table_name,
                const std::string& alias_name,
                const std::string& alias_real,
                uint64_t& out_table_id,
                Node* node);

        TableRef* add_table_item(
                ResultPlan* plan,
                uint64_t query_id,
                const std::string& alias_name,
                const std::string& alias_real,
                uint64_t& out_table_id
        );

        TableRef* add_table_item_func(
                ResultPlan* plan,
                const std::string& func_name,
                const std::string& alias_name,
                const std::string& alias_real,
                uint64_t& out_table_id,
                Node* node);

        CteDef* add_cte_def(
                ResultPlan* plan,
                uint64_t query_id,
                const std::string& cte_name
        );

        bool get_table_item(
                const std::string& schema,
                const std::string& table_name,
                TableRef*& out_table_ref);

        bool get_table_item(
                uint64_t table_id,
                TableRef*& out_table_ref
                );

        bool check_table_column(
                ResultPlan* plan,
                const std::string& column_name,
                TableRef*& table_item,
                uint64_t& column_id);

        bool find_tableref_by_column(
                ResultPlan* plan,
                const std::string& schema,
                const std::string& table,
                const std::string& column_name,
                TableRef*& table_item,
                uint64_t& column_id);



        void add_join(IJoinedTable* j) { joins_.push_back(j); }
        void add_from_item(uint64_t id, FromItem::FromItemType t) { from_items_.push_back({id, t}); }
        uint64_t generate_join_id() { return gen_joined_tid_--; }

    protected:
        uint64_t query_id_{ 0 };
        Stmt* parent_{nullptr};
        std::vector<TableRef*> table_items_;
        std::vector<CteDef*> cte_defs_;

        std::vector<IJoinedTable*> joins_;
        std::vector<FromItem> from_items_;

        INode * _node{ nullptr };

        uint64_t gen_joined_tid_{UINT64_MAX - 10};
        //unsigned int _sctx_param_index{ 0 };
        //std::string _sctx_param_value;
        SQL_USER_VALUE _sql_user_value;
    public:
        virtual StmtType GetStmtType() override { return  E_NONE; }
        virtual uint64_t GetQueryID() override { return get_query_id(); }
        virtual size_t GetTableItemCount() const override { return table_items_.size(); }
        virtual size_t GetFromItemCount() const override { return from_items_.size(); }
        virtual ITableItem* GetTableItem(size_t index) override;
        virtual IStmt* GetParent() override { return get_parent(); }
        virtual INode * GetNode() override { return _node;}
        void SetNode(INode * node) override {_node = node;}
        IJoinedTable* GetJoinedTable(uint64_t table_id) override {
            for (auto joined_table: joins_)
            {
              if (joined_table->IsJoinedTable(table_id))
              {
                return joined_table;
              }
            }
            return nullptr;
        }
        virtual bool DenyTables(std::vector<ITableItem*>& tbis, const std::string& denied_schema_name) override;
        //void SetSessionContextParamId(unsigned int id) { _sctx_param_index = id; }
        //void SetSessionContextValue(const std::string& val) { _sctx_param_value = val; }
        void SetSqlUserValue(const SQL_USER_VALUE& val) { _sql_user_value = val; }
        virtual const SQL_USER_VALUE& GetSqlUserValue() { return _sql_user_value; }
        //virtual unsigned int GetSessionContextParamId() override { return _sctx_param_index;  };
        //virtual std::string GetSessionContextValue() override { return _sctx_param_value;  }
        friend class CteTableRef;
    };
}

#endif