#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>
#include <map>
#include "policy_expression.h"

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > BINARY_LOGIC_OP{
        {Token::TK_OR, AstExpr::EXPR_TYPE::OR}, {Token::TK_AND, AstExpr::EXPR_TYPE::AND}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > UNARY_LOGIC_OP{
        {Token::TK_NOT, AstExpr::EXPR_TYPE::NOT}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > COMPARE_OP{
        {Token::TK_EQ, AstExpr::EXPR_TYPE::COMP_EQ}, {Token::TK_NEQ, AstExpr::EXPR_TYPE::COMP_NEQ},
        {Token::TK_LT, AstExpr::EXPR_TYPE::COMP_LT}, {Token::TK_LTEQ, AstExpr::EXPR_TYPE::COMP_LE},
        {Token::TK_GT, AstExpr::EXPR_TYPE::COMP_GT}, {Token::TK_GTEQ, AstExpr::EXPR_TYPE::COMP_GE}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > SPECIAL_OP{
        {Token::TK_INCLUDES, AstExpr::EXPR_TYPE::INCLUDES}, {Token::TK_EQUALS_UNORDERED, AstExpr::EXPR_TYPE::EQUALS_UNORDERED}
};

const std::map<Token::TOKEN_TYPE , AstColumnRef::COL_TYPE > TYPES{
        {Token::TK_HOST, AstColumnRef::COL_TYPE::HOST}, {Token::TK_USER, AstColumnRef::COL_TYPE::SUB},
        {Token::TK_APP, AstColumnRef::COL_TYPE::APP}, {Token::TK_RES, AstColumnRef::COL_TYPE::RES}
};

bool is_type(Token::TOKEN_TYPE tp, AstColumnRef::COL_TYPE& out) {
    auto fd = TYPES.find(tp);
    if (fd != TYPES.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_binary_logic_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = BINARY_LOGIC_OP.find(op);
    if (fd != BINARY_LOGIC_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_unary_logic_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = UNARY_LOGIC_OP.find(op);
    if (fd != UNARY_LOGIC_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_comp_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = COMPARE_OP.find(op);
    if (fd != COMPARE_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_special_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = SPECIAL_OP.find(op);
    if (fd != SPECIAL_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_predicate_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    return is_comp_op(op, out) || is_special_op(op, out);
}

#endif