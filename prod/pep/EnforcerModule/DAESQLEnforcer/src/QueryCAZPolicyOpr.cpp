//
// Created by jeff on 2020/4/24.
//

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(push,0)
#pragma warning(disable: 5045)

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>


#endif

#include "enforcermgr.h"
#include "QueryCAZPolicyOpr.h"
#include "QueryCloudAZSDKCppWrapper.h"
#include "sql_collect.h"
#include "EMDBType.h"
#include "assert.h"
#include "DAELog.h"
#include "sqlparserwrapper.h"
#include "EMDBConfig.h"

bool is_digit(char c) { return '0' <= c && c <= '9'; }
bool is_underline(char c) { return '_' == c; }
bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }

//RequestInfo::RequestInfo(const RequestInfo& info){
//    this->_action = info._action;
//    for( auto it = this->_users.begin();it != this->_users.end(); ++it) {
//        delete(it->second);
//    }
//    this->_users.clear();
//    for( auto it = info._users.begin();it != info._users.end(); ++it) {
//        ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
//        this->_users[it->first]=uValue;
//    }
//    for( auto it = this->_rescs.begin();it != this->_rescs.end(); ++it) {
//        delete(it->second);
//    }
//    this->_rescs.clear();
//    for( auto it = info._rescs.begin();it != info._rescs.end(); ++it) {
//        ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
//        this->_rescs[it->first]=uValue;
//    }
//}

//RequestInfo & RequestInfo::operator=(const RequestInfo& info){
//    this->_action = info._action;
//    for( auto it = this->_users.begin();it != this->_users.end(); ++it) {
//        delete(it->second);
//    }
//    this->_users.clear();
//    for( auto it = info._users.begin();it != info._users.end(); ++it) {
//        ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
//        this->_users[it->first]=uValue;
//    }
//    for( auto it = this->_rescs.begin();it != this->_rescs.end(); ++it) {
//        delete(it->second);
//    }
//    this->_rescs.clear();
//    for( auto it = info._rescs.begin();it != info._rescs.end(); ++it) {
//        ResourceAttrValue * uValue = new ResourceAttrValue(it->second->strValue, it->second->attrType );
//        this->_rescs[it->first]=uValue;
//    }
//    return *this;
//}



RequestInfo::~RequestInfo(){
}

std::string RequestInfo::GenerateCacheKey() {
    //
    std::string ret = "";
    if (_users2.size() > 0) {
        ret += "U=";
        for (auto& it : _users2) {
            ret += it.first;
            ret += ":";
            if(it.second != NULL)
                ret += it.second->strValue;
            ret += ",";
        }
    }
    if (!_action.empty()) {
        ret += "A=";
        ret += _action;
        ret += ",";
    }
    if (_rescs2.size() > 0) {
        ret += "R=";
        for (auto& it : _rescs2) {
            if (it.second != NULL && !it.second->strValue.empty()) {
                ret += it.first;
                ret += ":";
                ret += it.second->strValue;
                ret += ",";
            }
                
        }
    }
    return ret;
}


/*
bool is_percent(char c) { return '%' == c; }
bool is_end(const std::string& s, unsigned int iIndex) { return s.length() <= iIndex; }
bool is_double_quote(char c) { return '"' == c; }

static std::set<std::string,IgnoreCaseCmp> g_reserved = {
        "ALL", "AND", "ANY", "ARRAY", "AS", "ASC", "AVG",
        "BETWEEN", "BIGINT", "BINARY", "BLOB", "BOOLEAN", "BY",
        "CASE", "CAST", "CALL", "CHAR", "CHARACTER", "CHARACTERS", "CLOB",
        //"COALESCE", "CODE_UNITS",
        "COLLATE", "CONVERT", "CORRESPONDING", "COUNT", "CROSS",
        //"CUME_DIST", "CURRENT", "CURRENT_TIMESTAMP", "CURRENT_USER",
        "DATE", "DAY", "DEC", "DECIMAL", "DEFAULT", "DELETE",
        //"DENSE_RANK",
        "DESC", "DISTINCT", "DOUBLE", "ELSE",
        "END", "END_P", "ESCAPE", "EXCEPT", "EXCLUDE", "EXISTS",
        "FALSE","FLOAT", "FOLLOWING", "FOR", "FROM", "FULL",
        "G", "GROUP", "GROUPING",
        "HAVING", "HOUR", "IN", "INNER", "INSERT", "INT", "INTEGER", "INTERSECT", "INTERVAL", "INTO", "IS",
        "JOIN", "K", "LARGE", "LEFT", "LIKE", "M", "MAX", "MIN", "MINUTE", "MOD", "MONTH", "MULTISET",
        "NATIONAL", "NATURAL", "NCHAR", "NCLOB", "NO", "NOT", "NULLIF", "NUMERIC", "NULL",
        "OBJECT", "OCTETS", "OF", "ON", "ONLY", "OR", "ORDER", "OTHERS", "OUTER", "OVER",
        "PARTITION",
        //"PERCENT_RANK", "PRECEDING", "PRECISION",
        "RANGE", "RANK", "READ", "REAL",
        "RECURSIVE", "REF", "RIGHT", "ROW", "ROWS",
        //"ROW_NUMBER",
        "SCOPE", "SECOND", "SELECT",
        "SESSION_USER", "SET", "SETS", "SMALLINT", "SOME", "STDDEV_POP",
        "SUM",
        //"SYSTEM_USER",
        "TRUE", "THEN", "TIES", "TIME", "TIMESTAMP", "TO", "UNBOUNDED", "UNION", "UPDATE", "USING", "VALUES", "VARCHAR", "VARYING",
        "VAR_POP", "VAR_SAMP", "WHEN", "WHERE", "WITH", "WITHOUT", "YEAR", "ZONE", "LIMIT", "OFFSET"
};
*/
bool is_num(const std::string & ss){
    unsigned int pos = 0;
    if(ss.length() <= 0) return false;
    while(pos < ss.length()) {
        if(!is_digit(ss[pos])
           && '.' != ss[pos]
           && ss[0] != '-' )
            return false;
        pos++;
    }
    return true;
}

