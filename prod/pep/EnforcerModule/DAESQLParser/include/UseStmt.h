#ifndef USE_STMT_H
#define USE_STMT_H

#include "Interface.h"
#include "Stmt.h"

namespace resolve {
    class UseStmt : public Stmt, IUseStmt {
    public:
        void set_database_name(const std::string& db) { database_name_ = db; }
        std::string get_database_name() const { return database_name_; }
        void set_schema_name(const std::string& sc) {_schema = sc;}
        void set_db_type(EMDB_DB_TYPE db_type){ _db_type = db_type; }


        virtual const char * GetSchema() { return  _schema.c_str(); };
        virtual const char * GetDbname() { return  database_name_.c_str(); };
    public:
        virtual StmtType GetStmtType() override { return  E_STMT_TYPE_USE; }

    private:
        std::string database_name_;
        std::string _schema;
        EMDB_DB_TYPE _db_type;
    };
}

#endif