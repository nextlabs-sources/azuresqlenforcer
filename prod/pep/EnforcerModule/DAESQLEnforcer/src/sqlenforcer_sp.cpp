//
// Created by jeff on 2021/3/18.
//

#include "sqlenforcer_sp.h"
#include "commfun.h"
#include <set>
#include <string.h>
#ifdef WINDOWS_APP_DEMO
bool EMDBSP_parser_sp_cmd(const std::vector<std::string>& values, EMDB_SP_INFO & info);

bool is_digit_(char c) { return '0' <= c && c <= '9'; }
bool is_underline_(char c) { return '_' == c; }
bool is_letter_(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }

bool is_write_(char c){
    return c==' '||c=='\n'||c=='\t'||c=='\r'||c=='\f';
}
bool is_op_(char c){
    return c=='>'||c=='<'||c=='='||c=='!';
}
bool is_paren_(char c){
    return c=='(' || c==')' || c == '[' || c==']';
}

struct IgnoreCaseCmpare : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string& left, const std::string& right) const {
#ifndef WIN32
        return strcasecmp(left.c_str(), right.c_str()) < 0;
#else
        return stricmp(left.c_str(), right.c_str()) < 0;
#endif
    }
    static int compare(const std::string& left, const std::string& right) {
#ifndef WIN32
        return strcasecmp(left.c_str(), right.c_str());
#else
        return stricmp(left.c_str(), right.c_str());
#endif
    }

};

static std::set<std::string,IgnoreCaseCmpare> g_reserved_begin = {
        "SELECT","DELETE","UPDATE","INSERT","BEGIN","DECLARE","USE","CREATE","WITH","IF","CALL","DROP"
//        "ALL", "AND", "ANY", "ARRAY", "AS", "ASC", "AVG",
//        "BETWEEN", "BIGINT", "BINARY", "BLOB", "BOOLEAN", "BY",
//        "CASE", "CAST", "CALL", "CHAR", "CHARACTER", "CHARACTERS", "CLOB",
//        //"COALESCE", "CODE_UNITS",
//        "COLLATE", "CONVERT", "CORRESPONDING", "COUNT", "CROSS",
//        //"CUME_DIST", "CURRENT", "CURRENT_TIMESTAMP", "CURRENT_USER",
//        "DATE", "DAY", "DEC", "DECIMAL", "DEFAULT", "DELETE",
//        //"DENSE_RANK",
//        "DESC", "DISTINCT", "DOUBLE", "ELSE",
//        "END", "END_P", "ESCAPE", "EXCEPT", "EXCLUDE", "EXISTS",
//        "FALSE","FLOAT", "FOLLOWING", "FOR", "FROM", "FULL",
//        "G", "GROUP", "GROUPING",
//        "HAVING", "HOUR", "IN", "INNER", "INSERT", "INT", "INTEGER", "INTERSECT", "INTERVAL", "INTO", "IS",
//        "JOIN", "K", "LARGE", "LEFT", "LIKE", "M", "MAX", "MIN", "MINUTE", "MOD", "MONTH", "MULTISET",
//        "NATIONAL", "NATURAL", "NCHAR", "NCLOB", "NO", "NOT", "NULLIF", "NUMERIC", "NULL",
//        "OBJECT", "OCTETS", "OF", "ON", "ONLY", "OR", "ORDER", "OTHERS", "OUTER", "OVER",
//        "PARTITION",
//        //"PERCENT_RANK", "PRECEDING", "PRECISION",
//        "RANGE", "RANK", "READ", "REAL",
//        "RECURSIVE", "REF", "RIGHT", "ROW", "ROWS",
//        //"ROW_NUMBER",
//        "SCOPE", "SECOND", "SELECT",
//        "SESSION_USER", "SET", "SETS", "SMALLINT", "SOME", "STDDEV_POP",
//        "SUM",
//        //"SYSTEM_USER",
//        "TRUE", "THEN", "TIES", "TIME", "TIMESTAMP", "TO", "UNBOUNDED", "UNION", "UPDATE", "USING", "VALUES", "VARCHAR", "VARYING",
//        "VAR_POP", "VAR_SAMP", "WHEN", "WHERE", "WITH", "WITHOUT", "YEAR", "ZONE", "LIMIT", "OFFSET"
};