std::string parse_identifier(const std::string & str, unsigned int & pos) {
    std::string ret;
    if(str[pos] >= '0' && str[pos] <= '9') {
        //error;
    } else if(str[pos] == '"') {//"aaa"
        pos++;
        if(is_digit(str[pos]) ) {
            //error;
        } else {
            while(str[pos] != '\0') {
                if(str[pos] == '"' ) {
                    pos++;
                    if(str[pos] != '"'){
                        break;
                    }
                }
                ret += str[pos];
                pos++;
            }
        }
    } else if(is_underline(str[pos]) || is_letter(str[pos])  ){
        ret += str[pos];
        pos++;
        while(str[pos] != '\0') {
            if(is_underline(str[pos]) || is_letter(str[pos]) || is_digit(str[pos]) ) {
                ret += str[pos];
            } else if(str[pos] == '.') {
                break;
            } else {
                break;
            }
            pos++;
        }
        transform(ret.begin(),ret.end(),ret.begin(),toupper);
    } else if(str[pos]  == '*') {
        ret += '*';/*support format: * */
    } else {
    }

    while(str[pos] != ',' && str[pos] != '.' && str[pos] != '\0') {
        pos++;
    }

    return  ret;
}

void parse_fields(const std::string & ss, std::set<std::string> & set_fields) {
    unsigned int ipos = 0;
    std::set<std::string> pset ;//= new std::set<std::string>;
    while(ipos < ss.length()) {
        if(ss[ipos] == ' '|| ss[ipos] == ',' ){
        } else {
            std::string scol = parse_identifier(ss, ipos);
            if(!scol.empty()) {
                pset.insert(scol);
            }
        }
        ipos++;
    }
    if(!pset.empty()) set_fields.insert(pset.begin(),pset.end());
}


bool is_write(char c){
    return c==' '||c=='\n'||c=='\t'||c=='\r'||c=='\f';
}
bool is_op(char c){
    return c=='>'||c=='<'||c=='='||c=='!';
}
bool is_paren(char c){
    return c=='(' || c==')' || c == '[' || c==']';
}
/*
bool is_reserved(const std::string &src){
    if(g_reserved.find(src) != g_reserved.end())
        return true;
    return false;
}


std::string parser_identifier(const std::string &src, unsigned int & p, char c){
    bool bquote = (c=='`'||c=='"');
    if(bquote){
        p++;
    }
    size_t len = src.length();
    std::string ret;
    while(p < len){
        if(!bquote) { //
            if (is_write(src[p]) || is_op(src[p]) || is_paren(src[p])){
                break;
            }else if(is_underline(src[p]) || is_letter(src[p]) || src[p]=='-' || is_digit(src[p])){

            } else {
                return "";

            }
        } else {
            if(src[p] == c && src[p-1] != '\\'){
                p++;
                break;
            }
        }
        ret += src[p];
        p++;
    }
    return ret;
}
std::string parser_string(const std::string &src,unsigned int & p, char c){
    if(src[p] != c)
        return "";
    p++;
    size_t len = src.length();
    std::string ret;
    bool bend = false;
    while(p < len){
        if(src[p] == c && src[p-1] != '\\'){
            p++;
            bend = true;
            break;
        }
        ret += src[p];
        p++;
    }
    if(!bend){
        return "";
    }
    return ret;
}

std::string parser_num(const std::string &src,unsigned int & p, char c){
    size_t len = src.length();
    std::string ret;
    while(p < len){
        if(is_write(src[p]) || is_op(src[p]) || is_paren(src[p]))
            break;
        switch (src[p]){
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '-':
            case '.':
            case 'e':{
                ret += src[p];
            }break;
            default:{
                //error;
                return "";
            }break;
        }
        p++;
    }
    return ret;
}
*/
//std::string get_qualify(const ConditionInfo* pInfo){
//    std::string ret;
//    if(pInfo->dbType == EMDB_DB_BIGQUERY){
//        bool in = false;
//        int ipos = 0;
//        int ilen = pInfo->tableName.length();
//        std::string tb;
//        while(ipos < ilen){
//            switch(pInfo->tableName[ipos]){
//                case '.': {
//                    if(in) {
//                        tb += '.';
//                        break;
//                    }
//                    tb.clear();//only reserve tablename
//                } break;
//                case '`': {
//                    if(in) in = false;
//                    else in =true;
//                    tb += '`';
//                } break;
//                default: {
//                    tb += pInfo->tableName[ipos];
//                } break;
//
//            }
//            ipos++;
//        }
//        if(!tb.empty()){
//            ret = tb;
//        }
//    }
//    else{
//        ret = pInfo->tableName;
//    }
//    if(!ret.empty()){
//        ret+='.';
//    }
//
//    return  ret;
//}

