#include "node.h"
#include "parser.h"
#include "resolve.h"
#include "ResultPlan.h"
#include "EMDBType.h"
#include "where_clause.h"
#include "EMDBConfig.h"
#include "commfun.h"

const auto is_config_loaded = EMDBConfig::GetInstance().Load(CommonFun::GetConfigFilePath());

bool CompressSql(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms) {
    switch (dbms) {
        //case EMDB_DB_TYPE::EMDB_DB_BIGQUERY: {
        //    return parser::compress_sqlbigquery(sql, out_sql);
        //} //break;
        //case EMDB_DB_DB2:{
        //    return parser::compress_sqldb2(sql, out_sql);
        //} //break;
        case EMDB_DB_TYPE::EMDB_DB_SQLSERVER : {
            return parser::compress_sql2003(sql, out_sql);
        } //break;
        //case EMDB_DB_TYPE::EMDB_DB_ORACLE: {
        //    return parser::compress_sqloracle(sql, out_sql);
        //} //break;
        //case EMDB_DB_TYPE::EMDB_DB_HANA: {
        //    return parser::compress_sqlhana(sql, out_sql);
        //} //break;
        default: {
            return false;
        }
    }
}

bool BeautifySql(const std::string& sql, std::string& out_sql, EMDB_DB_TYPE dbms) {
    switch (dbms) {
        //case EMDB_DB_TYPE::EMDB_DB_BIGQUERY: {
        //    return parser::beautify_sqlbigquery(sql, out_sql);
        //} //break;
        //case EMDB_DB_DB2:{
        //    return parser::beautify_sqldb2(sql, out_sql);
        //} //break;
        case EMDB_DB_TYPE::EMDB_DB_SQLSERVER : {
            return parser::beautify_sql2003(sql, out_sql);
        } //break;
        //case EMDB_DB_TYPE::EMDB_DB_ORACLE: {
        //    return parser::beautify_sqloracle(sql, out_sql);
        //} //break;
        //case EMDB_DB_TYPE::EMDB_DB_HANA: {
        //    return parser::beautify_sqlhana(sql, out_sql);
        //} //break;
        default: {
            return false;
        }
    }
}

IPlan* CreatePlan(
        const StmtVisit& StmtVisit,
        const BaseTableColumnVisit& baseTableColumnVisit,
        const StartNewStmt& startNewStmt,
        const WhereClauseVisit& whereClauseVisit,
        const ErrorOccur& errorOccur,
        void* context, IParseResult* parseResult) {
    auto parser_result = dynamic_cast<ParseResult*>(parseResult);
    if (!parser_result) return nullptr;
    auto root = dynamic_cast<Node*>(parser_result->result_tree_);
    if (!root) return nullptr;
    resolve::ResultPlan* ret = new resolve::ResultPlan(
            StmtVisit, baseTableColumnVisit,
            startNewStmt, whereClauseVisit, errorOccur);
    ret->context_ = context;
    ret->tree_root_ = root;
    ret->type_ = parser_result->type_;
    return ret;
}

void DestroyPlan(IPlan* plan) {
    delete(dynamic_cast<resolve::ResultPlan*>(plan));
}

void VisitPlan(IPlan* p) {
    resolve::ResultPlan* plan = dynamic_cast<resolve::ResultPlan*>(p);
    if (!plan)
        return;
    if (!plan->tree_root_)
        return;

    resolve::resolve_multi_statements(plan, plan->tree_root_);
}



IParseResult* ParseSql(const std::string &sql, EMDB_DB_TYPE dbms) {
    ParseResult* result = new ParseResult;
    result->type_ = dbms;
    switch (dbms) {
        //case EMDB_DB_TYPE::EMDB_DB_BIGQUERY: {
        //    parser::parse_sqlbigquery(sql, result);
        //} break;
        case EMDB_DB_TYPE::EMDB_DB_SQLSERVER : {
            parser::parse_sql2003(sql, result);
        } break;
        //case EMDB_DB_TYPE::EMDB_DB_ORACLE: {
        //    parser::parse_sqloracle(sql, result);
        //} break;
        //case EMDB_DB_TYPE::EMDB_DB_HANA: {
        //    parser::parse_sqlhana(sql, result);
        //}break;
        //case EMDB_DB_TYPE::EMDB_DB_DB2: {
        //    parser::parse_sqldb2(sql, result);
        //}break;
        default: {
            result->errDetail = "UNKNOWN EMDB_DB_TYPE";
        }
    }
    return result;
}

void DestroyParseResult(IParseResult* result) {
    delete(dynamic_cast<ParseResult*>(result));
}

NodeType Node::GetType() {
    return nodeType_;
}

INode* Node::GetParent() {
    return parent_;
}

bool Node::SetParent(INode *node) {
    Node* parent = dynamic_cast<Node*>(node);
    if (!parent)
        return false;
    setParent(parent);
    return true;
}

INode* Node::GetChild(int key) {
    return getChild(key);
}

bool Node::SetChild(int key, INode *node) {
    Node* child = dynamic_cast<Node*>(node);
    if (!child)
        return false;
    return setChild(key, child);
}

size_t Node::GetChildrenCount() const {
    return getChildrenCount();
}

std::string Node::Text() {
    return text();
}

bool Node::IsTerminal() const {
    return isTerminalToken;
}

std::string Node::Serialize() {
    return serialize();
}

void Node::Print() {
    print(this, 0);
}

bool ParseResult::IsAccept() const {
    return accept;
}

INode* ParseResult::GetParseTree() {
    return result_tree_;
}

size_t ParseResult::GetErrorLine() const {
    return errFirstLine;
}

size_t ParseResult::GetErrorColumn() const {
    return errFirstColumn;
}

std::string ParseResult::GetErrorDetail() const {
    return errDetail;
}

IWhereCluase * ParseResult::CreateSimpleSelectWhereClause() const {
    IWhereCluase * ret = NULL;
    Node * root = result_tree_;
    if(root->GetType() == E_DIRECT_SELECT){
        root = root->getChild(1);
    }
    if(root->GetType() == E_SELECT && root->getChild(6) == NULL) {
        resolve::WhereCluase * pw = new resolve::WhereCluase;
        pw->bind(root->getChild(3), root);
        ret = pw;
    }
    return ret;
}

void ParseResult::ReleaseWhereClause(IWhereCluase * pw) const {
    if(pw){
        delete(pw);
    }
}