std::string parser_identifier(const std::string &src, unsigned int & p, char c){
    bool bquote = false;
    size_t len = src.length();
    std::string ret;
    while(p < len){
        if(!bquote) { //
            if (is_write_(src[p]) || is_op_(src[p]) || src[p] == ';'){
                break;
            } else if ('.'==src[p]){
                    break;
            }else if(is_underline_(src[p]) || is_letter_(src[p]) || src[p]=='-' || is_digit_(src[p])){

            } else {
                return "";

            }
        } else {
//            if(src[p] == c && src[p-1] != '\\'){
//                p++;
//                break;
//            }
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
        if(src[p] == c && src[p-1] != c){
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
        if(is_write_(src[p]) || is_op_(src[p]) || src[p] == ','|| src[p] == ';')
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


std::string  EMDBSP_output_new_sp(const EMDB_SP_INFO & info){
    std::string ret;
    if(!info._call_str.empty()){
        ret += info._call_str;
        ret += ' ';
    }
    if(!info._qualifier.empty()) {
        ret += info._qualifier;
    }
    ret += info._new_sp;
    ret += ' ';

    std::string all_condi;
    for(auto it: info._params){
        if(!all_condi.empty()){
            all_condi += ',';
        }
        if(!it.second._paramname.empty()){
            all_condi += it.second._paramname;
            all_condi += "=";
        }
        all_condi += it.second._condition;

    }
    if(!all_condi.empty()){
        ret += all_condi;
    }

    return ret;
}

bool EMDBSP_carete_indirect_sql(std::map<unsigned int,std::string> &sqls,const  EMDB_SP_INFO & info){
    for(auto it: info._params){
        if(it.second._table.empty())
            continue;
        std::string sql = "select invalid_column from ";
        if(!it.second._db.empty()){
            sql += it.second._db;
            sql += '.';
        }
        if(!it.second._schema.empty()){
            sql += it.second._schema;
            sql += '.';
        }
        sql += it.second._table;
        sql += ' ';
        if(!it.second._alias.empty()){
            sql += it.second._alias;
        }
        sqls.insert(std::make_pair(it.first, sql));
    }
    return sqls.size()>0;
}


bool EMDBSP_parser_sp_cmd(const std::string & sql, EMDB_SP_INFO & info){
    unsigned int len = sql.length();
    unsigned int pos = 0;
    std::vector<std::string> values;
    while(pos < len){
        char c = sql[pos];
        switch (c){
            case '\0':{
                break;
            }
            case ' ':
            case '\t':
            case '\r':
            case '\f':
            case '\n':{
                pos++;
            } break;
            case ',':{ //
                values.push_back(",");
                pos++;
            }break;
            case '\'':{ // string
                std::string ss = parser_string(sql, pos, '\'');
                if(!ss.empty()){
                    values.push_back(ss);
                }
            }break;
            case '.':{
                values.push_back(".");
                pos++;
            }break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':{
                std::string ss = parser_num(sql, pos,'a');
                if(!ss.empty()){
                    values.push_back(ss);
                }
            } break;
            case ';':{
                pos++;
            } break;
            case '/':{
                pos++;
                if(sql[pos] == '*'){
                    while(true){
                        pos++;
                        if(sql[pos-1] == '*'&&sql[pos] == '/'){
                            pos++;
                            break;
                        }
                    }
                } else {
                    return  false;
                }
            }break;
            case '(':
            case '#':
            case '@':{
                return  false;
            }break;
            default:{
                std::string ss = parser_identifier(sql, pos, 'a');
                if(!ss.empty()){
                    if(values.size() == 0){
                        if(g_reserved_begin.find(ss) != g_reserved_begin.end())
                            return  false;
                    }

                    values.push_back(ss);
                }
            }break;
        }
    }


    bool ret = EMDBSP_parser_sp_cmd(values, info);
    if(ret){
        if(info._ori_sp.empty()){
            return  false;
        }
    }
    return ret;

}
bool EMDBSP_parser_sp_cmd(const std::vector<std::string> &values, EMDB_SP_INFO & info){
    bool getname = false;
    unsigned  int len = values.size();
    int params_id = 0;
    for(unsigned  int i = 0; i < values.size(); ++i){
        const std::string & val = values[i];
        if(i == 0){
            if(CommonFun::StrCaseCmp(val.c_str(), "EXEC") == 0 ||
               CommonFun::StrCaseCmp(val.c_str(), "EXECUTE") == 0
                    ) {
                info._qualifier += val;
                info._qualifier += ' ';

            } else {
                if(len > i+1 && values[i+1]=="."){
                    //schema
                    i++;
                    info._qualifier += val;
                    info._qualifier += '.';
                } else {
                    info._ori_sp = val;
                    getname = true;
                }
            }
            continue;
        }
        if(getname == true){
            //params
            if(val == ","){

            } else {
                EMDB_SP_TABLES tbs;
                tbs._condition = val;
                info._params.insert(std::make_pair(params_id, tbs));
                params_id ++;
            }
        } else {
            if(len > i+1 && values[i+1]=="."){
                //schema
                i++;
                info._qualifier += val;
                info._qualifier += '.';
            } else {
                info._ori_sp = val;
                getname = true;
            }
        }

    }
    return true;
}


bool EMDBSP_load_new_sp(EMDB_SP_INFO & info){
    if(info._ori_sp.empty()){
        return false;
    }
    if(CommonFun::StrCaseCmp("dsp_userinfo_by_salary_range", info._ori_sp.c_str()) == 0 &&
            CommonFun::StrCaseCmp("dsp_userinfo_by_salary_range_EMDB", info._ori_sp.c_str()) != 0){
        //compare
        EMDB_SP_TABLES tbs;
        tbs._db = "Demo";
        tbs._schema = "dbo";
        tbs._table = "salary";
        tbs._alias = "b";
        tbs._paramname = "@condition";
        info._new_sp = "dsp_userinfo_by_salary_range_EMDB";
        info._params.insert(std::make_pair(2,tbs));
    } else {
        return false;
    }

    return true;

}

std::string escape_string(const std::string & src){
    unsigned  int len = src.length();
    unsigned int ipos = 0;
    std::string ret;
    ret += '\'';
    while(ipos < len){
        if(src[ipos]=='\''){
            ret+='\'';
        }
        ret+=src[ipos];
        ipos++;
    }
    ret += '\'';
    return  ret;
}

bool EMDBSP_resolve_new_sp(const std::map<unsigned int,std::string> & sqls, EMDB_SP_INFO & info){
    int ifind = std::string::npos;
    for(auto it: sqls){
        ifind = it.second.find("WHERE");
        if(std::string::npos != ifind ){
            std::string cond = it.second.substr(ifind+5);
            auto itp = info._params.find(it.first);
            if(itp != info._params.end()){
                itp->second._condition = escape_string(cond);
            }
            break;
        }
    }
    return std::string::npos != ifind ;
}

#endif