std::string get_field_qualify( EMDB_DB_TYPE dbtype, const std::string & tablename){
    std::string ret;
    if(dbtype == EMDB_DB_BIGQUERY){
        bool in = false;
        size_t ipos = 0;
        size_t ilen = tablename.length();
        std::string tb;
        while(ipos < ilen){
            switch(tablename[ipos]){
                case '.': {
                    if(in) {
                        tb += '.';
                        break;
                    }
                    tb.clear();//only reserve tablename
                } break;
                case '`': {
                    if(in) in = false;
                    else in =true;
                    tb += '`';
                } break;
                default: {
                    tb += tablename[ipos];
                } break;

            }
            ipos++;
        }
        if(!tb.empty()){
            ret = tb;
        }
    }
    else{
        ret = tablename;
    }

    return  ret;
}
/*
std::string error_opr(const std::string & src, const unsigned int p){
    theLog->WriteLog(log_warning, "policy obligation parser error (at:%d) (condition:%s)",p,src.c_str());
    //printf("condition error:%d\n",p);
    return "";
}
*/
std::string repalce_user_attr(const std::string & src,  ConditionInfo & info) {
    std::string ret ;//= src;
    size_t ilen = src.length();
    size_t ipos = 0;
    while(ipos < ilen){

        if(src[ipos] == '$' && ipos+6 < ilen){
            //$user. attr
            std::string user = src.substr(ipos+1,5);
            if( user.compare("user.") == 0){
                std::string attr ;
                ipos += 6;
                while(ipos < ilen){
                    //
                    if( is_letter(src[ipos]) || is_digit(src[ipos]) || is_underline(src[ipos]) || src[ipos] == '-'){
                        attr += src[ipos];
                        ipos++;
                    } else {
                        break;
                    }
                }
                if(! attr.empty()) {
                    if(info.pUserAttr){
                        std::string val = info.pUserAttr->GetUserAttr(attr.c_str());
                        ret += val;
                    } else {
                        ret += "$user.";
                        ret += attr;
                        //
                        theLog->WriteLog(log_warning, "repalce_user_attr error userattr is nullptr");
                    }
                } else {
                    ret += "$user.";
                }
                continue;
            } else if( user.compare("table") == 0){
                ipos += 6;
                std::string qualifier = get_field_qualify(info.dbType, info.tableName);
                ret += qualifier;
                continue;
            } else {
                ret += src[ipos];
            }
        } else {
            ret += src[ipos];
        }
        ipos++;
    }
    return  ret;
}


