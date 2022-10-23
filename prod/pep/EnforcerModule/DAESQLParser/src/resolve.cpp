#include "resolve.h"
#include "node.h"
#include "EMColumnRef.h"
#include "SelectStmt.h"
#include "LogicPlan.h"
#include "ResultPlan.h"
#include <assert.h>
#include <list>
#include <regex>
#include "LocalTableMgr.h"
#include "TableRef.h"
#include "LogicPlan.h"
#include "where_clause.h"
#include "UpdateStmt.h"
#include "join.h"
#include "UseStmt.h"
#include "DeleteStmt.h"
#include "InsertStmt.h"
#include "CallStmt.h"
#include "EMDBConfig.h"
#include "CreateTableStmt.h"
#include "DropTableStmt.h"
#include "parser.h"
#include "commfun.h"

namespace resolve {
    int resolve_jointb_using_cols( Node* node, Stmt* parent, JoinedTable* jt);

    int resolve(ResultPlan* plan, Node* node, uint64_t& query_id) {
        assert(node != nullptr);
        assert(plan != nullptr);

        switch (node->nodeType_) {
            case E_STMT_LIST: {
                resolve_multi_statements(plan, node);
            } break;
            case E_DIRECT_SELECT: {
                query_id = OB_INVALID_ID;
                resolve_select_statement(plan, node, query_id);
            } break;
            case E_CREATE_TABLE: {
                query_id = OB_INVALID_ID;
                resolve_create_table_statement(plan, node, query_id);
            } break;
            case E_DROP_TABLE: {
                query_id = OB_INVALID_ID;
                resolve_drop_table_statement(plan, node, query_id);
            } break;
            case E_UPDATE: {
                query_id = OB_INVALID_ID;
                resolve_update_statement(plan, node, query_id);
            } break;
            case E_DELETE: {
                query_id = OB_INVALID_ID;
                resolve_delete_statement(plan, node, query_id);
            } break;
            case E_INSERT: {
                query_id = OB_INVALID_ID;
                resolve_insert_statement(plan, node, query_id);
            } break;
            case E_USE: {
                resolve_special_statement(plan,node,query_id);
            } break;
            case E_CALL: {
                query_id = OB_INVALID_ID;
                resolve_call_statement(plan, node, query_id);
            } break;
            case E_SET_HANA: {
                resolve_special_statement(plan, node,query_id);
            } break;
            case EORA_ALTER_SESSION: {
                resolve_special_statement(plan, node,query_id);
            } break;
            case E_STMT_BODYS: {
                resolve(plan, node->getChild(0),query_id);
            } break;
            case E_SP_EXECUTESQL: {
              resolve_sp_executesql_statement(plan, node, query_id);
            } break;
            default:
                //assert(false);  /*unreachable*/
                break;
        }
        return 0;
    }

    int resolve_multi_statements(ResultPlan* plan, Node* node) {
        std::list<Node*> stmts;
        Node::ToList(node, stmts);
        uint64_t query_id = OB_INVALID_ID;
        for (auto stmt : stmts) {
            plan->reset();
            resolve(plan, stmt, query_id);
            plan->startNewStmt_(plan, query_id);
        }
        return 0;
    }

    int resolve_special_statement(
            ResultPlan *plan,
            Node *node,
            uint64_t& query_id){
        query_id = OB_INVALID_ID;
        EMDB_DB_TYPE tp = plan->GetDatabaseType();
        switch(tp) {
            case EMDB_DB_HANA:{
                if(node->GetType() == E_SET_HANA){
                    query_id = plan->logicPlan_->generate_query_id();
                    UseStmt *use_stmt = dynamic_cast<UseStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_USE));
                    std::string key = node->getChild(0)->terminalToken_.str;
                    if(CommonFun::CaseInsensitiveEquals(key, "SCHEMA")){
                        use_stmt->set_schema_name(node->getChild(1)->terminalToken_.str);
                    }
                    use_stmt->set_query_id(query_id);
                    use_stmt->set_db_type(EMDB_DB_HANA);
                    plan->base_stmt_visit_(plan, use_stmt);/*add bu jeff*/
                }
            } break;
            case EMDB_DB_ORACLE:{
                if(node->GetType() == EORA_ALTER_SESSION){
                    query_id = plan->logicPlan_->generate_query_id();
                    UseStmt *use_stmt = dynamic_cast<UseStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_USE));
                    std::string key = node->getChild(0)->terminalToken_.str;
                    if(CommonFun::CaseInsensitiveEquals(key, "CURRENT_SCHEMA")){
                        use_stmt->set_schema_name(node->getChild(1)->terminalToken_.str);
                    }
                    use_stmt->set_query_id(query_id);
                    use_stmt->set_db_type(EMDB_DB_ORACLE);
                    plan->base_stmt_visit_(plan, use_stmt);/*add bu jeff*/
                }
            } break;
            case EMDB_DB_SQLSERVER:{
                if(node->nodeType_ == E_USE){
                    query_id = plan->logicPlan_->generate_query_id();
                    UseStmt* use_stmt = dynamic_cast<UseStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_USE));
                    use_stmt->set_query_id(query_id);
                    use_stmt->set_parent(NULL);
                    Node* db = node->getChild(E_USE_DATABASE_NAME);
                    assert(db != nullptr);
                    use_stmt->set_database_name(db->terminalToken_.str);
                    plan->local_table_mgr->set_default_database(use_stmt->get_database_name());
                    use_stmt->set_db_type(EMDB_DB_SQLSERVER);
                    plan->base_stmt_visit_(plan, use_stmt);/*add bu jeff*/
                }
            } break;
            default:{

            } break;
        }
        return 0;
    }

    int resolve_call_statement(
            ResultPlan *plan,
            Node *node,
            uint64_t& query_id,
            Stmt *parent/* = nullptr */,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_CALL);
        query_id = plan->logicPlan_->generate_query_id();
        CallStmt *call_stmt = dynamic_cast<CallStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_CALL));
        call_stmt->set_query_id(query_id);
        call_stmt->set_parent(parent);

        Node *sp_node = node->getChild(E_CALL_SP_NAME);
        assert(sp_node != nullptr);
        assert(sp_node->nodeType_ == E_TABLE_IDENT);

        Node *server_node = sp_node->getChild(E_TABLE_IDENT_SERVER);
        Node *database_node = sp_node->getChild(E_TABLE_IDENT_DATABASE);
        Node *schema_node = sp_node->getChild(E_TABLE_IDENT_SCHEMA);
        Node *sp = sp_node->getChild(E_TABLE_IDENT_OBJECT);
        assert(sp != nullptr);
        std::string sp_name = sp->terminalToken_.str;
        std::string schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
        std::string db_name = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
        std::string sv_name = server_node ? server_node->terminalToken_.str : "";

        call_stmt->set_store_procedure(sv_name, db_name, schema_name, sp_name);
