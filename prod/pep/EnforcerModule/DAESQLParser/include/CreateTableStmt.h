#ifndef CREATE_TABLE_STMT_H
#define CREATE_TABLE_STMT_H

#include "Interface.h"
#include "Stmt.h"

namespace resolve {
    class CreateTableStmt : public Stmt, public ICreateTableStmt
    {
    public:
        ~CreateTableStmt();
    public:
        virtual StmtType GetStmtType() override { return E_STMT_TYPE_CREATE_TABLE; }
    };
}

#endif