std::string parser_condition_v2(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields){
    // replace user attr
    std::string ret = repalce_user_attr( src, condinfo);
    //component sql
    std::string sql_head = "select * from a where ";
    std::string tsql = sql_head + ret;
    //check and modify condition
    CSqlparserWrapper *pSqlparser = CSqlparserWrapper::Instance();
    //pSqlparser->LoadParser();
    IParseResult *parserResult = pSqlparser->ParseSql(tsql, condinfo.dbType);
    if (!parserResult->IsAccept())
    {
//        printf("sql script syntax error at(%d, %d): %s",
//               (int)parserResult->GetErrorLine(),
//               (int)parserResult->GetErrorColumn(),
//               parserResult->GetErrorDetail().c_str() );
        theLog->WriteLog(log_warning, "policy condition:%s error at(%d): %s", ret.c_str(), (int)parserResult->GetErrorColumn()-sql_head.length(), parserResult->GetErrorDetail().c_str());
        return "";
    }
    IWhereCluase * pw = parserResult->CreateSimpleSelectWhereClause();
    std::string qualifier = get_field_qualify(condinfo.dbType, condinfo.tableName);
    if(condinfo._join_tp == E_JOIN_NATURAL){
        qualifier.clear();
    }
    if(pw ){
        ret = pw->GetModifyedCondition( qualifier, condinfo._using_cols, fields);
    }
    parserResult->ReleaseWhereClause(pw);

    pSqlparser->DestroyParseResult(parserResult);

    return  ret;
}
/*
std::string parser_condition(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields) {
    std::string ret;
    bool b_paren_in = false;//Parentheses must exist in pairs
    std::string qualify = condinfo.tableName;
    unsigned int p = 0;
    unsigned int len = src.length();
    while(p < len) {
        switch(src[p]) {
            case '$':{//$user.attr
                std::string subuser = src.substr(p+1, 5);
                if(subuser == "user."){
                    p+=6;
                    std::string attr;
                    while(p<len){
                        if(is_write(src[p])) break;
                        attr += src[p];
                        p++;
                    }
                    if(attr.empty()){
                        //error
                        return error_opr(src,p);
                    } else {
                        std::string val(condinfo.pUserAttr->GetUserAttr(attr.c_str()));
                        if(val.empty()){
                            theLog->WriteLog(log_warning, "policy obligation parser error (user attr:%s not find)",p,attr.c_str());
                           // printf("condition error:%s",p);
                            return "";
                        } else {
                            if(is_num(val)){
                                ret += val;
                            } else {
                                ret += '\'';
                                ret += val;
                                ret+= '\'';
                            }
                            ret+=' ';
                        }
                    }
                } else {
                    //error;
                    return error_opr(src,p);
                }

            }break;
            case '>': {
                if(src[p+1]=='='){
                    ret += ">=";
                    p += 2;

                } else {
                    ret += '>';
                    p++;
                }
            } break;
            case '<':{
                if(src[p+1]=='='){
                    ret += "<=";
                    p+=2;
                } else {
                    ret += '<';
                    p++;
                }
            } break;
            case '!':{
                if(src[p+1]=='='){
                    ret += "<>";
                    p += 2;
                } else {
                    return error_opr(src,p);
                }
            } break;
            case '=':{
                ret += '=';
                p++;
            } break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '-':{
                std::string str = parser_num(src, p, '\0');
                if(str.empty()){
                    return error_opr(src,p);
                } else {
                    ret += str;
                    ret += ' ';
                }

            }break;
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '\f':{
                p++;
                ret += ' ';
            }break;
            case '`':{
                std::string ident = parser_identifier(src, p,'`');
                if(!ident.empty()){
                    ret += get_field_qualify(condinfo.dbType, condinfo.tableName);
                    ret += '`';
                    ret += ident;
                    ret += '`';
                    fields.insert(ident);
                } else{
                    return error_opr(src,p);
                }
            }break;
            case '"':{
                if(condinfo.dbType == EMDB_DB_BIGQUERY){
                    std::string str = parser_string(src, p, '"');
                    if(str.empty()){
                        return error_opr(src,p);
                    } else{
                        ret += str;
                    }
                } else {
                    std::string ident = parser_identifier(src, p,'"');
                    if(!ident.empty()){
                        ret += get_field_qualify(condinfo.dbType, condinfo.tableName);
                        ret += '"';
                        ret += ident;
                        ret += '"';
                        fields.insert(ident);
                    } else {
                        return error_opr(src,p);
                    }
                }
                ret += ' ';
            }break;
            case '\'':{
                std::string str = parser_string(src, p, '\'');
                if(str.empty()){
                    return error_opr(src,p);
                } else {
                    ret += '\'';
                    ret += str;
                    ret += '\'';
                    ret += ' ';
                }
            }break;
            case '(':{//
                ret += src[p];
                p++;
                b_paren_in = true;
            }break;
            case ')':{//
                if(!b_paren_in)
                    return error_opr(src,p);
                ret += src[p];
                p++;
                b_paren_in = false;
            }break;
            default:{
                if(is_underline(src[p]) || is_letter(src[p])){
                    std::string ident = parser_identifier(src, p, '\0');
                    //todo: some reserved id also can be used as column name, so here something need to do
                    if(!ident.empty()){
                        if(is_reserved(ident)){
                            ret += ident;
                            ret += ' ';
                        } else {
                            ret += get_field_qualify(condinfo.dbType, condinfo.tableName);
                            ret += ident;
                            ret += ' ';
                            fields.insert(ident);
                        }
                    } else {
                        return error_opr(src,p);
                    }
                } else {
                    return error_opr(src,p);
                }
            }break;
        }
    }

    if(b_paren_in){
        theLog->WriteLog(log_warning, "policy obligation parser error (Parentheses must exist in pairs)");
        //printf("condition error:(Parentheses must exist in pairs)");
        return "";
    }
    return ret;
}
*/
/*
EMOperator EMCondition::TransformOp(const std::string & sop, SqlException & e) {
    if(sop.length() == 1) {
        if(sop[0] == '=') {
            return EMOP_EQ;
        } else if (sop[0] == '>') {
            return EMOP_GT;
        } else if (sop[0] == '<') {
            return EMOP_LT;
        }
    } else if (sop.length() == 2) {
        if (sop[0] == '>' && sop[1] == '=') {
            return EMOP_GE;
        } else if (sop[0] == '<' && sop[1] == '=') {
            return EMOP_LE;
        } else if (sop[0] == '!' && sop[1] == '=') {
            return  EMOP_NEQ;
        }else if (sop[0] == '<' && sop[1] == '>') {
            return  EMOP_NEQ;
        } else if(CommonFun::StrCaseCmp(sop.c_str(),"IN") == 0) {
            return  EMOP_IN;
        }
    } else if(sop.length() == 4) {
        if(CommonFun::StrCaseCmp(sop.c_str(),"LIKE") == 0){
            return  EMOP_LIKE;
        }
    } else if(sop.length() > 4) {
        if(sop.find("LIKE") > 0){
            if(sop.find("NOT") >= 0 ) {
                return EMOP_NOT_LIKE;
            } else {
                return  EMOP_LIKE;
            }
        }

    }
    if(!sop.empty())
        e.SetInfo(ERR_POLICY, "obligation hav nonsupport operation");
    return EMOP_EQ;
}

std::string EMCondition::Serialize(std::vector<EMCondition*> &conds, ConditionInfo & condinfo){
    return "";
}
*/
QueryPcResult::QueryPcResult() :_type(PCResult_Enforcer) {

}
QueryPcResult::QueryPcResult(const QueryPcResult& qpr) {
     _type = qpr._type;
     _deny = qpr._deny;
     _table_id = qpr._table_id;
}

EnforcerPcResult::~EnforcerPcResult(){
    for(auto it: _mask_map){
        for(auto itsub: *(it.second)){
            delete(itsub.second);
        }
        it.second->clear();
        delete(it.second);
    }
}