/*
        Node *args = node->getChild(E_CALL_SQL_ARG_LIST);
        std::list<Node*> ls;
        Node::ToList(args, ls);
        for (Node *expr : ls) {
            uint64_t sql_raw_expr_id = OB_INVALID_ID;
            resolve_independ_expr(NULL, expr, sql_raw_expr_id, parent);
        }*/
        return 0;
    }

    int resolve_use_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {

        return 0;
    }

    int resolve_update_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_UPDATE);
        query_id = plan->logicPlan_->generate_query_id();
        UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_UPDATE));
        update_stmt->set_query_id(query_id);
        update_stmt->set_parent(parent);
        update_stmt->SetNode(node);

        resolve_cte_clause(plan, node->getChild(E_UPDATE_OPT_WITH), update_stmt);
        resolve_from_clause(plan, node->getChild(E_UPDATE_FROM_LIST), update_stmt);
        resolve_update_clause(plan, node->getChild(E_UPDATE_UPDATE_RELATION), update_stmt);
        resolve_where_clause(plan, node->getChild(E_UPDATE_OPT_WHERE), node, update_stmt);
        resolve_update_items(plan, node->getChild(E_UPDATE_UPDATE_ELEM_LIST), update_stmt);
        plan->base_stmt_visit_(plan,update_stmt);/*add by jeff*/

        return 0;
    }

    int resolve_create_table_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_CREATE_TABLE);
        query_id = plan->logicPlan_->generate_query_id();
        CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_CREATE_TABLE));
        create_table_stmt->set_query_id(query_id);
        create_table_stmt->set_parent(parent);
        create_table_stmt->SetNode(node);

        uint64_t table_id = OB_INVALID_ID;
        resolve_table(plan, node->getChild(E_CREATE_TABLE_TABLE_NAME), create_table_stmt, table_id);
        plan->base_stmt_visit_(plan, create_table_stmt);

        return 0;
    }

    int resolve_drop_table_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_DROP_TABLE);
        query_id = plan->logicPlan_->generate_query_id();
        DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_DROP_TABLE));
        drop_table_stmt->set_query_id(query_id);
        drop_table_stmt->set_parent(parent);
        drop_table_stmt->SetNode(node);

        resolve_drop_table_clause(plan, node->getChild(E_DROP_TABLE_TABLE_LIST), drop_table_stmt);
        plan->base_stmt_visit_(plan, drop_table_stmt);/*add by jeff*/

        return 0;
    }

    int resolve_insert_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_INSERT);
        query_id = plan->logicPlan_->generate_query_id();
        InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_INSERT));
        insert_stmt->set_query_id(query_id);
        insert_stmt->set_parent(parent);
        insert_stmt->SetNode(node);

        resolve_insert_clause(plan, node->getChild(E_INSERT_INSERT_RELATION), insert_stmt);
        Node* cols_and_src = node->getChild(E_INSERT_INSERT_COLUMNS_AND_SOURCE);
        assert(cols_and_src != nullptr);
        Node* src = cols_and_src->getChild(1);
        assert(src != nullptr);
        uint64_t sql_raw_expr_id = OB_INVALID_ID;
        resolve_independ_expr(plan, src, sql_raw_expr_id, parent);
        plan->base_stmt_visit_(plan, insert_stmt);/*add by jeff*/

        return 0;
    }

    int resolve_delete_statement(
            ResultPlan* plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {
        assert(node->nodeType_ == E_DELETE);
        query_id = plan->logicPlan_->generate_query_id();
        DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_DELETE));
        delete_stmt->set_query_id(query_id);
        delete_stmt->set_parent(parent);
        delete_stmt->SetNode(node);

        resolve_cte_clause(plan, node->getChild(E_DELETE_OPT_WITH), delete_stmt);
        resolve_from_clause(plan, node->getChild(E_DELETE_FROM_LIST), delete_stmt);
        resolve_delete_clause(plan, node->getChild(E_DELETE_DELETE_RELATION), delete_stmt);
        resolve_where_clause(plan, node->getChild(E_DELETE_OPT_WHERE), node, delete_stmt);
        plan->base_stmt_visit_(plan, delete_stmt);/*add bu jeff*/

        return 0;
    }


    int resolve_select_statement(
            ResultPlan *plan,
            Node* node,
            uint64_t& query_id,
            Stmt* parent/* = nullptr*/,
            ScopeType scope/* = E_SCOPE_WHATEVER*/) {

        Node *with = nullptr;
        Node *select_clause = node;
        Node *order_by = nullptr;
        Node * for_update = nullptr;
        if (node->nodeType_ == E_DIRECT_SELECT) {
            with = node->getChild(E_DIRECT_SELECT_WITH);
            select_clause = node->getChild(E_DIRECT_SELECT_SELECT_CLAUSE);
            order_by = node->getChild(E_DIRECT_SELECT_ORDER);
            for_update = node->getChild(3);
        }

        if (select_clause->nodeType_ == E_SELECT_WITH_PARENS) {
            select_clause = Node::remove_parens(select_clause);
            if (select_clause->nodeType_ == E_DIRECT_SELECT) {
                // with = select_clause->getChild(E_DIRECT_SELECT_WITH);
                order_by = select_clause->getChild(E_DIRECT_SELECT_ORDER);
                select_clause = select_clause->getChild(E_DIRECT_SELECT_SELECT_CLAUSE);
                for_update = node->getChild(3);
            }
        }



        assert(select_clause->nodeType_ == E_SELECT);
        query_id = plan->logicPlan_->generate_query_id();
        SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(plan->logicPlan_->add_query(E_STMT_TYPE_SELECT));
        select_stmt->set_query_id(query_id);
        select_stmt->set_parent(parent);
        select_stmt->SetNode(node);

        resolve_cte_clause(plan, with, select_stmt);

        Node* set_op = select_clause->getChild(E_SELECT_SET_OPERATION);
        if (set_op != nullptr) {
            /*select with set operation*/
            Node* former = select_clause->getChild(E_SELECT_FORMER_SELECT_STMT);
            uint64_t left_query_id = OB_INVALID_ID;
            resolve_select_statement(plan, former, left_query_id, select_stmt);
            select_stmt->set_set_op_left_query_id(left_query_id);
            Node* latter = select_clause->getChild(E_SELECT_LATER_SELECT_STMT);
            uint64_t  right_query_id = OB_INVALID_ID;
            resolve_select_statement(plan, latter, right_query_id, select_stmt);
            select_stmt->set_set_op_right_query_id(right_query_id);
        }
        else {
            /*simple select*/
            resolve_from_clause(plan, select_clause->getChild(E_SELECT_FROM_LIST), select_stmt);
            resolve_select_items(plan, select_clause->getChild(E_SELECT_SELECT_EXPR_LIST), select_stmt, scope);
            resolve_where_clause(plan, select_clause->getChild(E_SELECT_OPT_WHERE), select_clause, select_stmt);
            resolve_group_by_clause(plan, select_clause->getChild(E_SELECT_GROUP_BY), select_stmt);
            resolve_having_clause(plan, select_clause->getChild(E_SELECT_HAVING), select_stmt);
            if(select_stmt->GetTableItemCount() > 0){
                plan->base_stmt_visit_(plan, select_stmt);
            }

        }
        if(order_by && order_by->nodeType_ == E_ORDER_BY ){
            resolve_order_by_clause(plan, order_by, select_stmt);
        }
        if(for_update && plan->GetDatabaseType() == EMDB_DB_DB2){ 
            //If "FOR UPDATE" is in front, switch "FOR UPDATE" and "LIMIT"     (DB2 error SQL0511N)(EMDB BUG:66674)
            if(for_update->GetType() == E_SPECIAL_CLAUSE_LIST){
                std::list<Node*> ls;
                Node::ToList(for_update, ls);
                Node * limit = NULL;
                //Node * limit_ = NULL;
                for (Node *c : ls) {
                    if(!c) continue;
                    if( c->GetType() == E_LIMIT_NUM){
                        limit = c;
                    } else if (c->GetType() == E_FOR_UPDATE && limit != NULL){
                        c->swap(limit);
                    }
                    
                }
            }
        }



        return 0;
    }

    bool parser_group_by(Node * node, std::vector<EMColumnInfo*>  &vec){
        if(NULL == node) return false;
        switch (node->nodeType_) {
            case E_GROUP_BY: {
                //0
                parser_group_by(node->getChild(0),  vec);
            } break;
            case E_EXPR_LIST: {
                std::list<Node*> ls;
                Node::ToList(node, ls);
                for(auto ptnode:ls){
                    parser_group_by(ptnode,  vec);
                }
            } break;
            case E_OP_NAME_FIELD: {
                EMColumnInfo *info = new EMColumnInfo(node);
                parser_db_field( node, info->_db, info->_schema, info->_tb, info->_field);
                info->_alias = false;
                if(!info->_field.empty()) {
                    vec.push_back(info);
                }else {
                    delete(info);
                }
            } break;
            case E_ARRAY_INDEX:{
                parser_group_by(node->getChild(0),  vec);
            } break;
            case E_EXPR_LIST_WITH_PARENS: {
                //0
                parser_group_by(node->getChild(0),  vec);
            } break;
            case E_FUN_CALL: {
                auto fun_name_node = node->getChild(0);
                auto fun_name = fun_name_node->serialize();

                if (fun_name.find("STRUCTURED RESULT") != std::string::npos)
                {
                    // To fix bug 65386
                    // Push a column info instance with invalid field to mask all.
                    vec.push_back(new EMColumnInfo());
                }
                else
                {
                    parser_group_by(node->getChild(1), vec);
                    parser_group_by(node->getChild(3), vec);
                }
            } break;
            default:
                break;
        }
        return true;
    }

    int resolve_group_by_clause(
            ResultPlan *plan,
            Node *node,
            Stmt *parent) {
        if (!node)
            return 0;
        assert(node->nodeType_ == E_GROUP_BY);
//        Node *expr_list = node->getChild(E_GROUP_BY_EXPR_LIST);
//        assert(expr_list);
//        std::list<Node*> ls;
//        Node::ToList(expr_list, ls);
//        for (Node *expr : ls) {
//            uint64_t sql_raw_expr_id = OB_INVALID_ID;
//            resolve_independ_expr(plan, expr, sql_raw_expr_id, parent);
//        }
        std::vector<EMColumnInfo*>  vec; // only collect invalid info.
        parser_group_by(node, vec);
        dynamic_cast<ISelectStmt*>(parent)->SetGroupByItems(vec);
        return 0;
    }

    int resolve_having_clause(
            ResultPlan *plan,
            Node *node,
            Stmt *parent) {
        if (!node)
            return 0;
        assert(node->nodeType_ == E_HAVING);
        Node *expr = node->getChild(E_HAVING_EXPR);
        assert(expr);
        uint64_t sql_raw_expr_id = OB_INVALID_ID;
        resolve_independ_expr(plan, expr, sql_raw_expr_id, parent);
        return 0;
    }

    int resolve_order_by_clause(
            ResultPlan *plan,
            Node *node,
            Stmt *parent) {
        if (!node)
            return 0;
        assert(node->nodeType_ == E_ORDER_BY);
        Node *sort_list = node->getChild(E_ORDER_BY_SORT_LIST);
        assert(sort_list);
        std::list<Node*> ls;
        Node::ToList(sort_list, ls);
        for (Node *sort_key : ls) {
            assert(sort_key->nodeType_ == E_SORT_KEY);
            Node *expr = sort_key->getChild(E_SORT_KEY_EXPR);
            uint64_t sql_raw_expr_id = OB_INVALID_ID;
            resolve_independ_expr(plan, expr, sql_raw_expr_id, parent);
        }
        return 0;
    }

    int resolve_cte_clause(
            ResultPlan* plan,
            Node* node,
            Stmt* parent) {
        if (!node)
            return 0;
        assert(node->nodeType_ == E_OPT_WITH_CLAUSE);
        Node* ctes = node->getChild(E_OPT_WITH_CLAUSE_CTE_LIST);
        std::list<Node*> ls;
        Node::ToList(ctes, ls);
        for (auto cte : ls)
        {
            uint64_t table_id = OB_INVALID_ID;
            resolve_cte(plan, cte, parent, table_id);
        }
        return 0;
    }

    int resolve_cte(
            ResultPlan* plan,
            Node* node,
            Stmt* parent,
            uint64_t& out_table_id) {
        assert(node && node->nodeType_ == E_COMMON_TABLE_EXPR);
        Node* subquery = node->getChild(E_COMMON_TABLE_EXPR_SUBQUERY);
        assert(subquery && subquery->nodeType_ == E_SELECT_WITH_PARENS);
        subquery = Node::remove_parens(subquery);
        assert(subquery->nodeType_ == E_SELECT || subquery->nodeType_ == E_DIRECT_SELECT);
        uint64_t query_id = OB_INVALID_ID;

        Node* tb = node->getChild(E_COMMON_TABLE_EXPR_TABLE);
        assert(tb && tb->nodeType_ == E_IDENTIFIER);
        std::string table_name = tb->terminalToken_.str;
        std::string alias_name;
        /*
         * Since recursive common table expression is supported,
         * so call add_cte_item before resolve the cte definition
         *
         * */
        CteDef* ctedef = parent->add_cte_def(plan, query_id, table_name);
        resolve_select_statement(plan, subquery, query_id, parent);
        /*
         * Since recursive common table expression is supported,
         * so this cte's definition query_id need to be set up
         * */
        ctedef->ref_query_id_ = query_id;
        Node* alias_list = node->getChild(E_COMMON_TABLE_EXPR_COLUMNS);
        std::vector<std::string> aliass{};
        if (alias_list) {
            alias_list = Node::remove_parens(alias_list);
            std::list<Node*> ls;
            Node::ToList(alias_list, ls);
            for (Node* it : ls)
                aliass.push_back(it->terminalToken_.str);
        }
        ctedef->set_column_alias(plan, aliass);
        return 0;
    }

    int resolve_from_clause(
            ResultPlan* plan,
            Node* node,
            Stmt* parent) {
        if (!node)
            return 0;

        assert(node->nodeType_ == E_FROM_CLAUSE);
        node = node->getChild(E_FROM_CLAUSE_FROM_LIST);
        std::list<Node*> ls{};
        Node::ToList(node, ls);
        for (auto child_node : ls) {
            uint64_t table_id = OB_INVALID_ID;
            resolve_table(plan, child_node, parent, table_id);
            if (child_node->nodeType_ == E_JOINED_TABLE ||
                child_node->nodeType_ == E_JOINED_TABLE_WITH_PARENS) {
                parent->add_from_item(table_id, FromItem::FromItemType::E_JOINED);
            }
            else {
                parent->add_from_item(table_id, FromItem::FromItemType::E_NORMAL);
            }
        }
        return 0;
    }

    int resolve_where_clause(
            ResultPlan* plan,
            Node* node,
            Node* node_parent,
            Stmt* parent) {
        if (node) {
            assert(node->nodeType_ == E_WHERE_CLAUSE);
            Node* expr = node->getChild(E_WHERE_CLAUSE_EXPR);
            uint64_t sql_raw_expr_id = OB_INVALID_ID;
            resolve_independ_expr(plan, expr, sql_raw_expr_id, parent);
        }

        {
            WhereCluase * wc = new WhereCluase();
            wc->_dbtype = plan->GetDatabaseType();
            wc->query_id_ = parent->get_query_id();
            wc->bind(node, node_parent);
            switch (parent->GetStmtType()){
                case E_STMT_TYPE_SELECT:{
                    dynamic_cast<ISelectStmt*>(parent)->SetWhereClause(wc);
                } break;
                case E_STMT_TYPE_UPDATE:{
                    dynamic_cast<IUpdateStmt*>(parent)->SetWhereClause(wc);
                } break;
                case E_STMT_TYPE_DELETE:{
                    dynamic_cast<IDeleteStmt*>(parent)->SetWhereClause(wc);
                } break;
                case E_STMT_TYPE_INSERT:{

                } break;
                default:
                    break;
            }
            //plan->whereClauseVisit_(plan, &wc);
        }

        return 0;
    }

