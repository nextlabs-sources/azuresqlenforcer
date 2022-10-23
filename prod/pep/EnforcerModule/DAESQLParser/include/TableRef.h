#ifndef TABLE_REF_H
#define TABLE_REF_H

#include "Interface.h"
#include "Stmt.h"

class Node;

class EMTableInfo {
public:
    EMTableInfo():_node(NULL){}
    EMTableInfo(Node * node):_node(node){}
    virtual ~EMTableInfo(){};
    Node * _node=NULL;
    std::string _db;
    std::string _schema;
    std::string _tb;
    std::string _alias;
};

namespace resolve {
    class ResultPlan;
    class LogicPlan;
    class SelectItem;
    class UpdateStmt;
    class DeleteStmt;
    class InsertStmt;
    class DropTableStmt;

    class TableRef : public  ITableItem {
    public:
        TableRef():table_ref_type_(NULL_REF),table_id_(0),query_id_(0),_match(false),_deny(false){}
    public:
        enum TableRefType {
            NULL_REF,
            BASE_TABLE_DIRECT_REF,
            BASE_TABLE_ALIAS_REF,
            GENERATED_TABLE_REF,
            CTE_TABLE_REF,
            FUNCTION_TABLE_REF
        };
        virtual ~TableRef() {}
        TableRefType get_table_ref_type() const { return table_ref_type_; }
        void set_table_ref_type(TableRefType table_ref_type) { table_ref_type_ = table_ref_type; }
        uint64_t get_table_id() const { return table_id_; }
        void set_table_id(uint64_t table_id) { table_id_ = table_id; }
        uint64_t get_query_id() const { return query_id_; }
        void set_query_id(uint64_t query_id) { query_id_ = query_id; }

        /*  */
        virtual std::string get_table_name() const = 0;
        virtual bool check_column(
                ResultPlan* plan,
                const std::string column_name,
                uint64_t& out_column_id) const = 0;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const = 0;
        virtual bool expand(
                ResultPlan* plan,
                std::vector<SelectItem*>& out_select_items,
                uint64_t start_index) = 0;
        virtual TableRef* clone() const = 0;

        /* Implement Of ITableItem */
        virtual TableItemType GetTableItemType() const override { return E_UNKNOWN; }
        virtual TableRefType GetTableRefType() const  { return table_ref_type_; }
        virtual uint64_t GetTableID() const override { return OB_INVALID_ID; }
        virtual uint64_t GetQueryID() const override { return OB_INVALID_ID; }
        virtual std::string GetTableName() const override { return ""; }
        virtual std::string GetTableAliasName() const override { return alias_name_; }
        virtual std::string GetTableAliasReal() const override { return alias_real_; }
        virtual std::string GetSchemaName() const override { return ""; }
        virtual std::string GetDatabaseName() const override { return ""; }
        virtual std::string GetServerName() const override { return ""; }
        virtual std::string GetTableHint() const override { return ""; }
        virtual std::string GetTableObject() const override { return ""; }
        virtual bool SetText(
                TableItemType tp,
                const std::string& newtable,
                const std::string& alias) override { return false; }
        virtual int GetLine() const override { return 0; }
        virtual int GetColumn() const override { return 0; }

    public:    /*add by jeff when mask opr we need record filter condition ,avoid query policy multiple.*/
        virtual bool IsMatch() const{
            return _match;
        }
        virtual void SetMatch(bool m){ 
            _match = m; 
        };
        virtual void SetFilterCondition(const std::string &cond){};
        virtual const std::string & GetFilterCondition()const { return _cond; }
        virtual bool IsDeny()const { return _deny; }
        virtual void SetDeny(bool d){ _match = d; }
        virtual void AddUsingColumns(const std::vector<std::string> & cols) {
            _using_cols.clear();
            _using_cols = cols;
        }
        virtual const std::vector<std::string> & GetUsingColumns(){ return _using_cols;}
        virtual NodeType GetJoinType() { return _join_type;}
        void SetJoinType(NodeType e) { _join_type = e; }

        virtual void SetMetadata(const MetadataVec & vec){ _metadata.assign(vec.begin(),vec.end()); }
        virtual  const MetadataVec & GetMetadata()const{ return _metadata;}

    protected:
        std::string alias_name_;
        std::string alias_real_;
        TableRefType table_ref_type_;
        uint64_t table_id_ = 0; // table unique code
        uint64_t query_id_ = 0; // location of stmt
        MetadataVec _metadata;
        friend class Stmt;
    private:
        bool _match{false};
        std::string _cond;
        bool _deny{false};
        std::vector<std::string> _using_cols;
        NodeType _join_type{ E_STRING };
    };

    class BaseTableRef : public TableRef {
    public:
        BaseTableRef() { table_ref_type_ = BASE_TABLE_DIRECT_REF; }
        virtual std::string get_table_name() const override;
        virtual bool check_column(
                ResultPlan* plan,
                const std::string column_name,
                uint64_t& out_column_id) const override;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const override;
        virtual bool expand(
                ResultPlan* plan,
                std::vector<SelectItem*>& out_select_items,
                uint64_t start_index) override;
        virtual TableRef* clone() const override;