EnforcerPcResult::EnforcerPcResult(const EnforcerPcResult& eps) {
    //parent
    _type = eps._type;
    _deny = eps._deny;
    _table_id = eps._table_id;
    
    _filter_cond = eps._filter_cond;
    for (auto& it : eps._mask_map) {
        MaskItemMap* pitmap = it.second;
        MaskItemMap* pitmapnew = new MaskItemMap;
        for (auto& iti : (*pitmap)) {
            MaskItem* pit = iti.second;
            MaskItem* pitnew = new MaskItem;
            *pitnew = *pit;
            (*pitmapnew)[iti.first] = pitnew;
        }
        _mask_map[it.first] = pitmapnew;
    }
    _table_name = eps._table_name;

    //RequestInfo _auditlog;  //todo
}
/*
QueryPcResult * ParserObligation(IPolicyResult* result, std::string & action, ConditionInfo & condinfo, SqlException & e){
    int icount = result->ObligationCount();
    for (int i = 0; i < icount; ++i)
    {
        const IObligation* pobg = result->GetObligationByIndex(i);
        const char* pname = pobg->GetObligationName();
        printf("obg_name:%s\n", pname);
        const IAttributes* pattr = pobg->GetAttributes();
        int icout = pattr->Count();
        for (int j = 0 ; j < icout; ++j)
        {
            const char * sname = nullptr;
            const char * svalue = nullptr;

            CEAttributeType tp = XACML_string;
            pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

            std::string strname(sname);
            std::string strvalue(svalue);
            //policy_model_id
            if (strvalue.empty()) continue;

            printf("obg:(%s, %s)\n", sname, svalue);
        }
    }
}

QueryPcResult * QueryPc(std::string & action, EMDictionary & user, EMDictionary & resource, ConditionInfo & condinfo, SqlException & e){
    QueryPcResult * pret = NULL;
    IPolicyRequest* pRequest = CreatePolicyRequest();
    //action necessary
    pRequest->SetAction(action.c_str());

    //user attr  necessary
    IAttributes* pUser = CreateCEAttr();
    for(auto it: user){
        if(it.first.empty() || it.second.empty()) continue;
        pUser->AddAttribute(it.first.c_str(), it.second.c_str(), XACML_string);
    }
    pRequest->SetUserInfo("user", "User", pUser);

    // Set source info, this is mandatory
    IAttributes* pSourceAttr = CreateCEAttr();
    for(auto it: user){
        if(it.first.empty() || it.second.empty()) continue;
        pSourceAttr->AddAttribute(it.first.c_str(), it.second.c_str(), XACML_string);
    }
    pRequest->SetSource("EMDB", "EMDATABASE", pSourceAttr);

    //host info necessary
    pRequest->SetHostInfo("HostName", "10.23.60.102", NULL);

    const IPolicyRequest* request[] = { pRequest };
    IPolicyResult* result[sizeof(request) / sizeof(request[0])] = { 0 };
    while (true)
    {
        IPolicyResult* result = NULL;
        QueryStatus qs0 = CheckSingleResource(pRequest, &result);
        if (QS_S_OK != qs0 || result == NULL) {
            e.SetInfo(ERR_POLICY, "no match policy");
            return NULL;
        }
        //QueryStatus qs1 = CheckMultiResource(&request[0], sizeof(request)/sizeof(request[0]), result);
        if (result)
        {
            pret = ParserObligation(result, action, condinfo, e);
            FreePolicyResult(result);
        }
        break;
    }
    return pret;
}
*/
/*
bool is_separator_index(const std::string & src, unsigned int pos, const std::string & sep){
    size_t len = sep.length();
    if(sep.empty()) return false;
    if(src.length() < len+pos) return false;
    for(size_t i = 0; i < len; i++){
        if(src[pos+i] != sep[i]) return false;
    }
    return true;
}

void parser_string2array(std::vector<std::string> &vec, const std::string & src, const std::string & sep){
    size_t len = src.length();
    unsigned int p = 0;
    unsigned int last = 0;
    while(p < len){
        if(is_separator_index(src,p,sep)){
            vec.push_back(src.substr(last, p-last));
            p+=sep.length();
            last = p;
        } else {
            p++;
        }
    }
    vec.push_back(src.substr(last));
}
*/
std::string constrct_res_id(const EMResDictionary & resource){
    std::string ret;
    if(resource.find("database") != resource.end()){
        ret += resource.find("database")->second->strValue;
        ret +='.';
    }
    if(resource.find("schema") != resource.end()){
        ret += resource.find("schema")->second->strValue;
        ret +='.';
    }
    if(resource.find("table") != resource.end()){
        ret += resource.find("table")->second->strValue;
    }
    return ret;
}
std::string constrct_res_id2(const EMResDictionary2 & resource){
    std::string ret;
    auto it = resource.find("database");
    if(it != resource.end()){
        ResourceAttrValue * pv = it->second.get();
        if (!pv->strValue.empty()) {
            ret += pv->strValue;
            ret += '.';
        }
    }
    it = resource.find("schema");
    if(it != resource.end()){
        ResourceAttrValue * pv = it->second.get();
        if (!pv->strValue.empty()) {
            ret += pv->strValue;
            ret += '.';
        }
    }
    it = resource.find("table");
    if(it != resource.end()){
        ResourceAttrValue * pv = it->second.get();
        ret += pv->strValue;
    }

    return ret;
}