#ifdef COMPLETE_ANALYSIS
    int resolve_select_items(
            ResultPlan* plan,
            Node* node,
            SelectStmt* parent,
            ScopeType scope)
    {
        assert(node != nullptr);
        std::list<Node*> ls;
        Node::ToList(node, ls);

        for (auto child : ls)
        {
            assert(child->nodeType_ == E_PROJECT_STRING);
            Node* nd = child->getChild(E_PROJECT_STRING_EXPR_OR_ALIAS);
            std::string alias_name = "";
            switch (nd->nodeType_)
            {
                case E_ALIAS:
                {
                    uint64_t sql_raw_expr_id = OB_INVALID_ID;
                    Node* t = nd->getChild(E_ALIAS_RELATION_FACTOR_OR_SELECT_WITH_PARENS);
                    resolve_independ_expr(plan, t, sql_raw_expr_id, parent);
                    assert(sql_raw_expr_id != OB_INVALID_ID);
                    Node* alias_node = nd->getChild(E_ALIAS_RELATION_NAME);
                    assert(alias_node != nullptr);
                    parent->add_select_item(sql_raw_expr_id, alias_node->terminalToken_.str, true);
                }
                    break;
                case E_STAR:
                {
                    parent->add_select_item_expand_star(plan, "", "");
                }
                    break;
                default:
                {
                    // expr
                    if (nd->nodeType_ == E_OP_NAME_FIELD)
                    {
                        Node* field = nd->getChild(E_OP_NAME_FIELD_COLUMN_NAME);
                        Node* tb = nd->getChild(E_OP_NAME_FIELD_RELATION_NAME);
                        Node* schema = nd->getChild(E_OP_NAME_FIELD_SCHEMA_NAME);

                        if (field->nodeType_ == E_STAR)
                        {
                            assert(tb != nullptr);
                            std::string table_name = tb->terminalToken_.str;
                            std::string schema_name = schema ? schema->terminalToken_.str : "";
                            parent->add_select_item_expand_star(plan, schema_name, table_name);
                        }
                        else
                        {
                            uint64_t sql_raw_expr_id = OB_INVALID_ID;
                            resolve_independ_expr(plan, nd, sql_raw_expr_id, parent);
                            std::string column_name = field->terminalToken_.str;
                            parent->add_select_item(sql_raw_expr_id, column_name, false);
                        }
                    }
                    else
                    {
                        uint64_t sql_raw_expr_id = OB_INVALID_ID;
                        resolve_independ_expr(plan, nd, sql_raw_expr_id, parent);
                        parent->add_select_item(sql_raw_expr_id, nd->serialize(), false);   // this is a nameless field
                    }
                }
                    break;
            }
        }
        return 0;
    }
#else
    int resolve_select_items(
            ResultPlan* plan,
            Node* node,
            SelectStmt* parent,
            ScopeType scope) {
        RawExpr* e = nullptr;
        resolve_expr(plan, node, nullptr, parent, e);
        if(e){
            EMColumnsRef *ref = new EMColumnsRef(e);
            ref->SetDBType( plan->GetDatabaseType());
            ref->SetStmtId((int)parent->get_query_id());
            ref->SetStmt(parent);
            //plan->baseTableColumnVisit_(plan, &ref);
            parent->SetSelectExpr(ref);
        }

        return 0;
    }
