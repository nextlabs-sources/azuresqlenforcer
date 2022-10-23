//
// Created by jeff on 2019/10/9.
//

#include "policy_expression.h"
#include <string.h>
#include <assert.h>
#include "tool.h"
//-------------AstExpr----------------
AstExpr::AstExpr(EXPR_TYPE expr_type):_expr_type(expr_type), _expr_parent(NULL){ }
AstExpr::EXPR_TYPE AstExpr::GetExprType()const{ return  _expr_type; }
AstExpr* AstExpr::GetParent() { return _expr_parent; }
void AstExpr::SetParent(AstExpr *parent) { assert( _expr_parent == NULL && parent != NULL ); _expr_parent = parent; }

//-------------AstBinaryOpExpr-------------
AstBinaryOpExpr::AstBinaryOpExpr(EXPR_TYPE expr_type, AstExpr *left, AstExpr *right):AstExpr(expr_type), _left(left), _right(right){
    _left->SetParent(this);
    _right->SetParent(this);
}
AstBinaryOpExpr::~AstBinaryOpExpr(){
    if (_left)  delete _left;
    _left = nullptr;
    if (_right) delete _right;
    _right = nullptr;
}
AstExpr* AstBinaryOpExpr::GetLeft() { return _left; }
AstExpr* AstBinaryOpExpr::GetRight() { return _right; }


//-------------AstConstantValue---------------
AstConstantValue::AstConstantValue(EXPR_TYPE expr_type) :AstExpr(expr_type) { u._int_data = 0; }
AstConstantValue::~AstConstantValue(){
    if (GetExprType() == EXPR_TYPE::C_STRING  || GetExprType() == EXPR_TYPE::C_PATTERN) {
        free (u._other_data);
        u._other_data = nullptr;
    } else if (GetExprType() == EXPR_TYPE::C_ARRAY) {
        for (auto it : _array) delete (it);
        _array.clear();
    }
}
void AstConstantValue::SetValue(int data){    u._int_data = data; }
void AstConstantValue::SetValue(const std::string& value){ u._other_data = strdup(value.c_str()); }
void AstConstantValue::SetValue(const std::vector<AstExpr*>& array) { _array = array; }
int  AstConstantValue::GetValueAsInt(bool& r) {
    if (GetExprType() == EXPR_TYPE::C_NUMBER) {
        r = true;
        return u._int_data;
    } else {
        char *end = nullptr;
        int i = (int)strtol(u._other_data, &end, 10);
        r = *end == '\0';
        return i;
    }
}

///-------------AstColumnRef-----------------------------------
AstColumnRef::AstColumnRef(COL_TYPE col_type, VAL_TYPE val_type, const AstIds& ids):AstExpr(EXPR_TYPE::EXPR_COLUMN_REF), _col_type(col_type), _val_type(val_type), _ids(ids){}
AstColumnRef::~AstColumnRef(){
    for (auto ids:_ids) {
        if (ids) delete ids;
    }
}
AstColumnRef::COL_TYPE AstColumnRef::GetColType() { return _col_type; }
AstColumnRef::VAL_TYPE AstColumnRef::GetValType() { return _val_type; }
void    AstColumnRef::SetColumn(const AstIds& ids){    _ids = ids; }
const AstIds&   AstColumnRef::GetColumn(){    return  _ids; }

///-------------AstUnaryOpExpr----------------------------------------
AstUnaryOpExpr::AstUnaryOpExpr(EXPR_TYPE expr_type, AstExpr *expr): AstExpr(expr_type), _expr(expr){
    _expr->SetParent(this);
}
AstUnaryOpExpr::~AstUnaryOpExpr(){ if (_expr) delete _expr;}
AstExpr    *AstUnaryOpExpr::GetExpr(){ return _expr; }

//

