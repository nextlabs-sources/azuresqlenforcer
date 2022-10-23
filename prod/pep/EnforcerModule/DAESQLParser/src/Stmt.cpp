#include "Stmt.h"
#include "ResultPlan.h"
#include "LogicPlan.h"
#include "LocalTableMgr.h"
#include "node.h"
#include "SelectStmt.h"
#include <assert.h>
#include "TableRef.h"
#include "SelectItem.h"
#include "join.h"
#include "commfun.h"

namespace resolve {
    Stmt::~Stmt() {
        for (auto it : cte_defs_)
            delete(it);
        cte_defs_.clear();
        for (auto it : table_items_)
            delete(it);
        table_items_.clear();
        for (auto it : joins_)
            delete(it);
        joins_.clear();
    }

    bool Stmt::check_in_cte(
            const std::string& table_name,
            uint64_t& out_query_id,
            size_t& out_index) {
        assert(table_name.length() > 0);
        Stmt* cur = this;
        while (cur != nullptr) {
            // reverse traversal to find the nearby cte definition
            size_t index = cur->cte_defs_.size() - 1;
            for (auto rit = cur->cte_defs_.rbegin(); rit != cur->cte_defs_.rend(); ++rit) {
                CteDef* cte = *rit;
                if (CommonFun::CaseInsensitiveEquals(cte->cte_def_name_, table_name)) {
                    out_query_id = cur->query_id_;
                    out_index = index;
                    if (cte->ref_query_id_ == OB_INVALID_ID) {
                        /*
                         * this means we are still resolving the cte definition,
                         * it also means that this cte is a recursive definition
                         * */
                        cte->is_recursive_cte_ = true;
                    }
                    return true;
                }
                --index;
            }
            /*
             * If no match in this WITH clause of 'select', maybe this is a subquery,
             * so look up in the outer query's WITH clause
             * */
            cur = cur->parent_;
        }
        return false;
    }

    TableRef* Stmt::add_table_item_func(
            ResultPlan* plan,
            const std::string& func_name,
            const std::string& alias_name,
            const std::string& alias_real,
            uint64_t& out_table_id,
            Node* node) {
        FunctionTableRef *ret = new FunctionTableRef(func_name);
        table_items_.push_back(ret);
        ret->alias_name_ = alias_name;
        ret->alias_real_ = alias_real;
        out_table_id = ret->table_id_;
        return  ret;
    }

    TableRef* Stmt::add_table_item(
            ResultPlan* plan,
            const std::string& schema_name,
            const std::string& table_name,
            const std::string& alias_name,
            const std::string& alias_real,
            uint64_t& out_table_id,
            Node* node) {
        assert(table_name.length() > 0);
        TableRef* ret = nullptr;

        uint64_t out_query_id = OB_INVALID_ID;
        size_t out_index = 0;
        bool is_cte = false;
        if (schema_name.length() > 0)
            is_cte = false;
        else
            is_cte = check_in_cte(table_name, out_query_id, out_index);
        if (is_cte) {
            CteTableRef* tb = new CteTableRef;
            tb->table_id_ = plan->logicPlan_->generate_table_id();
            tb->query_id_ = this->query_id_;
            tb->cte_name_ = table_name;
            tb->cte_at_query_id_ = out_query_id;
            tb->cte_index_ = out_index;
            if (alias_name.length() > 0) {
                tb->alias_name_ = alias_name;
                tb->alias_real_ = alias_real;
            }

            ret = tb;
        }
        else {
            std::string schema_name1 = schema_name.length() > 0 ? schema_name : plan->local_table_mgr->get_default_schema();
            if (alias_name.length() > 0) {
                BaseTableAliasRef* tb = new BaseTableAliasRef;
                tb->table_id_ = plan->logicPlan_->generate_table_id();
                tb->query_id_ = this->query_id_;
                tb->base_table_id_ = plan->local_table_mgr->get_local_table_id(schema_name1+"."+table_name);
                tb->table_name_ = table_name;
                tb->schema_name_ = schema_name1;
                /*todo*/
                assert(node != nullptr);
                tb->bind_node(plan, node);
                tb->alias_name_ = alias_name;
                tb->alias_real_ = alias_real;
                ret = tb;
            }
            else {
                BaseTableRef* tb = new BaseTableRef;
                tb->query_id_ = this->query_id_;
                tb->base_table_id_ = plan->local_table_mgr->get_local_table_id(schema_name1+"."+table_name);
                tb->table_id_ = tb->base_table_id_;
                tb->table_name_ = table_name;
                tb->schema_name_ = schema_name1;
                /*todo*/
                assert(node != nullptr);
                tb->bind_node(plan, node);
                ret = tb;
            }
        }

        for (TableRef* it : table_items_) {
            if (CommonFun::CaseInsensitiveEquals(ret->get_table_name(), it->get_table_name())) {
                /* ambiguous table name at from clause error */
                plan->error_detail_ = "ambiguous name " + ret->get_table_name();
                plan->errorOccur_(plan);
            }
        }

        table_items_.push_back(ret);
        out_table_id = ret->table_id_;
       // plan->base_table_visit_(plan, ret);
        return ret;
    }