#endif

    int resolve_update_clause(
            ResultPlan* plan,
            Node* node,
            UpdateStmt* parent) {
        assert(node);
        switch (node->nodeType_) {
            case E_TABLE_IDENT: {
                //Node *database_node = node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = node->getChild(E_TABLE_IDENT_OBJECT);
                assert(table_node != nullptr);
                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                TableRef* tbi = nullptr;
                parent->set_update_table(plan, E_TABLE_IDENT, schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF ) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
                        btbi->bind_node(plan, node);
//                        btbi->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        btbi->table_object_ = node->serialize();
//                        btbi->default_schema_ = (schema_node == nullptr);
//                        btbi->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        btbi->default_database_ = (database_node == nullptr);
                    }
                }

            } break;
            case E_ALIAS: {
                Node* body_node = node->getChild(E_ALIAS_ORIGN);
                Node* alias_node = node->getChild(E_ALIAS_ALIAS);

//                Node* database_node = body_node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = body_node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = body_node->getChild(E_TABLE_IDENT_OBJECT);

                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";

                TableRef* tbi = NULL;
                parent->set_update_table(plan, alias_node?E_ALIAS:E_TABLE_IDENT, schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF) {
                        BaseTableAliasRef* tba = dynamic_cast<BaseTableAliasRef*>(tbi);
                        tba->bind_node(plan, node);
//                        tba->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        tba->table_object_ = node->serialize();
//                        tba->default_schema_ = (schema_node == nullptr);
//                        tba->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        tba->default_database_ = (database_node == nullptr);
                        tba->alias_name_ = alias_name;
                        tba->alias_real_ = alias_real;
                    } else if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
                        btbi->bind_node(plan, body_node);
                    }
                }
            }   break;
            case E_TEMP_VARIABLE: { } break;
            default:
                break;  /* todo */
        }

        return 0;
    }

    int resolve_update_one_item(
            ResultPlan* plan,
            Node* node,
            UpdateStmt* parent  ){
        assert(node && parent);
        //E_OP_EQ   ( 0  =  1 )
        Node * field = node->getChild(0);
        if(field->GetType() == E_ARRAY_INDEX){
            field = field->getChild(0);
        }
        if(field->GetType() == E_OP_NAME_FIELD){
            //Node * val = node->getChild(1);
            EMColumnInfo info(field);
            parser_db_field( field, info._db, info._schema, info._tb, info._field);

            UpdateItem * item = new UpdateItem();
            item->_node = node;
            item->_field = info._field;
            item->_col = info;

            parent->add_update_item(item);
        }
        { /* subquery */
            uint64_t sql_raw_expr_id = OB_INVALID_ID;
            resolve_independ_expr(plan, node->getChild(1), sql_raw_expr_id, parent);
        }
        return 1;
    }

    int resolve_update_items(
            ResultPlan* plan,
            Node* node,
            UpdateStmt* parent) {
        assert(node);
        std::list<Node*> ls;
        Node::ToList(node, ls);
        for (auto it : ls) {
           // uint64_t sql_raw_expr_id = OB_INVALID_ID;
            resolve_update_one_item(plan, it,  parent);
        }
        return 0;
    }



    int resolve_delete_clause(
            ResultPlan* plan,
            Node* node,
            DeleteStmt* parent) {
        assert(node);
        switch (node->nodeType_) {
            case E_TABLE_IDENT: {
                //Node *database_node = node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = node->getChild(E_TABLE_IDENT_OBJECT);
                assert(table_node != nullptr);
                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                TableRef* tbi = nullptr;
                parent->set_delete_table(plan, E_TABLE_IDENT, schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF/* ||
                        tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF*/) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
//                        btbi->table_name_ = table_name;
//                        btbi->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        btbi->table_object_ = node->serialize();
//                        btbi->default_schema_ = (schema_node == nullptr);
//                        btbi->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        btbi->default_database_ = (database_node == nullptr);
                        btbi->bind_node(plan,node);
                    }
                }
            } break;
            case E_ALIAS: {
                Node* body_node = node->getChild(E_ALIAS_ORIGN);
                Node* alias_node = node->getChild(E_ALIAS_ALIAS);

                //Node* database_node = body_node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = body_node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = body_node->getChild(E_TABLE_IDENT_OBJECT);

                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";

                TableRef* tbi;
                parent->set_delete_table(plan, alias_node?E_ALIAS:E_TABLE_IDENT, schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF) {
                        BaseTableAliasRef* tba = dynamic_cast<BaseTableAliasRef*>(tbi);
//                        tba->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        tba->table_object_ = node->serialize();
//                        tba->default_schema_ = (schema_node == nullptr);
//                        tba->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        tba->default_database_ = (database_node == nullptr);
                        tba->bind_node(plan,node);
                        tba->alias_name_ = alias_name;
                        tba->alias_real_ = alias_real;
                    } else if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF/* ||
                        tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF*/) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
                        btbi->bind_node(plan, body_node);
                    }
                }
            }   break;
            case E_TEMP_VARIABLE: { } break;
            default:
                break;  /* todo */
        }
        return 0;
    }

    int resolve_insert_clause(
            ResultPlan* plan,
            Node* node,
            InsertStmt* parent) {
        assert(node);
        switch (node->nodeType_) {
            case E_TABLE_IDENT: {
//                Node *database_node = node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = node->getChild(E_TABLE_IDENT_OBJECT);
                assert(table_node != nullptr);
                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                TableRef* tbi = nullptr;
                parent->set_insert_table(plan, node->nodeType_,schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF ||
                        tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
//                        btbi->table_name_ = table_name;
//                        btbi->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        btbi->table_object_ = node->serialize();
//                        btbi->default_schema_ = (schema_node == nullptr);
//                        btbi->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        btbi->default_database_ = (database_node == nullptr);
                        btbi->bind_node(plan, node);
                    }
                }
            } break;
            case E_ALIAS: {
                Node* body_node = node->getChild(E_ALIAS_ORIGN);
                Node* alias_node = node->getChild(E_ALIAS_ALIAS);

//                Node* database_node = body_node->getChild(E_TABLE_IDENT_DATABASE);
                Node* schema_node = body_node->getChild(E_TABLE_IDENT_SCHEMA);
                Node* table_node = body_node->getChild(E_TABLE_IDENT_OBJECT);

                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";

                TableRef* tbi = NULL;
                parent->set_insert_table(plan, alias_node?E_ALIAS:E_TABLE_IDENT, schema_name, table_name, tbi);
                if (tbi) {
                    if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF) {
                        BaseTableAliasRef* tba = dynamic_cast<BaseTableAliasRef*>(tbi);
                        tba->bind_node(plan, node);
//                        tba->schema_name_ = schema_name = schema_node ? schema_node->terminalToken_.str : plan->local_table_mgr->get_default_schema();
//                        tba->table_object_ = node->serialize();
//                        tba->default_schema_ = (schema_node == nullptr);
//                        tba->database_name_ = database_node ? database_node->terminalToken_.str : plan->local_table_mgr->get_default_database();
//                        tba->default_database_ = (database_node == nullptr);
                        tba->alias_name_ = alias_name;
                        tba->alias_real_ = alias_real;
                    } else if (tbi->get_table_ref_type() == TableRef::BASE_TABLE_DIRECT_REF/* ||
                        tbi->get_table_ref_type() == TableRef::BASE_TABLE_ALIAS_REF*/) {
                        BaseTableRef* btbi = dynamic_cast<BaseTableRef*>(tbi);
                        btbi->bind_node(plan, body_node);
                    }
                }
            }   break;
            case E_TEMP_VARIABLE: { } break;
            default:
                break;  /* todo */
        }
        return 0;
    }

    int resolve_drop_table_clause(
            ResultPlan* plan,
            Node* node,
            DropTableStmt* parent) {
        assert(node);
        switch (node->nodeType_) {
            case E_RELATION_FACTOR_LIST: {
                std::list<Node*> ls;
                Node::ToList(node, ls);

                for (auto it : ls) {
                    uint64_t table_id = OB_INVALID_ID;
                    resolve_table(plan, it, parent, table_id);
                }
            } break;
            case E_TABLE_IDENT: {
                uint64_t table_id = OB_INVALID_ID;
                resolve_table(plan, node, parent, table_id);
            } break;
            default:
            break;
        }
        return 0;
    }

    int resolve_table(
            ResultPlan* plan,
            Node* node,
            Stmt* parent,
            uint64_t& out_table_id) {
        assert(node != nullptr);
        out_table_id = OB_INVALID_ID;
        Node* table_node = node;
        Node* alias_node = nullptr;
        Node* schema_node = nullptr;
        if (node->nodeType_ == E_ALIAS) {
            table_node = node->getChild(E_ALIAS_ORIGN);
            alias_node = node->getChild(E_ALIAS_ALIAS);
        }
        else if (node->nodeType_ == E_TABLE_IDENT) {
            table_node = node->getChild(E_TABLE_IDENT_OBJECT);
            schema_node = node->getChild(E_TABLE_IDENT_SCHEMA);
        }

        if (table_node->nodeType_ == E_EXPR_LIST_WITH_PARENS) {
            table_node = Node::remove_parens(table_node);
        }

        if (table_node->nodeType_ == E_SELECT_WITH_PARENS) {
            table_node = Node::remove_parens(table_node);
        }
        else if (table_node->nodeType_ == E_JOINED_TABLE_WITH_PARENS) {
            table_node = Node::remove_parens(table_node);
        }
        else if (table_node->nodeType_ == E_TABLE_IDENT) {
            schema_node = table_node->getChild(E_TABLE_IDENT_SCHEMA);
            table_node = table_node->getChild(E_TABLE_IDENT_OBJECT);
        }

        switch (table_node->nodeType_) {
            case E_IDENTIFIER: {
                
                std::string table_name = table_node->terminalToken_.str;
                std::string table_name2 = table_node->terminalToken_.yytex;
                if(table_name == table_name2){// no double quote
                    transform(table_name.begin(), table_name.end(), table_name.begin(), ::toupper);
                }
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";

                parent->add_table_item(plan, schema_name, table_name, alias_name,alias_real, out_table_id, node);
            } break;
            case E_DIRECT_SELECT:
            case E_SELECT: {
                //assert(alias_node != nullptr);  // The alias is actually not optional at all.
                std::string df = table_node->serialize();
                if (alias_node == nullptr) {
                    plan->set_err(0, "(" + df + ") must have alias name");
                    //plan->errorOccur_(plan);
                }

                uint64_t query_id = OB_INVALID_ID;
                resolve_select_statement(plan, table_node, query_id, parent, E_SCOPE_FROM);
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : df;
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
                TableRef* tbi = parent->add_table_item(plan, query_id, alias_name, alias_real, out_table_id);
                Node* alias_list = node->getChild(2);
                std::vector<std::string> aliass{};
                if (alias_list) {
                    alias_list = Node::remove_parens(alias_list);
                    std::list<Node*> ls;
                    Node::ToList(alias_list, ls);
                    for (Node* it : ls)
                        aliass.push_back(it->terminalToken_.str);
                }
                GeneratedTableRef* gtbi = dynamic_cast<GeneratedTableRef*>(tbi);
                assert(gtbi != nullptr);
                gtbi->set_column_alias(plan, aliass);
            } break;
            case E_JOINED_TABLE: {
                JoinedTable* joinedTable = new JoinedTable;
                out_table_id = parent->generate_join_id();
                joinedTable->set_join_id(out_table_id);
                resolve_joined_table(plan, table_node, parent, joinedTable);
                parent->add_join(joinedTable);
                resolve_jointb_using_cols(table_node,parent,joinedTable);

                //E_SIMPLE_IDENT_LIST_WITH_PARENS

            } break;
            case E_TABLE_VALUE_CTOR_PARENS:
                break;
            case E_FUN_CALL:{
                std::string funname = table_node->getChild(0)->Serialize();
                if(plan->GetDatabaseType() == EMDB_DB_ORACLE){
                    if(CommonFun::CaseInsensitiveEquals(funname, "ONLY") ||
                       CommonFun::CaseInsensitiveEquals(funname,"CONTAINERS") ||
                       CommonFun::CaseInsensitiveEquals(funname, "SHARDS")){
                        Node * node_tb = table_node->getChild(1);
                        if (node_tb->GetType() == E_TABLE_IDENT) {
                            table_node = node_tb->getChild(E_TABLE_IDENT_OBJECT);
                            schema_node = node_tb->getChild(E_TABLE_IDENT_SCHEMA);
                            if(table_node->GetType() == E_IDENTIFIER) {
                                std::string table_name = table_node->terminalToken_.str;
                                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
                                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";

                                parent->add_table_item(plan, schema_name, table_name, alias_name,alias_real, out_table_id, node_tb);
                            }
                        }
                    }
                } else {
                    std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                    std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
                    parent->add_table_item_func(plan, funname, alias_name,alias_real, out_table_id, table_node);
                }
            } break;
            case E_PIVOT_TABLE: {
                /*todo*/
                resolve_table(plan, table_node->getChild(0), parent, out_table_id);
            } break;
            case E_UNPIVOT_TABLE: {
                /*todo*/
                resolve_table(plan, table_node->getChild(0), parent, out_table_id);
            } break;
            case E_TEMP_VAR_FUN_CALL:
                /*todo*/
                break;
            case E_TEMP_VARIABLE:
                /*todo*/
                break;
            case E_UNNEST_DEF:
			case E_UNNEST_TABLE:{
                std::string funname = "UNNEST";
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
                parent->add_table_item_func(plan, funname, alias_name,alias_real, out_table_id, table_node);
            }
                break;
            case E_LATERAL_QUERY:{
                uint64_t sql_raw_expr_id = OB_INVALID_ID;
                resolve_independ_expr(plan, table_node->getChild(0), sql_raw_expr_id, parent);
				//
				std::string funname = "LATERAL";
				std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
				std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";
				parent->add_table_item_func(plan, funname, alias_name, alias_real, out_table_id, table_node);
            } break;
            case E_ASSOCIATED_TABLE:{
                auto associated_table_node = node->getChild(0);
                auto table_ident_node = associated_table_node->getChild(0);
                auto search_condition_node = associated_table_node->getChild(1);
                auto associated_ref_node = associated_table_node->getChild(2);

                uint64_t sql_raw_expr_id = OB_INVALID_ID;
                resolve_independ_expr(plan, search_condition_node, sql_raw_expr_id, parent);
                resolve_independ_expr(plan, associated_ref_node->getChild(1), sql_raw_expr_id, parent);

                table_node = table_ident_node->getChild(0);
                schema_node = table_ident_node->getChild(1);
                alias_node = alias_node ? alias_node : associated_ref_node->getChild(0);

                std::string table_name = table_node->terminalToken_.str;
                std::string schema_name = schema_node ? schema_node->terminalToken_.str : "";
                std::string alias_name = alias_node ? alias_node->terminalToken_.yytex : "";
                std::string alias_real = alias_node ? alias_node->terminalToken_.str : "";

                parent->add_table_item(plan, schema_name, table_name, alias_name, alias_real, out_table_id, table_ident_node);
            } break;
            case E_CASE_JOIN_TABLE: {
                auto case_join_node = table_node->getChild(2);
                auto case_join_when_node = case_join_node->getChild(0);
                std::list<Node*> ls;
                Node::ToList(case_join_when_node, ls);
                JoinedTable* joinedTable = new JoinedTable;

                std::list<Node*> ls_ret;
                Node * else_node = case_join_node->getChild(1); // else node
                if(else_node){
                  ls_ret.push_back(else_node->getChild(0));// ret node
                }
                for(auto when_node:ls) {
                  ls_ret.push_back( when_node->getChild(1));
                }

                for(auto ret_node: ls_ret) {
                  auto join_predicate_node = ret_node->getChild(2);
                  auto case_join_table_node = ret_node->getChild(1);

                  if (!case_join_table_node || case_join_table_node->GetType() != E_ALIAS) {
                    break;
                  }

                  auto table_ident_node = case_join_table_node->getChild(0);
                  auto table_name_node = table_ident_node->getChild(0);
                  auto table_schema_node = table_ident_node->getChild(1);
                  auto alias_name_node = case_join_table_node->getChild(1);

                  std::string table_name = table_name_node->terminalToken_.str;
                  std::string schema_name = table_schema_node ? table_schema_node->terminalToken_.str : "";
                  std::string alias_name = alias_name_node ? alias_name_node->terminalToken_.yytex : "";
                  std::string alias_real = alias_name_node ? alias_name_node->terminalToken_.str : "";

                  joinedTable->set_join_id(parent->generate_join_id());

                  parent->add_table_item(plan, schema_name, table_name, alias_name, alias_real, out_table_id, case_join_table_node);

                  if (out_table_id == OB_INVALID_ID) {
                    break;
                  }

                  joinedTable->add_table(E_CASE_JOIN_WHEN, out_table_id, join_predicate_node);
                }
                parent->add_join(joinedTable);
                resolve_table(plan, table_node->getChild(0), parent, out_table_id);
            } break;
            default:
                /*unreachable*/
                assert(false);
        }
        return 0;
    }

    int resolve_joined_table(
            ResultPlan* plan,
            Node* node,
            Stmt* parent,
            JoinedTable* joinedTable) {
        int ret = 0;
        assert(node->nodeType_ == E_JOINED_TABLE);
        Node* table_node = nullptr;
        Node * join_tp = node->getChild(E_JOINED_TABLE_JOIN_TYPE);
        NodeType join_type = join_tp?join_tp->GetType():E_STRING;
        for (size_t i = 0; i < node->getChildrenCount(); ++i) {
            table_node = node->getChild((int)i);
            if (E_JOINED_TABLE_TABLE_FACTOR_L == i ||
                E_JOINED_TABLE_TABLE_FACTOR_R == i) {
                switch (table_node->nodeType_) {
                    case E_IDENTIFIER:
                    case E_SELECT:
                    case E_ALIAS: {
                        uint64_t tid = OB_INVALID_ID;
                        ret = resolve_table(plan, table_node, parent, tid);
                        joinedTable->add_table(join_type,tid);
                    } break;
                    case E_JOINED_TABLE: {
                        ret = resolve_joined_table(plan, table_node, parent, joinedTable);
                    } break;
                    case E_SELECT_WITH_PARENS: {
                        table_node = Node::remove_parens(table_node);
                        uint64_t tid = OB_INVALID_ID;
                        ret = resolve_table(plan, table_node, parent, tid);
                        joinedTable->add_table(join_type,tid);
                    } break;
                    case E_JOINED_TABLE_WITH_PARENS: {
                        table_node = Node::remove_parens(table_node);
                        ret = resolve_joined_table(plan, table_node, parent, joinedTable);
                    } break;
                    default:
                        break;
                }
            }
        }
        uint64_t sql_raw_expr_id = OB_INVALID_ID;
        resolve_independ_expr(plan, node->getChild(E_JOINED_TABLE_ON_EXPR), sql_raw_expr_id, parent);
        //parser  on condition
        //ret tableitem  add  on

//        Node* join_type_node = node->getChild(E_JOINED_TABLE_JOIN_TYPE);
//        switch (join_type_node->nodeType_) {
//            case E_JOIN_FULL: {
//                joinedTable->add_join_op(E_JOIN_FULL, sql_raw_expr_id);
//            } break;
//            case E_JOIN_CROSS: {
//                joinedTable->add_join_op(E_JOIN_CROSS, OB_INVALID_ID);
//            } break;
//            case E_JOIN_LEFT: {
//                joinedTable->add_join_op(E_JOIN_LEFT, sql_raw_expr_id);
//            } break;
//            case E_JOIN_RIGHT: {
//                joinedTable->add_join_op(E_JOIN_RIGHT, sql_raw_expr_id);
//            } break;
//            case E_JOIN_INNER: {
//                joinedTable->add_join_op(E_JOIN_INNER, sql_raw_expr_id);
//            } break;
//            case E_JOIN_NATURAL: {
//                joinedTable->add_join_op(E_JOIN_NATURAL, OB_INVALID_ID);
//            } break;
//            default:
//                assert(false);  /* unreachable */
//                break;
//        }
        return ret;
    }

