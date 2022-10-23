#ifndef PARSE_EXPRESSION_H
#define PARSE_EXPRESSION_H

#include "Lex.h"
#include <vector>

class PolicyModelList;


struct ParseException {
    ParseException() : _code(ParseException::RET::SUCCESS),_syms(NULL) {}
    void SetFail(Token::TOKEN_TYPE /*expect*/, Lex* /*lex*/) { 
        _code = RET::FAIL;
    }
    void SetFail(const std::vector<Token::TOKEN_TYPE >& /*expects*/, Lex* /*lex*/) 
    {
         _code = RET::FAIL;
    }
    enum class RET{ SUCCESS, FAIL }  _code = RET::SUCCESS;
    std::string             _detail;
    PolicyModelList        *_syms;
};

class AstExpr;

AstExpr *parse_boolean_expr(Lex *lex, ParseException *e);

AstExpr *parse_value(Lex *lex, ParseException *e);

class AstColumnRef;

std::vector<AstColumnRef*> parse_oblication(LexOb *lex);


#endif