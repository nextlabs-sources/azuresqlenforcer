#ifndef DROP_TABLE_STMT_H
#define DROP_TABLE_STMT_H

#include "Interface.h"
#include "Stmt.h"

namespace resolve {
    class DropTableStmt : public Stmt, public IDropTableStmt
    {
    public:
        ~DropTableStmt();
    public:
        virtual StmtType GetStmtType() override { return E_STMT_TYPE_DROP_TABLE; }
    };
}

#endif
