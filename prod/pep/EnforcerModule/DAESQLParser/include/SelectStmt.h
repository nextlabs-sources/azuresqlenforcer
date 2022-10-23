#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include <string>
#include <vector>
#include <algorithm>
#include "Interface.h"
#include "Stmt.h"


namespace resolve {
    class SelectItem;

    class SelectStmt : public Stmt, public ISelectStmt {
    public:
        SelectStmt():_select_items(NULL),_where_clause(NULL){ }
        virtual ~SelectStmt(){
            delete(_select_items);
            delete(_where_clause);
            for(auto it:_group_by){
                delete(it);
            }
        }
        //const std::vector<SelectItem*>& get_select_items() const { return select_items_; }
        bool add_select_item_expand_star(
                ResultPlan* plan,
                const std::string& schema,
                const std::string& table);
        bool add_select_item(uint64_t expr_id, const std::string& alias, bool is_real_alias);
        bool is_set_op() const;
        uint64_t get_set_op_left_query_id() const;
        uint64_t get_set_op_right_query_id() const;
        void set_set_op_left_query_id(uint64_t left_query_id);
        void set_set_op_right_query_id(uint64_t right_query_id);

        /*add by jeff*/
        void SetGroupByItems(std::vector<EMColumnInfo*> & group_by){   _group_by = group_by;    }
        const std::vector<EMColumnInfo*> & GetGroupByItems(){ return _group_by;}
        void SetSelectExpr(IColumnsRefItem * expr){_select_items = expr;}
        IColumnsRefItem * GetSelectExpr(){ return  _select_items;}
        void SetWhereClause(IWhereCluase * where_clause){_where_clause = where_clause;}
        IWhereCluase * GetWhereClause(){ return _where_clause;}


    private:
        uint64_t get_last_column_id() const;
        //std::vector<SelectItem*> select_items_;
        IColumnsRefItem * _select_items;
        IWhereCluase * _where_clause;
        std::vector<EMColumnInfo*> _group_by;
        uint64_t left_query_id_=0;
        uint64_t right_query_id_=0;

    public:
        virtual StmtType GetStmtType() override { return  E_STMT_TYPE_SELECT; }
    };
}

#endif