void print_ast(std::set<std::string>& set_val,  AstExpr* ast){
    switch(ast->GetExprType()){
        case AstExpr::EXPR_TYPE::C_TRUE:
        case AstExpr::EXPR_TYPE::C_FALSE:
        case AstExpr::EXPR_TYPE::C_UNKNOWN:
        case AstExpr::EXPR_TYPE::C_NULL:
        case AstExpr::EXPR_TYPE::C_PATTERN:
        {
           break;
        }break;
        case AstExpr::EXPR_TYPE::C_NUMBER:{
            /*
            bool b;
            int i = dynamic_cast<AstConstantValue*>(ast)->GetValueAsInt(b);
            */
			break;
        }
        case AstExpr::EXPR_TYPE::C_STRING:
        {
            std::string ss = dynamic_cast<AstConstantValue*>(ast)->GetValueAsStr();
            if(!ss.empty()){
                set_val.insert(ss);
            }
        }break;
        case AstExpr::EXPR_TYPE::C_ARRAY:{
            std::vector<AstExpr*> & asts = dynamic_cast<AstConstantValue*>(ast)->GetArray();
            for(auto it: asts){
                print_ast(set_val, it);
            }
        }break;
        default:break;
    }

}

void get_attribute_value_vec(std::set<std::string>& set_val,  AstExpr* ast,const AstColumnRef::COL_TYPE type, const std::string &colname ){
    switch(ast->GetExprType()){
        /* BINARY OPERATOR */
    case AstExpr::EXPR_TYPE::OR:
        case AstExpr::EXPR_TYPE::AND:
        case AstExpr::EXPR_TYPE::COMP_LE:
        case AstExpr::EXPR_TYPE::COMP_LT:
        case AstExpr::EXPR_TYPE::COMP_GE:
        case AstExpr::EXPR_TYPE::COMP_GT:
        case AstExpr::EXPR_TYPE::COMP_EQ:
        case AstExpr::EXPR_TYPE::COMP_NEQ:
        case AstExpr::EXPR_TYPE::LIKE:
        case AstExpr::EXPR_TYPE::NOT_LIKE:
        case AstExpr::EXPR_TYPE::INCLUDES:
        case AstExpr::EXPR_TYPE::EQUALS_UNORDERED:{
            //get_attribute_value_vec( vec_val,  ast, type, colname );
            AstExpr* astl = dynamic_cast<AstBinaryOpExpr*>(ast)->GetLeft();
            AstExpr* astr = dynamic_cast<AstBinaryOpExpr*>(ast)->GetRight();
            if(astl->GetExprType() == AstExpr::EXPR_TYPE::EXPR_COLUMN_REF ) {
                AstColumnRef * astcr = dynamic_cast<AstColumnRef*>(astl);
                if(astcr->GetColType() == type){
                    const AstIds& ids =  astcr->GetColumn();
                    size_t s = ids.size();

                   const std::string& ss = ids[s-1]->GetId();
                   if(ss.compare(colname) == 0){
                       print_ast( set_val, astr);
                   }

                }
            } else if(astr->GetExprType() == AstExpr::EXPR_TYPE::EXPR_COLUMN_REF ) {
                AstColumnRef * astcr = dynamic_cast<AstColumnRef*>(astr);
                if(astcr->GetColType() == type){
                    const AstIds& ids =  astcr->GetColumn();
                    size_t s = ids.size();

                    const std::string& ss = ids[s-1]->GetId();
                    if(ss.compare(colname) == 0){
                        print_ast( set_val, astl);
                    }

                }
            } {
                get_attribute_value_vec( set_val,  astl, type, colname );
                get_attribute_value_vec( set_val,  astr, type, colname );
            }
        }break;
            /* UNARY */
        case AstExpr::EXPR_TYPE::NOT:{
            AstExpr* astl = dynamic_cast<AstUnaryOpExpr*>(ast)->GetExpr();
            get_attribute_value_vec( set_val,  astl, type, colname );
        }break;
        /* CONSTANT */
        case AstExpr::EXPR_TYPE::C_TRUE:
        case AstExpr::EXPR_TYPE::C_FALSE:
        case AstExpr::EXPR_TYPE::C_UNKNOWN:
        case AstExpr::EXPR_TYPE::C_NULL:
        case AstExpr::EXPR_TYPE::C_NUMBER:
        case AstExpr::EXPR_TYPE::C_STRING:
        case AstExpr::EXPR_TYPE::C_PATTERN:
        case AstExpr::EXPR_TYPE::C_ARRAY:{
            //get_attribute_value_vec( vec_val,  ast, type, colname );
        }break;
        case AstExpr::EXPR_TYPE::EXPR_COLUMN_REF:{
            //get_attribute_value_vec( vec_val,  ast, type, colname );
        }break;
        case AstExpr::EXPR_TYPE::EXPER_NOT_SUPPORT:{
            //get_attribute_value_vec( vec_val,  ast, type, colname );
        }break;
        default:
            break;

    }
}