#ifdef COMPLETE_ANALYSIS
    int resolve_independ_expr(
            ResultPlan *plan,
            Node *node,
            uint64_t& sql_raw_expr_id,
            Stmt* parent)
    {
        SqlRawExpr* sql_expr = plan->logicPlan_->add_expr();
        sql_raw_expr_id = plan->logicPlan_->generate_sql_raw_expr_id();
        sql_expr->set_expr_id(sql_raw_expr_id);

        RawExpr* raw_expr = nullptr;
        resolve_expr(plan, node, sql_expr, parent, raw_expr);
        sql_expr->set_expr(raw_expr);
        assert(raw_expr != nullptr);
        sql_expr->set_query_id(parent->get_query_id());
        sql_expr->expr_content = node->serialize();
        printf("%s updateable %d :", sql_expr->expr_content.c_str(), sql_expr->check_base_field_ref(plan) ? 1 : 0);
        sql_expr->debug(plan);
        printf("\n");
        return 0;
    }
#else
    int resolve_independ_expr(
            ResultPlan *plan,
            Node *node,
            uint64_t& sql_raw_expr_id,
            Stmt* parent) {
        if (!node)
            return 0;
        const uint32_t opr = EMDBConfig::GetInstance().get_sqluser_format_o();
        NodeType tpcom = E_SPECIAL_TERMINAL;
        if (opr == 0) tpcom = E_OP_NE;
        else if (opr == 1) tpcom = E_OP_EQ;

        if (node->nodeType_ == E_SELECT || node->nodeType_ == E_DIRECT_SELECT) {
            uint64_t query_id = OB_INVALID_ID;
            resolve_select_statement(plan, node, query_id, parent);
            return 0;
        }
        else if (tpcom == node->nodeType_) {

            Node* childL = node->getChild(0);
            Node* childR = node->getChild(1);

            do {
                if (childL && childR) {
					if ((USER_SOURCE_SQL_USER & EMDBConfig::GetInstance().get_user_source()) == 0) {
						break;
					}
                    if (!childR->IsTerminal()) {
                        break;
                    }
                    //const std::string& r = EMDBConfig::GetInstance().get_sqluser_format_r();
                    std::string l_com;
                    if (childL->GetType() == E_OP_NAME_FIELD) {
                        l_com = childL->getChild(0)->terminalToken_.str;
                    }
                    else if (childL->GetType() == E_STRING) {
                        l_com = childL->Serialize();
                    }
                    const std::string& l = EMDBConfig::GetInstance().get_sqluser_format_l();
                    if (IgnoreCaseCmp::compare(l_com, l) == 0) {
                        IStmt::SQL_USER_VALUE val;
                        if (childR->GetType() == EORA_PARAM || childR->GetType() == E_QUESTIONMARK) {
                            val._type = IStmt::SQL_USER_VALUE::ValueType::TYPE_PARAM_ID;
                            val._val = std::to_string(childR->questmarkid_);
                        }
                        else if (childR->GetType() == E_STRING) {
                            val._type = IStmt::SQL_USER_VALUE::ValueType::TYPE_STRING;
                            val._val = SqlCommon::EscapeQuotationSqlString(childR->terminalToken_.str);// childR->terminalToken_.str;
                        }
                        else if (childR->GetType() == E_INT || childR->GetType() == E_DOUBLE) {
                            val._type = IStmt::SQL_USER_VALUE::ValueType::TYPE_NUMBER;
                            val._val = childR->Serialize();
                        }
                        if (!val._val.empty()) {
                            parent->SetSqlUserValue(val);
                        }
                        //
                        if (!EMDBConfig::GetInstance().get_sql_user_keep()) {
                            Node* node_s = Node::makeTerminalNode(E_STRING, "'99999'");
                            (*childL) = (*node_s);
                            delete(node_s);
                            if (E_OP_EQ == node->nodeType_) {
                                node->serialize_format = &OP_NE_SERIALIZE_FORMAT;
                            }
                        }

                    }
                }
            } while (0);
        }
        //if (node->nodeType_ == E_SELECT || node->nodeType_ == E_DIRECT_SELECT) {
        //    uint64_t query_id = OB_INVALID_ID;
        //    resolve_select_statement(plan, node, query_id, parent);
        //    return 0;
        //}
        //else if(E_OP_NE == node->nodeType_) {
        //    Node* childL = node->getChild(0);
        //    Node* childR = node->getChild(1);
        //    if (childL && childR && childL->IsTerminal() && childR->IsTerminal()) {

        //        const std::string sctx_flag = "'DAE_APP_USER'";
        //        if (IgnoreCaseCmp::compare(sctx_flag, childL->Serialize()) == 0) {
        //            if (childR->GetType() == EORA_PARAM || childR->GetType() == E_QUESTIONMARK) {
        //                parent->SetSessionContextParamId(childR->questmarkid_);
        //            }
        //            else if (childR->GetType() == E_STRING) {
        //                parent->SetSessionContextValue(SqlCommon::EscapeQuotationSqlString( childR->terminalToken_.str) );
        //            }
        //        }
        //        
        //        if (IgnoreCaseCmp::compare(sctx_flag, childR->Serialize()) == 0) {
        //            
        //            if (childL->GetType() == EORA_PARAM || childL->GetType() == E_QUESTIONMARK) {
        //                parent->SetSessionContextParamId(childL->questmarkid_);

        //            }
        //            else if (childL->GetType() == E_STRING) {
        //                parent->SetSessionContextValue(SqlCommon::EscapeQuotationSqlString(childR->terminalToken_.str));
        //            }
        //        }
        //        
        //        
        //    }
        //}
        /*
         * we donot resolve expr, just find subquery and resolve it
         * */

        for (size_t i = 0; i < node->getChildrenCount(); ++i) {
            Node* child = node->getChild((int)i);
            if (child)
                resolve_independ_expr(plan, child, sql_raw_expr_id, parent);
        }
        sql_raw_expr_id = OB_INVALID_ID;
        return 0;
    }
