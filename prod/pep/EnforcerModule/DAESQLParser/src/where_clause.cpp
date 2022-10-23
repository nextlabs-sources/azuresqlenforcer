#include "where_clause.h"
#include "node.h"
#include <assert.h>
#include "serialize_format.h"
#include "EMMaskOpr.h"

namespace resolve {
    std::string WhereCluase::GetCondition() {
        if (!where_)
            return "";
        else
            return where_->serialize();
    }
    void add_qualify_to_field(Node * node, const std::string & field_qualify, const std::vector<std::string> & using_cols, std::set<std::string> & fields){
        if(NULL == node) return;
        if(node->isTerminalToken) return;

        size_t icount = node->getChildrenCount();
        for(int i = 0 ; i < icount; ++i ){
            Node * child = node->getChild(i);
            if(child == NULL) {
                continue;
            }
            if(child->isTerminalToken) {
                continue;
            }
            NodeType tp = child->GetType();
            if(E_OP_NAME_FIELD == tp) {
                Node * n1 = child->getChild(0);
                assert(n1);
                fields.insert(n1->terminalToken_.str); // output fields;

                size_t count = child->GetChildrenCount();
                for (size_t i = 1; i < count; i++) {
                    if (NULL != child->getChild(i)) {
                        delete(child->getChild(i));
                        child->setChild(i, NULL);
                    }

                }

                bool is_using_col = false;
                for(size_t i = 0 ; i < using_cols.size(); ++i){
                    if( IgnoreCaseCmp::compare(using_cols[i], n1->terminalToken_.str) == 0) {
                        is_using_col = true;
                        break;
                    }
                }
                if(is_using_col){
                    continue;
                }
                if(field_qualify.length() > 0) {

                    Node * quanlify = Node::makeTerminalNode(E_IDENTIFIER, field_qualify); // no need dot
                    child->setChild(1,quanlify);
                }

                continue;
            } else if(E_SELECT == tp || E_DIRECT_SELECT == tp){
                continue;
            } else {
                add_qualify_to_field(child, field_qualify, using_cols, fields);
            }
        }

    }

    std::string WhereCluase::GetModifyedCondition(const std::string & field_qualify, const std::vector<std::string> & using_cols, std::set<std::string> & fields){
        if (!where_)
            return "";
        //add qualify and out put fields
        Node * nodew = where_->getChild(E_WHERE_CLAUSE_EXPR);
        add_qualify_to_field(nodew, field_qualify, using_cols, fields);
        return nodew->Serialize();
    }

    bool WhereCluase::AddCondition(const std::string& condition,SqlException & e) {
        if (!where_) {
            Node* new_expr = Node::makeTerminalNode(E_STRING, condition);
            Node* where = Node::makeNonTerminalNode(E_WHERE_CLAUSE, E_WHERE_CLAUSE_PROPERTY_CNT, new_expr);
            where->serialize_format = &WHERE_SERIALIZE_FORMAT;
            switch (select_->nodeType_) {
                case E_SELECT: {
                    select_->setChild(E_SELECT_OPT_WHERE, where);
                } break;
                case E_UPDATE: {
                    select_->setChild(E_UPDATE_OPT_WHERE, where);
                } break;
                case E_DELETE: {
                    select_->setChild(E_DELETE_OPT_WHERE, where);
                } break;
                default:
                    e.SetInfo(ERR_FILTER_OPR,"nonsupport sql statment type.");
                    assert(false);  /* unreachable */
                    break;
            }
        }
        else {
            Node* ori_expr = where_->getChild(E_WHERE_CLAUSE_EXPR);
            Node* ori_expr_paren = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, ori_expr);
            ori_expr_paren->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
            Node* cond = Node::makeTerminalNode(E_STRING, condition);
            Node* new_expr = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, ori_expr_paren, cond);
            new_expr->serialize_format = &OP_AND_SERIALIZE_FORMAT;
            where_->setChild(E_WHERE_CLAUSE_EXPR, new_expr);
        }
        return true;
    }

    void WhereCluase::bind(Node* where, Node* select) {
        where_ = where;
        select_ = select;
        if (!where_) {
            assert(select);
            assert(select->nodeType_ == E_SELECT ||
            select->nodeType_ == E_UPDATE ||
            select->nodeType_ == E_DELETE);
        }
    }

    bool EMColumnsRef::MaskOpr(MaskConditionMap *  map_mask, SqlException & e){
        bool bret = mask_fields_v3(dynamic_cast<ISelectStmt*>(_stmt), _column , map_mask,_dbtype,MASK_2ASLABEL, e);
        return bret;
    }

    void UpdateItem::DenyUpdate(const std::string & condition, const std::string & field_qualify, EMDataType e, EMDB_DB_TYPE db){
        //E_OP_EQ   ( 0  =  1 )
        std::string cond = condition;
        if(cond.empty()){
            cond = " 1 = 1 ";
        }
        Node *field = _node->getChild(0);
        Node *val = _node->getChild(1);

        std::string qualify_;
        if(_col._tb.empty()){ // field no qualify
            qualify_ = field_qualify;
            qualify_ += '.';
        }
        std::string val_else ;
        if(e == EMDATA_STRING2 && db == EMDB_DB_ORACLE){
            val_else = "TO_NCHAR(" + val->serialize() + ")";
        } 
        else if (e == EMDATA_NUMBER && db == EMDB_DB_ORACLE){
            val_else = "TO_NUMBER(" + val->serialize() + ")";
        }else {
            val_else = val->serialize();
        }


        std::string val_s =
                "CASE WHEN " + cond + " THEN " + qualify_ + field->serialize() + " ELSE " + val_else + " END ";

        Node *val_new = Node::makeTerminalNode(E_STRING, val_s);
        _node->setChild(1, val_new);
        delete (val);
    }

    std::string UpdateItem::GetFieldName() {
        return _field;
    }
    UpdateItem::~UpdateItem(){

    }
    
}