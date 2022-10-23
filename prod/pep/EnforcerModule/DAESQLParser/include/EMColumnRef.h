//
// Created by jeff on 2020/4/18.
//

#ifndef EMDB_EMCOLUMNREF_H
#define EMDB_EMCOLUMNREF_H

#include "resolve.h"
#include "keydef.h"
#include <vector>
#include <string>
#include "Interface.h"

class EMColumnInfo {
public:
    EMColumnInfo():_node(NULL),_alias(false),_questmarkid(0){}
    EMColumnInfo(Node * node):_node(node),_alias(false),_questmarkid(0){}
    virtual ~EMColumnInfo(){};
    Node * _node;
    std::string _db;
    std::string _schema;
    std::string _tb;
    std::string _field;
    bool _alias;
    unsigned  int _questmarkid;
};

namespace resolve {
    struct TableColumnRef {
        uint64_t table_id_;
        uint64_t column_id_;
    };
//base class
    class RawExpr {
    public:
        RawExpr() : type_(E_NODETYPE_BEGIN) {}
        virtual ~RawExpr() {}
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref){}
        NodeType get_expr_type() const { return type_; }
        void set_expr_type(NodeType type) { type_ = type; }
    private:
        NodeType type_;
    };

    class ConstRawExpr : public RawExpr {
    public:
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref){}
    private:
        union {
            int i;
            double num;
            const char* text;
        } value_{0};
    };

    class RawExprScalarSubquery : public RawExpr {
    public:
        uint64_t get_ref_id() const { return query_id_; }
        void set_ref_id(uint64_t id) { query_id_ = id; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        uint64_t query_id_=0;
    };

    class RawExprBinaryRef : public RawExpr {
    public:
        ~RawExprBinaryRef(){ }
        uint64_t get_first_ref_id() const { return first_id_; }
        void set_first_ref_id(uint64_t id) { first_id_ = id; }
        uint64_t get_second_ref_id() const { return second_id_; }
        void set_second_ref_id(uint64_t id) { second_id_ = id; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
        
        void SetTbItems(const std::vector<const ITableItem*> &table_items){ table_items_ = table_items; }
        const std::vector<const ITableItem*>  GetTbItems(){ return table_items_;}

        void SetColumnValue(const EMColumnInfo & col){ _colinfo = col;}

        EMColumnInfo & GetColInfo(){ return _colinfo;}
        void SetAlias(Node * node){
            _colinfo._node = node;
            _colinfo._alias = true;
        }
    private:
        uint64_t first_id_=0;     // table_id
        uint64_t second_id_=0;    // column_id
        EMColumnInfo _colinfo;
        std::vector<const ITableItem*> table_items_;

    };

    class RawExprUnaryOp : public RawExpr {
    public:
        RawExprUnaryOp():expr_(NULL){}
        ~RawExprUnaryOp(){ delete(expr_);}
        RawExpr* get_op_expr() { return expr_; }
        void set_op_expr(RawExpr* expr) { expr_ = expr; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        RawExpr* expr_;
    };

    class RawExprBinaryOp : public RawExpr {
    public:
        RawExprBinaryOp():first_expr_(NULL),second_expr_(NULL){}
        ~RawExprBinaryOp(){
            delete(first_expr_);
            delete(second_expr_);
        }
        RawExpr* get_first_op_expr() { return first_expr_; }
        void set_first_op_expr(RawExpr* expr) { first_expr_ = expr; }
        RawExpr* get_second_op_expr() { return second_expr_; }
        void set_second_op_expr(RawExpr* expr) { second_expr_ = expr; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        RawExpr* first_expr_;
        RawExpr* second_expr_;
    };

    class RawExprTripleOp : public RawExpr { // three expr
    public:
        RawExprTripleOp():first_expr_(NULL),second_expr_(NULL),third_expr_(NULL){}
        ~RawExprTripleOp(){
            delete(first_expr_);
            delete(second_expr_);
            delete(third_expr_);
        }
        RawExpr* get_first_op_expr() { return first_expr_; }
        void set_first_op_expr(RawExpr* expr) { first_expr_ = expr; }
        RawExpr* get_second_op_expr() { return second_expr_; }
        void set_second_op_expr(RawExpr* expr) { second_expr_ = expr; }
        RawExpr* get_third_op_expr() { return third_expr_; }
        void set_third_op_expr(RawExpr* expr) { third_expr_ = expr; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        RawExpr* first_expr_;
        RawExpr* second_expr_;
        RawExpr* third_expr_;
    };

    class RawExprMultiOp : public RawExpr {
    public:
        ~RawExprMultiOp(){
            for(auto it:exprs_){ delete(it); }
        }
        RawExpr* get_op_expr(size_t index)
        {
            return 0 <= index && index < exprs_.size() ?  exprs_[index] : nullptr;
        }
        void add_op_expr(RawExpr* expr) { exprs_.push_back(expr); }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
        size_t get_expr_len(){ return exprs_.size();}
    private:
        std::vector<RawExpr*> exprs_;
    };

    class RawExprCaseOp : public RawExpr {
    public:
        RawExprCaseOp():arg_expr(NULL),default_expr_(NULL){}
        ~RawExprCaseOp(){
            delete(arg_expr);
            delete(default_expr_);
            for(auto it:when_exprs_) delete(it);
            when_exprs_.clear();
            for(auto it:then_exprs_) delete(it);
            then_exprs_.clear();
        }
        RawExpr* get_arg_op_expr() { return arg_expr; }
        void set_arg_op_expr(RawExpr* expr) { arg_expr = expr; }
        RawExpr* get_default_expr() { return default_expr_; }
        void set_default_expr(RawExpr* expr) { default_expr_ = expr; }
        RawExpr* get_when_op_expr(size_t index)
        {
            return 0 <= index && index < when_exprs_.size() ? when_exprs_[index] : nullptr;
        }
        void add_when_op_expr(RawExpr* expr) { when_exprs_.push_back(expr); }
        RawExpr* get_then_op_expr(size_t index)
        {
            return 0 <= index && index < then_exprs_.size() ? then_exprs_[index] : nullptr;
        }
        size_t get_when_len(){ return when_exprs_.size();}
        size_t get_then_len(){ return then_exprs_.size();}
        void add_then_op_expr(RawExpr* expr) { then_exprs_.push_back(expr); }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        RawExpr* arg_expr;
        std::vector<RawExpr*> when_exprs_;
        std::vector<RawExpr*> then_exprs_;
        RawExpr* default_expr_;
    };

    /*
     * https://en.wikipedia.org/wiki/Aggregate_function
     * */
    class RawExprAggFun : public RawExpr {
    public:
        RawExprAggFun():param_expr_(NULL){}
        ~RawExprAggFun(){ delete(param_expr_); }
        RawExpr* get_param_expr() { return param_expr_; };
        void set_param_expr(RawExpr* param_expr) { param_expr_ = param_expr; }
        bool get_distinct() { return distinct_; }
        void set_distinct(bool distinct) { distinct_ = distinct; }
        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        RawExpr* param_expr_;   // null means '*'
        bool distinct_=false;
    };

    class RawExprSysFun : public RawExpr {
    public:
        ~RawExprSysFun(){for(auto it: param_exprs_) delete(it);}
        std::string get_func_name() { return func_name_; }
        void set_func_name(const std::string& func_name) { func_name_ = func_name; }
        void add_param_expr(RawExpr* param_expr) { param_exprs_.push_back(param_expr); }
        RawExpr* get_param_expr(size_t index) {
            return 0 <= index && index < param_exprs_.size() ? param_exprs_[index] : nullptr;
        }
        size_t get_func_len(){ return param_exprs_.size();}

        virtual void scanf_table_column_ref(LogicPlan* logic, std::vector<TableColumnRef>& out_table_column_ref);
    private:
        std::string func_name_;
        std::vector<RawExpr*> param_exprs_;
    };

    /////
    class SqlRawExpr {
    public:
        ~SqlRawExpr(){ delete(expr_); }
        uint64_t get_expr_id() const { return expr_id_; }
        void set_expr_id(uint64_t expr_id) { expr_id_ = expr_id; }
        uint64_t get_query_id() const { return query_id_; }
        void set_query_id(uint64_t query_id) { query_id_ = query_id; }
        RawExpr* get_expr() { return expr_; }
        void set_expr(RawExpr* raw_expr) { expr_ = raw_expr; }
        void debug(ResultPlan* plan, int lvl = 0);
        bool check_base_field_ref(ResultPlan* plan);
    private:
        static void debug1(ResultPlan* plan, uint64_t query_id, uint64_t table_id, uint64_t column_id, int lvl);
        static bool check_base_field_ref1(ResultPlan* plan, uint64_t query_id, uint64_t table_id, uint64_t column_id);
    public:
        std::string expr_content;   // for debug info
    private:
        uint64_t query_id_;
        uint64_t expr_id_;
        uint64_t table_id_;
        uint64_t column_id_;
        RawExpr* expr_;
    };

    class AsAliasExpr : public RawExpr {
    public:
        AsAliasExpr():_node(NULL),_expr(NULL){}
        ~AsAliasExpr(){if(_expr) delete(_expr);}
        void SetNode(Node * node) { _node = node; }
        void SetAlias(const std::string & str){ _alias = str; }
        void SetExpr(RawExpr * expr) { _expr = expr;}
        RawExpr* GetExpr(){ return _expr;}
    private:
        Node * _node;
        std::string _alias;
        RawExpr* _expr;
    };

    struct ReplaceExpr{
        Node* _node = nullptr;
        std::map<std::string,Node*, IgnoreCaseCmp> _col2node;
    };
    struct ExceptExpr{
        Node * _node = nullptr;
        std::vector<std::string> _cols;
    };

    class AsteriskExpr : public RawExpr {
    public:
        AsteriskExpr(Node * node):_node(node){}
        void AddTbItem(const ITableItem* table_item){ table_items_.push_back(table_item); }
        const std::vector<const ITableItem*>  GetTbItems(){ return table_items_;}
        Node * GetNode(){ return _node;}
    public:
        std::string _database;
        std::string _schema;
        std::string _table;
        ExceptExpr _exceptexpr;
        ReplaceExpr _replace;
    private:
        std::vector<const ITableItem*> table_items_;
        Node * _node;
    };

    class ArrayDefExpr: public RawExpr{
    public:
        ArrayDefExpr(){};
        ~ArrayDefExpr(){for(auto it:_arr_expr) delete(it);}
        void add_param_expr(RawExpr* param_expr) { _arr_expr.push_back(param_expr); }
        RawExpr* get_param_expr(size_t index) {
            return 0 <= index && index < _arr_expr.size() ? _arr_expr[index] : nullptr;
        }
        size_t get_array_len(){ return _arr_expr.size();}
        void set_param_type(unsigned int type){_param_type = type;}
    public:
        std::vector<RawExpr*> _arr_expr;
        unsigned int _param_type=0;
    };

    class StructDefExpr: public RawExpr{
    public:
        StructDefExpr(){};
        ~StructDefExpr(){for(auto it:_struct_expr) delete(it);}
        void add_param_expr(RawExpr* param_expr) { _struct_expr.push_back(param_expr); }
        RawExpr* get_param_expr(size_t index) {
            return 0 <= index && index < _struct_expr.size() ? _struct_expr[index] : nullptr;
        }
        size_t get_array_len(){ return _struct_expr.size();}
    public:
        std::vector<RawExpr*> _struct_expr;
        std::vector<unsigned int> _param_types;//todo parser struct data type
        bool _hav_datatype_def = false;
        bool _hav_struct_name = false;;
    };
}


#endif //EMDB_EMCOLUMNREF_H