#endif

    int resolve_expr(
            ResultPlan* plan,
            Node* node,
            SqlRawExpr* sql_raw_expr,
            Stmt* parent,
            RawExpr*& out_raw_expr) {
        assert(node != nullptr);
        switch (node->nodeType_) {
            case E_NULL:
            case E_INT:
            case E_DOUBLE:
            case E_BOOL:
            case E_TEMP_VARIABLE:
            case E_QUESTIONMARK:
            case E_STRING: {
                ConstRawExpr* expr = new ConstRawExpr;
                expr->set_expr_type(node->nodeType_);
                out_raw_expr = expr;
            } break;
            case E_OP_NAME_FIELD: {
                // this is a column_ref   *,XXX.* illeagal
                EMColumnInfo info(node);
                parser_db_field( node, info._db, info._schema, info._tb, info._field);

                if(info._field.empty()){
                    break;
                }

                uint64_t table_id = OB_INVALID_ID;
                uint64_t column_id = OB_INVALID_ID;
                TableRef* tbi = nullptr;
                 std::vector<const ITableItem*> tbs;
                if (info._tb.empty()) {
                    bool fd = parent->check_table_column(plan, info._field, tbi, column_id);
                    if (!fd) {
                        //assert(false);
                    }
                    if(tbi){
                        tbs.push_back(tbi);
                        table_id = tbi->get_table_id();
                    } else{
                        const std::vector<TableRef*>& tbstemp = parent->get_table_items();
                        for(auto tb : tbstemp) tbs.push_back(tb);
                    }
                }
                else {
                    bool fd = parent->find_tableref_by_column(plan, info._schema, info._tb, info._field, tbi, column_id);
                    if (!fd) {
                       // assert(false);
                    }
                   if(tbi){
                       tbs.push_back(tbi);
                       table_id = tbi->get_table_id();
                   } else {
                       // if  column hav qualify  then must be hav table
                   }
                }

                RawExprBinaryRef* expr = new RawExprBinaryRef;
                expr->set_expr_type(node->nodeType_);
                expr->set_first_ref_id(table_id);
                expr->set_second_ref_id(column_id);
                expr->SetColumnValue(info);

                expr->SetTbItems(tbs);
                out_raw_expr = expr;
            } break;
            case E_EXPR_LIST_WITH_PARENS: {
                Node* nd = Node::remove_parens(node);
                resolve_expr(plan, nd, sql_raw_expr, parent, out_raw_expr);
            } break;
            case E_EXPR_LIST: {
                std::list<Node*> ls;
                Node::ToList(node, ls);
                RawExprMultiOp* expr = new RawExprMultiOp;
                expr->set_expr_type(E_EXPR_LIST);
                for (auto it : ls) {
                    RawExpr* e = nullptr;
                    resolve_expr(plan, it, sql_raw_expr, parent, e);
                    expr->add_op_expr(e);
                }
                out_raw_expr = expr;
            } break;
            case E_SELECT_EXPR_LIST: {
                std::list<Node*> ls;
                Node::ToList(node, ls);
                RawExprMultiOp* expr = new RawExprMultiOp;
                expr->set_expr_type(E_SELECT_EXPR_LIST);
                for (auto it : ls) {
                    RawExpr* e = nullptr;
                    resolve_expr(plan, it, sql_raw_expr, parent, e);
                    expr->add_op_expr(e);
                }
                out_raw_expr = expr;
            } break;
            case E_PROJECT_STRING: {
                resolve_expr(plan, node->getChild(0), sql_raw_expr, parent, out_raw_expr);
            } break;
            case E_ALIAS: {//aaa AS bbb
                resolve_expr(plan, node->getChild(0), sql_raw_expr, parent, out_raw_expr);
                Node * aslias = node->getChild(1);
                if(out_raw_expr && out_raw_expr->get_expr_type() == E_OP_NAME_FIELD) {
                    ((RawExprBinaryRef*)out_raw_expr)->SetAlias(node);
                }
                assert(aslias);
                AsAliasExpr * as_ = new AsAliasExpr();
                as_->set_expr_type(E_ALIAS);
                as_->SetNode(node);
                as_->SetAlias(aslias->serialize());
                as_->SetExpr(out_raw_expr);
                out_raw_expr = as_;
                
            }  break;
            case E_STAR: {//aaa AS bbb
                AsteriskExpr * expr = new AsteriskExpr(node);
                expr->set_expr_type(E_ASTERISK_EXPR);
                const std::vector<TableRef*>& tbs = parent->get_table_items();
                for(auto item: tbs){
                    expr->AddTbItem(item);
                }
                //
                out_raw_expr = expr;
            }  break;
            case E_ASTERISK_QUALIFY:{
                AsteriskExpr * expr = new AsteriskExpr(node);
                expr->set_expr_type(E_ASTERISK_EXPR);
                expr->_table = node->getChild(2)->terminalToken_.str;

                std::string qualify = expr->_table;
                if(!qualify.empty()){
                    size_t s = parent->GetTableItemCount();
                    for(size_t i = 0 ; i < s; ++i){
                        ITableItem* it = parent->GetTableItem(i);
                        if(!it->GetTableAliasReal().empty()){
                            if( IgnoreCaseCmp::compare(it->GetTableAliasReal(), qualify) ==0){
                                expr->AddTbItem(it);
                                break;
                            }
                        }
                        if(!it->GetTableName().empty()){
                            if( IgnoreCaseCmp::compare(it->GetTableName(), qualify) == 0){
                                expr->AddTbItem(it);
                                break;
                            }
                        }
                    }
                }
                //
                out_raw_expr = expr;
            } break;
            case E_ASTERISK_EXPR: {
                AsteriskExpr * expr = new AsteriskExpr(node);
                expr->set_expr_type(E_ASTERISK_EXPR);
                Node * star = node->getChild(0);
                Node * except_node = node->getChild(1);
                Node * replace_node = node->getChild(2);
                if(star->nodeType_ == E_STAR){
                    const std::vector<TableRef*>& tbs = parent->get_table_items();
                    for(auto item: tbs){
                        expr->AddTbItem(item);
                    }

                } else if(star->nodeType_ == E_ASTERISK_QUALIFY){
                    //star = star->getChild(3);
                    Node * node_t = star->getChild(2);
                    if(node_t){   // code in parser_bigquery_field
                        std::string table_name = node_t->terminalToken_.str;
                        std::string table_name2 = node_t->terminalToken_.yytex;
                        if(table_name == table_name2){// no double quote
                            transform(table_name.begin(), table_name.end(), table_name.begin(), ::toupper);
                        }
                        std::size_t ifind = table_name.find('.');
                        if(ifind != std::string::npos){
                            std::string sub1 = table_name.substr(ifind+1);
                            std::size_t ifind2 = sub1.find('.');
                            if(ifind2 != std::string::npos){
                                expr->_database = table_name.substr(0, ifind);
                                expr->_schema = sub1.substr(0, ifind2);
                                expr->_table = sub1.substr(ifind2+1);
                            } else {
                                expr->_schema = table_name.substr(0, ifind);
                                expr->_table = sub1;
                            }
                        } else {
                            expr->_table = table_name;
                        }
                    }
                    //expr->_table =node_t?node_t->terminalToken_.str:"";
//                    Node* node_s = star->getChild(1);
//                    expr->_schema =node_s?node_s->terminalToken_.str:"";
//                    Node * node_db = star->getChild(0);
//                    expr->_database =node_db?node_db->terminalToken_.str:"";

                    std::string qualify = expr->_table;
                    if(!qualify.empty()){
                        size_t s = parent->GetTableItemCount();
                        for(size_t i = 0 ; i < s; ++i){
                            ITableItem* it = parent->GetTableItem(i);
                            if(!it->GetTableAliasReal().empty()){
                                if(IgnoreCaseCmp::compare(it->GetTableAliasReal(), qualify)==0){
                                    expr->AddTbItem(it);
                                    break;
                                }
                            }
                            if(!it->GetTableName().empty()){
                                if(IgnoreCaseCmp::compare(it->GetTableName(), qualify) == 0){
                                    expr->AddTbItem(it);
                                    break;
                                }
                            }
                        }
                    }
                }
                if(except_node){//E_EXCEPT_EXPR
                    expr->_exceptexpr._node = except_node;
                    Node*list =Node::remove_parens(except_node->getChild(0));
                    std::list<Node*> ls;
                    Node::ToList(list, ls);
                    for(auto it:ls){
                        expr->_exceptexpr._cols.push_back(it->terminalToken_.str);
                    }
                }
                if(replace_node){//E_REPLACE_EXPR
                    expr->_replace._node = replace_node;
                    std::list<Node*> ls;
                    Node::ToList(replace_node->getChild(0), ls);//E_ROW_EXPR_AS_LABEL_LIST
                    for(auto it:ls){//E_ROW_EXPR_AS_LABEL
                        Node * label = it->getChild(1);//
                        if(label){
                            expr->_replace._col2node.insert(std::make_pair(label->terminalToken_.str, it));
                        }
                    }
                }
                //
                out_raw_expr = expr;
            }break;
            case E_SELECT_WITH_PARENS: {
                Node* nd = Node::remove_parens(node);
                resolve_expr(plan, nd, sql_raw_expr, parent, out_raw_expr);
            } break;
            case E_DIRECT_SELECT:
            case E_SELECT: {
                uint64_t query_id = OB_INVALID_ID;
                resolve_select_statement(plan, node, query_id, parent);
                RawExprScalarSubquery* expr = new RawExprScalarSubquery;
                expr->set_expr_type(E_SELECT);
                expr->set_ref_id(query_id);
                out_raw_expr = expr;
            } break;
            case E_OP_ADD:
            case E_OP_MINUS:
            case E_OP_MUL:
            case E_OP_DIV:
            case E_OP_REM:
            case E_OP_POW:
            case E_OP_MOD:
            case E_OP_LE:
            case E_OP_LT:
            case E_OP_EQ:
            case E_OP_GE:
            case E_OP_GT:
            case E_OP_NE:
            case E_OP_LIKE:
            case E_OP_NOT_LIKE:
            case E_OP_AND:
            case E_OP_AND_SPC:
            case E_OP_OR:
            case E_OP_IS:
            case E_OP_IS_NOT:
            case E_OP_IN:
            case E_OP_NOT_IN:
            case E_OP_CNN:
            case E_OP_ASS:
            case E_OP_ASS_ADD:
            case E_OP_ASS_MINUS:
            case E_OP_ASS_MUL:
            case E_OP_ASS_DIV:
            case E_OP_ASS_REM:
            case E_OP_ASS_BIT_AND:
            case E_OP_ASS_BIT_OR:
            case E_OP_ASS_BIT_XOR:
            case E_OP_COLLATE: {
                RawExpr* l = nullptr;
                RawExpr* r = nullptr;
                resolve_expr(plan, node->getChild(E_OP_BINARY_OPERAND_L), sql_raw_expr, parent, l);
                resolve_expr(plan, node->getChild(E_OP_BINARY_OPERAND_R), sql_raw_expr, parent, r);
                RawExprBinaryOp* expr = new RawExprBinaryOp;
                expr->set_expr_type(node->nodeType_);
                expr->set_first_op_expr(l);
                expr->set_second_op_expr(r);
                out_raw_expr = expr;
            } break;
            case E_OP_NOT:
            case E_OP_EXISTS:
            case E_OP_POS:
            case E_OP_NEG: {
                RawExpr* r = nullptr;
                resolve_expr(plan, node->getChild(E_OP_UNARY_OPERAND), sql_raw_expr, parent, r);
                RawExprUnaryOp* expr = new RawExprUnaryOp;
                expr->set_expr_type(node->nodeType_);
                expr->set_op_expr(r);
                out_raw_expr = expr;
            } break;
            case E_OP_BTW:
            case E_OP_NOT_BTW: {
                RawExpr* a = nullptr;
                RawExpr* b = nullptr;
                RawExpr* c = nullptr;
                resolve_expr(plan, node->getChild(E_OP_TERNARY_OPERAND_1), sql_raw_expr, parent, a);
                resolve_expr(plan, node->getChild(E_OP_TERNARY_OPERAND_2), sql_raw_expr, parent, b);
                resolve_expr(plan, node->getChild(E_OP_TERNARY_OPERAND_3), sql_raw_expr, parent, c);
                RawExprTripleOp* expr = new RawExprTripleOp;
                expr->set_expr_type(node->nodeType_);
                expr->set_first_op_expr(a);
                expr->set_second_op_expr(b);
                expr->set_third_op_expr(c);
                out_raw_expr = expr;
            } break;
            case E_CASE: {
                /*
                RawExprCaseOp* expr = new RawExprCaseOp;
                expr->set_expr_type(E_CASE);
                RawExpr* arg = nullptr;
                resolve_expr(plan, node->getChild(E_CASE_ARG), sql_raw_expr, parent, arg);
                expr->set_arg_op_expr(arg);
                Node* deft = node->getChild(E_CASE_ELSE);
                if (!deft)
                    expr->set_default_expr(nullptr);
                else {
                    RawExpr* e = nullptr;
                    resolve_expr(plan, deft->getChild(E_CASE_DEFAULT_EXPR), sql_raw_expr, parent, e);
                    expr->set_default_expr(e);
                }

                Node* when = node->getChild(E_CASE_WHEN_CLAUSE_LIST);
                std::list<Node*> ls;
                Node::ToList(when, ls);
                for (auto it : ls) {
                    assert(it->nodeType_ == E_WHEN);
                    RawExpr* w = nullptr;
                    resolve_expr(plan, it->getChild(E_WHEN_WHEN_EXPR), sql_raw_expr, parent, w);
                    expr->add_when_op_expr(w);
                    RawExpr* t = nullptr;
                    resolve_expr(plan, it->getChild(E_WHEN_THEN_EXPR), sql_raw_expr, parent, t);
                    expr->add_then_op_expr(t);
                }
                out_raw_expr = expr;*/
            } break;
            case E_FUN_CALL: {
                Node* func = node->getChild(E_FUN_CALL_FUNC_NAME);
                Node* params = node->getChild(E_FUN_CALL_PARAMS);
                RawExprSysFun* expr = new RawExprSysFun;
                expr->set_expr_type(E_FUN_CALL);
                expr->set_func_name(func->serialize());
                std::list<Node*> ls;
                Node::ToList(params, ls);
                for (auto it : ls) {
                    RawExpr* param = nullptr;
                    resolve_expr(plan, it, sql_raw_expr, parent, param);
                    expr->add_param_expr(param);
                }
                out_raw_expr = expr;
            } break;
//            case E_EXPR_LIST_WITH_BRACKETS:{
//                Node * nd = node->getChild(0);
//                resolve_expr(plan, nd, sql_raw_expr, parent, out_raw_expr);
//            } break;
            case E_ARRAY_INDEX:{
                resolve_expr(plan, node->getChild(0), sql_raw_expr, parent, out_raw_expr);
            } break;
            case E_ARRAY_DEF:{
                //todo array member data type parser  node->getChild(1)
                ArrayDefExpr * expr = new ArrayDefExpr;
                expr->set_expr_type(E_ARRAY_DEF);
                Node * params = node->getChild(2);
                assert(params);//E_EXPR_LIST_WITH_BRACKETS
                Node * nd_list = params->getChild(0);
                assert(nd_list);

                std::list<Node*> ls;
                Node::ToList(nd_list, ls);
                for (auto it : ls) {
                    RawExpr* e = nullptr;
                    resolve_expr(plan, it, sql_raw_expr, parent, e);
                    expr->add_param_expr(e);
                }
                out_raw_expr = expr;

            }break;
            case E_STRUCT_DEF:{
                StructDefExpr* expr = new StructDefExpr;
                expr->set_expr_type(E_STRUCT_DEF);
                expr->_hav_datatype_def = node->getChild(1)?true:false;
                expr->_hav_struct_name = node->getChild(0)?true:false;
                Node * params = node->getChild(2);                
                assert(params);
                Node * nd_list = params->getChild(0);
                assert(nd_list);

                std::list<Node*> ls;
                Node::ToList(nd_list, ls);
                for (auto it : ls) {
                    RawExpr* e = nullptr;
                    resolve_expr(plan, it, sql_raw_expr, parent, e);
                    expr->add_param_expr(e);
                }
                out_raw_expr = expr;
            } break;
            default:
                break;

        }
        return 0;
    }


    int resolve_jointb_using_cols( Node* node, Stmt* parent, JoinedTable* jt) {

        std::vector<std::string> cols;
        if(node->GetChildrenCount() == 4 && NULL != node->GetChild(3)){
            Node * node_using = node->getChild(3);
            if(node_using->GetType() == E_SIMPLE_IDENT_LIST_WITH_PARENS){
                node_using = Node::remove_parens(node_using);
                std::list<Node*> ls;
                Node::ToList(node_using, ls);
                for (Node* it : ls) {
                    cols.push_back(it->terminalToken_.str);
                }
            }
        }

        const std::vector<JoinedTable::JoinNode> & tbs = jt->get_all_table_id();
        for(size_t i = 0 ; i < tbs.size(); ++i){
            uint64_t id = tbs[i].id_;

            TableRef* table_ref = NULL;
            parent->get_table_item(id,table_ref);
            if(table_ref) {
                if(cols.size() > 0) {
                    if(TableRef::BASE_TABLE_DIRECT_REF == table_ref->get_table_ref_type() ||
                       TableRef::BASE_TABLE_ALIAS_REF == table_ref->get_table_ref_type() ){
                        table_ref->AddUsingColumns(cols);
                    }
                }
                table_ref->SetJoinType(tbs[i].join_type_); // set join type for table item

            }
        }

        return 0;
    }


    int resolve_sp_executesql_statement(ResultPlan* plan, Node* node, uint64_t& query_id) {
      // This is a workaround to fix bug 70738
      // Character constants are not allowed in the statement of sp_executesql. We can not add mask conditions in it.
      // As a workaround, we get the inner statement, make it as the whole statement, and enforce it.
      // Detail: https://docs.microsoft.com/en-us/sql/relational-databases/system-stored-procedures/sp-executesql-transact-sql?view=sql-server-ver15

      assert(node->nodeType_ == E_SP_EXECUTESQL);

      const std::regex regex {"(N?')([^']*)(')"};
      std::string sql = node->getChild(E_SP_EXECUTESQL_STATEMENT)->serialize();
      std::smatch match_result{};

      if (std::regex_match(sql, match_result, regex)) {
        ParseResult* tmp_parse_result = new ParseResult;
        sql = match_result[2].str();
        parser::parse_sql2003(sql, tmp_parse_result);

        if (tmp_parse_result->accept) {
          // Clean up unused space
          node->swap(tmp_parse_result->result_tree_);
          resolve(plan, node, query_id);
        }

        delete tmp_parse_result;
      }

      return 0;
    }

}