        /* Implement Of ITableItem */
        virtual TableItemType GetTableItemType() const override { return E_BASIC_TABLE; }
        virtual uint64_t GetTableID() const override { return base_table_id_; }
        virtual uint64_t GetQueryID() const override { return query_id_; }
        virtual std::string GetTableName() const override { return table_name_; }
        virtual std::string GetSchemaName() const override { return schema_name_; }
        virtual std::string GetDatabaseName() const override { return database_name_; }
        virtual std::string GetServerName() const override { return server_name_; }
        virtual std::string GetTableHint() const override { return table_hints_; }
        virtual std::string GetTableObject() const override { return table_object_; }
        virtual bool SetText( TableItemType tp, const std::string& newtable, const std::string& alias) override { return false; }
        virtual int GetLine() const override { return line_; }
        virtual int GetColumn() const override { return column_; }
        virtual Node* GetNode() const { return node_;  }

    public:
        void bind_node(ResultPlan* plan, Node* node);

    protected:
        uint64_t base_table_id_=0;
        std::string table_name_;
        std::string schema_name_;
        bool default_schema_{false};
        std::string database_name_;
        bool default_database_{false};
        std::string server_name_;
        std::string table_hints_;
        std::string table_object_;
        int line_ = 0;
        int column_ = 0;
        Node* node_ = NULL;
    friend class Stmt;
    friend class SqlRawExpr;
    friend class UpdateStmt;
    friend class DeleteStmt;
    friend class InsertStmt;
    friend class DropTableStmt;
    friend class BaseTableAliasRef;
    friend int resolve_update_clause(
            ResultPlan* plan,
            Node* node,
            UpdateStmt* parent
    );
    friend int resolve_delete_clause(
            ResultPlan* plan,
            Node* node,
            DeleteStmt* parent
    );
    friend int resolve_insert_clause(
            ResultPlan* plan,
            Node* node,
            InsertStmt* parent
    );
    };

    class BaseTableAliasRef: public BaseTableRef {
    public:
        BaseTableAliasRef() { table_ref_type_ = BASE_TABLE_ALIAS_REF; }
        virtual std::string get_table_name() const override;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const override;
        virtual TableRef* clone() const override;

        /* Implement Of ITableItem */
        virtual TableItemType GetTableItemType() const override { return E_BASIC_TABLE_WITH_ALIAS; }

        virtual bool SetText(
                TableItemType tp,
                const std::string& newtable,
                const std::string& alias) override { return false; }
    private:

        friend class Stmt;
    };

    class GeneratedTableRef: public TableRef {
    public:
        GeneratedTableRef() { table_ref_type_ = GENERATED_TABLE_REF; }
        ~GeneratedTableRef();
        virtual std::string get_table_name() const override;
        virtual bool check_column(
                ResultPlan* plan,
                const std::string column_name,
                uint64_t& out_column_id) const override;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const override;
        virtual bool expand(
                ResultPlan* plan,
                std::vector<SelectItem*>& out_select_items,
                uint64_t start_index) override;
        virtual TableRef* clone() const override;
        bool set_column_alias(
                ResultPlan*plan,
                const std::vector<std::string>& col_alias);

    private:
        std::vector<SelectItem*> cols_;
        uint64_t ref_query_id_ = 0;  // link of selectstmt
        friend class Stmt;
        friend class SqlRawExpr;
    };

    class FunctionTableRef: public TableRef {
    public:
        FunctionTableRef(const std::string& name):_func_name(name) { table_ref_type_ = FUNCTION_TABLE_REF; }
        ~FunctionTableRef();
        virtual std::string get_table_name() const override;
        virtual bool check_column(
                ResultPlan* plan,
                const std::string column_name,
                uint64_t& out_column_id) const override;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const override;
        virtual bool expand(
                ResultPlan* plan,
                std::vector<SelectItem*>& out_select_items,
                uint64_t start_index) override;
        virtual TableRef* clone() const override;
    private:
        friend class Stmt;
        friend class SqlRawExpr;
        std::string _func_name;
    };

    class CteTableRef: public TableRef {
    public:
        CteTableRef() { table_ref_type_ = CTE_TABLE_REF; }
        virtual std::string get_table_name() const override;
        virtual bool check_column(
                ResultPlan* plan,
                const std::string column_name,
                uint64_t& out_column_id) const override;
        virtual bool check_is_ref(
                const std::string& schema,
                const std::string& table) const override;
        virtual bool expand(
                ResultPlan* plan,
                std::vector<SelectItem*>& out_select_items,
                uint64_t start_index) override;
        virtual TableRef* clone() const override;
        std::string GetCteName() const { return cte_name_; }
    private:
        std::string cte_name_;
        uint64_t cte_at_query_id_=0;
        uint64_t cte_index_ = 0;
        friend class Stmt;
        friend class SqlRawExpr;
        friend class UpdateStmt;
    };

    class Stmt;
    class CteDef {
    public:
        ~CteDef();
        bool set_column_alias(
                ResultPlan*plan,
                const std::vector<std::string>& col_alias);
        const std::vector<SelectItem*>& get_select_items() const { return cols_; }
    private:
        std::vector<SelectItem*> cols_;
        std::string cte_def_name_;
        uint64_t ref_query_id_ = 0; // link of selectstmt
        bool is_recursive_cte_{false};
        friend class Stmt;
        friend int resolve_cte(
                ResultPlan* plan,
                Node* node,
                Stmt* parent,
                uint64_t& out_table_id);
        friend class SqlRawExpr;
    };

}


#endif