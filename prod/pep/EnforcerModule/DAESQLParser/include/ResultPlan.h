#ifndef RESULT_PLAN_H
#define RESULT_PLAN_H

#include "Interface.h"
#include "EMDBType.h"

class Node;

namespace resolve {
    class LogicPlan;
    class LocalTableMgr;
    class ResultPlan : public IPlan {
    public:
        ResultPlan(
                const StmtVisit& visit,
                const BaseTableColumnVisit& visit1,
                const StartNewStmt& visit2,
                const WhereClauseVisit& visit3,
                const ErrorOccur& errorOccur);
        ~ResultPlan();
        void reset();
        StmtVisit base_stmt_visit_;
        BaseTableColumnVisit baseTableColumnVisit_;
        StartNewStmt startNewStmt_;
        WhereClauseVisit whereClauseVisit_;
        ErrorOccur errorOccur_;
        LogicPlan* logicPlan_{nullptr};
        LocalTableMgr* local_table_mgr{nullptr};
        void* context_{nullptr};
        Node* tree_root_{nullptr};
        EMDB_DB_TYPE type_;

    public:
        void set_err(int err_code, const std::string err_detail);
        std::string error_detail_;
        int error_code_;

        // implement IPlan
        virtual void* GetContext() override { return context_; }
        virtual IStmt* GetQuery(uint64_t query_id) override;
        virtual void SetDefaultSchema(const std::string& default_schema) override;
        virtual void SetDefaultDatabase(const std::string& default_database) override;
        virtual std::string GetDefaultDatabase() override;
        virtual void AddTableStructure(const std::string& schema,
                const std::string& table, const std::list<std::string>& columns) override;
        virtual std::string GetErrorDetail() override { return error_detail_; }
        virtual EMDB_DB_TYPE GetDatabaseType() override { return type_; }
    };
}


#endif