    TableRef* Stmt::add_table_item(
            ResultPlan* plan,
            uint64_t query_id,
            const std::string& alias_name,
            const std::string& alias_real,
            uint64_t& out_table_id) {
        GeneratedTableRef* ret = new GeneratedTableRef;
        ret->table_id_ = query_id;
        ret->query_id_ = this->query_id_;
        ret->ref_query_id_ = query_id;
        assert(alias_name.length() > 0);
        ret->alias_name_ = alias_name;
        ret->alias_real_ = alias_real;

        for (TableRef* it : table_items_) {
            if (CommonFun::CaseInsensitiveEquals(ret->get_table_name(), it->get_table_name())) {
                /* ambiguous table name at from clause error */
                plan->error_detail_ = "ambiguous name " + ret->get_table_name();
                plan->errorOccur_(plan);
            }
        }

        table_items_.push_back(ret);
        out_table_id = ret->table_id_;
        return ret;
    }

    CteDef* Stmt::add_cte_def(
            ResultPlan* plan,
            uint64_t query_id,
            const std::string& cte_name) {
        CteDef* ret = new CteDef;
        ret->cte_def_name_ = cte_name;
        ret->ref_query_id_ = query_id;
        cte_defs_.push_back(ret);
        return ret;
    }

    bool Stmt::get_table_item(
            const std::string& schema,
            const std::string& table_name,
            TableRef*& out_table_ref) {
        assert(table_name.length() > 0);

        Stmt* cur = this;
        while (cur != nullptr) {
            for (TableRef* tbi : cur->table_items_) {
                if (tbi->check_is_ref(schema, table_name)) {
                    out_table_ref = tbi;
                    return true;
                }
            }
            /*
             * Since correlated subquery is supported, if no match in this query,
             * maybe this is a subquery, so look up in the outer query
             * */
            cur = cur->parent_;
        }
        return false;
    }


    /*
     * todo
     * a subquery at from list not check outer stmt
     * SELECT * FROM SALES N, (SELECT N.Qty) N1  error invalid N.Qty
     * */
    bool Stmt::get_table_item(
            uint64_t table_id,
            TableRef*& out_table_ref) {
        Stmt* cur = this;
        while (cur != nullptr) {
            for (TableRef* tbi : cur->table_items_) {
                if (tbi->table_id_ == table_id) {
                    out_table_ref = tbi;
                    return true;
                }
            }
            /*
             * Since correlated subquery is supported, if no match in this query,
             * maybe this is a subquery, so look up in the outer query
             * */
            cur = cur->parent_;
        }
        return false;
    }


    /*
     * todo
     * a subquery at from list not check outer stmt
     * SELECT * FROM SALES N, (SELECT N.Qty) N1  error invalid N.Qty
     * */
    bool Stmt::check_table_column(
            ResultPlan* plan,
            const std::string& column_name,
            TableRef*& table_item,
            uint64_t& column_id) {
        assert(column_name.length() > 0);
        Stmt* cur = this;
        while (cur != nullptr) {
            /*
             * should not have ambiguous column
             * currently, scanf left-to-right return first hit
             * */

            for (TableRef* tbi : cur->table_items_) {
                if (tbi->check_column(plan, column_name, column_id)) {/*todo*/
                    table_item = tbi;
                    return true;
                }
            }
            /*
             * Since correlated subquery is supported, if no match in this query,
             * maybe this is a subquery, so look up in the outer query
             * */
            cur = cur->parent_;
        }
        return false;
    }

    bool Stmt::find_tableref_by_column(
            ResultPlan* plan,
            const std::string& schema,
            const std::string& table,
            const std::string& column_name,
            TableRef*& table_item,
            uint64_t& column_id) {
        bool ret = get_table_item(schema, table, table_item);
        if (!ret)
            return false;
        ret = table_item->check_column(plan, column_name, column_id);
        if (!ret)
            return false;
        return true;
    }
    ITableItem* Stmt::GetTableItem(size_t index) {
        return 0 <= index && index < table_items_.size() ? table_items_[index] : nullptr;
    }

    bool Stmt::DenyTables(std::vector<ITableItem*>& tbis, const std::string& denied_schema_name) {
        bool ret = false;
        for (ITableItem* tbi : tbis) {
            TableRef* tbref = dynamic_cast<TableRef*> (tbi);
            if (tbref == NULL) {
                continue;
            }

            Node* node = NULL;
            Node* ntable = NULL;
            if (tbref->GetTableRefType() == TableRef::BASE_TABLE_DIRECT_REF) {
                BaseTableRef* tbitem = dynamic_cast<BaseTableRef*> (tbi);
                if (tbitem == NULL) {
                    continue;
                }
                node = tbitem->GetNode();
                if (node->nodeType_ == E_ALIAS)
                    node = node->getChild(E_ALIAS_ORIGN);
                if (node->GetType() == E_EXPR_LIST_WITH_PARENS)
                    node = Node::remove_parens(node);
                if (node && node->GetType() == E_TABLE_IDENT) {
                    ntable = node->getChild(0);
                    
                }
            }
            else if( tbref->GetTableRefType() == TableRef::BASE_TABLE_ALIAS_REF) {
                BaseTableAliasRef* tbitem = dynamic_cast<BaseTableAliasRef*> (tbi);
                if (tbitem == NULL) {
                    continue;
                }
                node = tbitem->GetNode();
                if (node->nodeType_ == E_ALIAS)
                    node = node->getChild(E_ALIAS_ORIGN);
                if (node->GetType() == E_EXPR_LIST_WITH_PARENS)
                    node = Node::remove_parens(node);
                if (node && node->GetType() == E_TABLE_IDENT) {
                    ntable = node->getChild(0);

                }
            }
            if (ntable && ntable->GetType() == E_IDENTIFIER) {
                node->isTerminalToken = true;
                node->terminalToken_.str = denied_schema_name + "." + ntable->terminalToken_.str;
                node->terminalToken_.yytex = denied_schema_name + "." + ntable->terminalToken_.yytex;
                ret = true;
            }
        }
        return ret;
    }
}