IPolicyRequest * create_request(const RequestInfo & req, const std::string & user_name){
    IPolicyRequest * ret = QueryCloudAZSDKCppWrapper::Instance()->CreatePolicyRequest();
    //action necessary
    ret->SetAction(req._action.c_str());

    //user attr  necessary
    IAttributes* pUser = QueryCloudAZSDKCppWrapper::Instance()->CreateCEAttr();
    std::string uname = user_name;
    std::string uid = user_name;
//    for(auto it = req._users.begin(); it != req._users.end(); ++it){
//        ResourceAttrValue * pv = it->second;
//        std::string sf = it->first;
//        if(sf.empty() || pv->strValue.empty()) continue;
//        if(pv->attrType == XACML_ArrStr){
//            std::string separtor = (*theConfig)[USER_val_separator];
//            std::vector<std::string> vec;
//            parser_string2array(vec,pv->strValue,separtor);
//            for(auto val:vec){
//                if(val.empty()) continue;
//                pUser->AddAttribute(sf.c_str(), val.c_str(), XACML_string);
//            }
//        } else if(pv->attrType == XACML_ArrInt){
//            std::string separtor = (*theConfig)[USER_val_separator];
//            std::vector<std::string> vec;
//            parser_string2array(vec,pv->strValue,separtor);
//            for(auto val:vec){
//                if(val.empty()) continue;
//                pUser->AddAttribute(sf.c_str(), val.c_str(), XACML_int);
//            }
//        }else{
//            pUser->AddAttribute(sf.c_str(), pv->strValue.c_str(), pv->attrType);
//        }
//        if(CommonFun::StrCaseCmp("id",sf.c_str()) == 0){
//            uid = pv->strValue;
//        } else if(CommonFun::StrCaseCmp("name",sf.c_str()) == 0){
//            uname = pv->strValue;
//        }
//    }

    for(auto it = req._users2.begin(); it != req._users2.end(); ++it){
        ResourceAttrValue * pv = it->second.get();
        std::string sf = it->first;
        if(sf.empty() || pv->strValue.empty()) continue;
        if(pv->attrType == XACML_ArrStr){
            std::string separator = EMDBConfig::GetInstance().get_usermode_multi_value_separator();
            std::vector<std::string> vec;
            CommonFun::SplitString(pv->strValue, separator, vec);
           // parser_string2array(vec,pv->strValue,separator);
            for(auto val:vec){
                if(val.empty()) continue;
                pUser->AddAttribute(sf.c_str(), val.c_str(), XACML_string);
            }
        } else if(pv->attrType == XACML_ArrInt){
            std::string separator = EMDBConfig::GetInstance().get_usermode_multi_value_separator();
            std::vector<std::string> vec;
            CommonFun::SplitString(pv->strValue, separator, vec);
            for(auto val:vec){
                if(val.empty()) continue;
                pUser->AddAttribute(sf.c_str(), val.c_str(), XACML_int);
            }
        }else{
            pUser->AddAttribute(sf.c_str(), pv->strValue.c_str(), pv->attrType);
        }
        if(CommonFun::StrCaseCmp("id",sf.c_str()) == 0){
            uid = pv->strValue;
        } else if(CommonFun::StrCaseCmp("name",sf.c_str()) == 0){
            uname = pv->strValue;
        }
    }


    ret->SetUserInfo(uid.c_str(), uname.c_str(), pUser);

    // Set source info, this is mandatory
    IAttributes* pSourceAttr = QueryCloudAZSDKCppWrapper::Instance()->CreateCEAttr();
//    for(auto it = req._rescs.begin(); it != req._rescs.end(); ++it){
//        if(it->first.empty() || it->second->strValue.empty()) continue;
//        pSourceAttr->AddAttribute(it->first.c_str(), it->second->strValue.c_str(), it->second->attrType);
//    }

    for(auto it = req._rescs2.begin(); it != req._rescs2.end(); ++it){
        ResourceAttrValue * pv = it->second.get();
        if(it->first.empty() || pv->strValue.empty()) continue;
        pSourceAttr->AddAttribute(it->first.c_str(), pv->strValue.c_str(), pv->attrType);
    }

   // std::string resname = constrct_res_id(req._rescs);
    std::string resname = constrct_res_id2(req._rescs2);

    ret->SetSource(resname.c_str(), EMDBConfig::GetInstance().get_policy_modelname().c_str(), pSourceAttr);

    //host info
    std::string hostname = "unknown";
    std::string hostip = "0.0.0.0";

    if (!req._client_host_name.empty()) {
        hostname = req._client_host_name;
    }

    if (!req._client_ip.empty()) {
        hostip = req._client_ip;
    }

    ret->SetHostInfo(hostname.c_str(), hostip.c_str(), NULL);


    std::string app_name = "unknown";
    std::string app_path = "unknown";

    if (!req._client_app_name.empty()) {
        app_name = req._client_app_name;
        app_path = req._client_app_name;
    }

    ret->SetAppInfo(app_name.c_str(), app_path.c_str(), "", NULL);


    if(TEST_LOG){
        std::string log;
        log +="subject(";
        for(auto it:req._users2){
            log+=it.first;
            log+=":";
            log+=it.second.get()->strValue;
            log+=",";
        }
        log +=")resource(";
        for(auto it:req._rescs2){
            log+=it.first;
            log+=":";
            log+=it.second.get()->strValue;
            log+=",";
        }
        log+=")action(";
        log+=req._action;
        log+=")host(";
        log+=hostname;
        log+=" ";
        log+=hostip;
        log+=")";
        theLog->WriteLog(log_info,log.c_str());
    }
    return ret;
}
QueryStatus  single_query_pc(const IPolicyRequest* pcRequest, IPolicyResult** pcResult){
    return QueryCloudAZSDKCppWrapper::Instance()->CheckSingleResource(pcRequest, pcResult);
}
QueryStatus  multi_query_pc (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult){
    return  QueryCloudAZSDKCppWrapper::Instance()->CheckMultiResource(pcRequest,nRequestCount, pcResult);
}
std::shared_ptr<QueryPcResult> parser_result_array(IPolicyResult * result, ConditionInfo & condinfo, SqlException & e){
    if(result ==NULL)
        return NULL;
    std::string action = condinfo._action;
    if(CommonFun::StrCaseCmp( action.c_str() ,ACTION_DAE_CONFIG) == 0 ) {
        std::shared_ptr<QueryPcResult> ret = std::make_shared<ConfigPcResult>();
        return ret;
    } else {
        std::shared_ptr<EnforcerPcResult> ret = std::make_shared<EnforcerPcResult>();
        //EnforcerPcResult * ret = new EnforcerPcResult();
        ret->SetTableId(condinfo.table_id);
        bool deny = result->GetEnforcement() == PolicyEnforcement::Deny;
        ret->SetDeny(deny);
        std::string cond;
        if(deny){
            return ret;
        }

        size_t icount = result->ObligationCount();
        if(icount == 0){
            ret.reset();
            return ret;
        } else {
            for (size_t i = 0; i < icount; ++i)
            {
                const IObligation* pobg = result->GetObligationByIndex(i);
                const char * pname = pobg->GetObligationName();
                std::string obname(pobg->GetObligationName());
                //printf("oblgation_name:%s\n", obname.c_str());
                const IAttributes* pattr = pobg->GetAttributes();

                if(CommonFun::StrCaseCmp(pname, "predicate_condition") == 0){
                    size_t icout = pattr->Count();
                    assert(icout==2);

                    for (size_t j = 0; j < icout; ++j)
                    {
                        const char * sname = nullptr;
                        const char * svalue = nullptr;

                        CEAttributeType tp = XACML_string;
                        pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                        std::string strname(sname);
                        std::string strvalue(svalue);

                        if (strvalue.empty()) continue;
                        if (strname.compare("condition") == 0) {
                            //filter condition
                            std::set<std::string> cond_fields;
                            std::string temp_cond = parser_condition_v2(strvalue, condinfo, cond_fields);
                            if(!temp_cond.empty()) {
                                if(ret->_filter_cond.empty()){
                                    ret->_filter_cond += "(";
                                    ret->_filter_cond +=temp_cond;
                                    ret->_filter_cond += ")";
                                } else {
                                    ret->_filter_cond += " AND (";
                                    ret->_filter_cond += temp_cond;
                                    ret->_filter_cond += ")";

                                }
                            } else {
                                ret->SetDeny(true);
                                theLog->WriteLog(log_warning, "predicate_condition parser failed, condition: %s", strvalue.c_str());
                            }
                        }
                        else if (strname.compare("policy_model_id") == 0) {

                        }
                    }
                } else if(CommonFun::StrCaseCmp(pname, "data_masking") == 0){
                    size_t icout = pattr->Count();
                    assert(icout==9);

                    std::string table;
                    std::set<std::string> fields;
                    std::set<std::string> cond_fields;
                    std::string mask_symbol;
                    std::string datatype;
                    std::string mask_format;
                    std::string mask_cond1;
                    std::string mask_cond2;
                    std::string mask_cond3;

                    for (size_t j = 0; j < icout; ++j)
                    {
                        const char * sname = nullptr;
                        const char * svalue = nullptr;

                        CEAttributeType tp = XACML_string;
                        pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                        std::string strname(sname);
                        std::string strvalue(svalue);
                        //MaskConditionMap map;

                        if (strvalue.empty()) continue;
                        if (strname.compare("mask_tables") == 0) {//Mask Tables	mask_tables	Single Row
                            unsigned int  pos = 0;
                            table = parse_identifier(strvalue,  pos);
                            //table = strvalue;
                        }else if (strname.compare("mask_fields") == 0) {//Mask Fields	mask_fields	Single Row
                            parse_fields(strvalue, fields);
                        }else if (strname.compare("type") == 0) {//OR Mask Condition	mask_cond3	Single Row
                            datatype = strvalue;
                        }else if (strname.compare("mask_format") == 0) {//Mask Format	mask_format	Single Row		[ms]
                            mask_format = strvalue;
                        }else if (strname.compare("mask_symbol") == 0) {//Mask Symbol	mask_symbol	Single Row
                            mask_symbol = strvalue;
                        }else if (strname.compare("mask_cond1") == 0) {//Mask Condition	mask_cond1	Single Row
                            mask_cond1 = parser_condition_v2(strvalue, condinfo, cond_fields);
                            if(mask_cond1.empty()){
                                theLog->WriteLog(log_warning, "mask_cond1 parser failed, condition: %s", strvalue.c_str());
                            }
                        }else if (strname.compare("mask_cond2") == 0 ) {//OR Mask Condition	mask_cond2	Single Row
                            mask_cond2 = parser_condition_v2(strvalue, condinfo, cond_fields);
                            if(mask_cond2.empty()){
                                theLog->WriteLog(log_warning, "mask_cond2 parser failed, condition: %s", strvalue.c_str());
                            }
                        }else if (strname.compare("mask_cond3") == 0 ) {//OR Mask Condition	mask_cond3	Single Row
                            mask_cond3 = parser_condition_v2(strvalue, condinfo, cond_fields);
                            if(mask_cond3.empty()){
                                theLog->WriteLog(log_warning, "mask_cond3 parser failed, condition: %s", strvalue.c_str());
                            }
                        }
                        //printf("obg:(%s, %s)\n", sname, svalue);
                    }

                    if(table.empty() || fields.size() == 0|| mask_symbol.empty()){
                        continue;
                    }

                    EMDataType dtp = MaskItem::GetDataType(condinfo.dbType, datatype );
                    if(dtp == EMDATA_NONSUPPORT){
                        theLog->WriteLog(log_warning, "datatype nonsupport, datatype: %s",datatype.c_str());
                    }
                    EMMaskFormat ft = MaskItem::GetMaskFormat(mask_format);
                    std::string conda = mask_cond1;
                    if(!mask_cond2.empty()){
                        if( conda.empty()){
                            conda = mask_cond2;
                        } else {
                            conda += " OR ";
                            conda += mask_cond2;
                        }
                    }
                    if(!mask_cond3.empty()){
                        if( conda.empty()){
                            conda = mask_cond3;
                        } else {
                            conda += " OR ";
                            conda += mask_cond3;
                        }
                    }

                    auto it = ret->_mask_map.find(table);
                    if(it == ret->_mask_map.end()){
                        MaskItemMap *sub = new MaskItemMap();
                        for(auto field:fields){
                            MaskItem * item = new MaskItem();
                            item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                            item->_condition = conda;
                            item->_datatype = dtp;
                            item->_format = ft;
                            item->_symbols = mask_symbol;

                            sub->insert(std::make_pair(field, item));
                        }
                        ret->_mask_map.insert(std::make_pair(table, sub));
                    } else {
                        MaskItemMap *sub = it->second;
                        for(auto field:fields){
                            auto itsub = sub->find(field);
                            if(itsub == sub->end()){
                                MaskItem * item = new MaskItem();
                                item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
                                item->_condition = conda;
                                item->_datatype = dtp;
                                item->_format = ft;
                                item->_symbols = mask_symbol;

                                sub->insert(std::make_pair(field, item));
                            } else {
                                //ignore
                            }
                        }
                    }
                }

            }
        }
        return ret;
    }

}

void free_pc_request(IPolicyRequest* p){
    assert(p);
    if(NULL!=p->GetUserAttr()){
        QueryCloudAZSDKCppWrapper::Instance()->FreeCEAttr(const_cast<IAttributes*>(p->GetUserAttr()));
    }
    if(NULL!=p->GetSourceAttr()){
        QueryCloudAZSDKCppWrapper::Instance()->FreeCEAttr(const_cast<IAttributes*>(p->GetSourceAttr()));
    }
    QueryCloudAZSDKCppWrapper::Instance()->FreePolicyRequest(p);
}
void free_pc_result(IPolicyResult* p){
    QueryCloudAZSDKCppWrapper::Instance()->FreePolicyResult(p);
}
void free_pc_result(std::vector<QueryPcResult *> & vp){
    for (auto it:vp) {
        delete(it);
    }
    vp.clear();
}



bool get_host(std::string *phost,std::string  *pip){
#ifdef WIN32
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    int retstart = ::WSAStartup(sockVersion, &wsaData);
    if (retstart != 0) {
        return false;
    }
#endif // WIN32

    char               buf[100];
    int                ret = 0;
    struct addrinfo    hints;
    struct addrinfo* res = NULL;
    struct addrinfo *curr = NULL;
    struct sockaddr_in *sa;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int iRet = gethostname(buf, sizeof(buf));
    if (iRet == 0) {

        if ((ret = getaddrinfo(buf, NULL, &hints, &res)) == 0) {
            curr = res;
            while (curr ) {
                sa = (struct sockaddr_in *)curr->ai_addr;
                //string sname = curr->ai_canonname;//hostname
                *phost =  curr->ai_canonname;
                *pip = inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof(buf));
                break;
            }
        }
    }
    freeaddrinfo(res);

#ifdef WIN32
    ::WSACleanup();
#endif // WIN32
    if(pip->empty())
        return false;
    return true;
}


bool get_app(std::string *app_name,std::string  *app_path){
#ifdef _WIN32
    char szfile_path[MAX_PATH];
    ::GetModuleFileNameA(NULL, szfile_path, MAX_PATH);
    std::string file_path = szfile_path;
    auto found = file_path.rfind("\\");
#else
    std::string file_path;
    zise_t found = std::string::npos;
#endif
    if(found != std::string::npos){
        *app_name = file_path.substr(found + 1);
        *app_path = file_path;//filePath.substr(0, found);
        return true;
    } else{
        return false;
    }
}




#ifdef WIN32
#pragma warning(pop)
#endif