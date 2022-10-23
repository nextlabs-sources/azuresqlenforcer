//
// Created by jeff on 2020/3/18.
//
#include "EMMaskOpr.h"

#include <vector>
#include <string>
#include <algorithm>

#include <assert.h>

#include "commfun.h"
#include "EMColumnRef.h"
#include "serialize_format.h"

bool is_num(const std::string & ss);
bool is_dec(char c);
int str_case_cmp(const char *s1, const char *s2);
std::string escape_chars(EMDB_DB_TYPE dbtype,const std::string &  src,char c);

bool check_date_time_format(EMDB_DB_TYPE dbtype, EMDataType type, const std::string & symbols);
std::string get_default_symbols(EMDataType type, EMDB_DB_TYPE dbtype);
std::string adjustment_mask_symbols_oracle(std::string & symbols, EMDataType type);
std::string get_rand_string_for_dbtype(int start_,int end_,EMDB_DB_TYPE dbtype);
std::string get_partial_string_for_dbtype(EMDB_DB_TYPE dbtype, const std::string& field, const std::string&symbols, int start, int end);
std::string get_mask_symbols(EMDB_DB_TYPE dbtype, EMDataType type, EMMaskFormat format_, const std::string & symbols, const std::string & field);
std::string make_one_mask_expression(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, SqlException & e);
std::string make_one_mask_expression_no_as(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, SqlException & e);
bool mask_one_field(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, MaskOprType masktype, SqlException & e);
bool mask_star_field_bigquery(ISelectStmt * stmt, AsteriskExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, SqlException & e);
bool mask_star_field_20210618(ISelectStmt * stmt, AsteriskExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, SqlException & e);
bool mask_fields_v3(ISelectStmt * stmt,RawExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, MaskOprType masktype, SqlException & e);

int str_case_cmp(const char *s1, const char *s2) {
#ifndef WIN32
    return strcasecmp(s1, s2);
#else
    return stricmp(s1, s2);
#endif
}

bool mask_star_field_20210618(ISelectStmt * stmt, AsteriskExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, SqlException & e){
    //select * replace(val as col ) from tb
    std::map<std::string, std::string> replace_map; // map<colname, mask_expression>
    std::string str_field;
    std::string mask_expression;

    Node * node = NULL;
    std::vector<const ITableItem *> tbis;
    tbis = root->GetTbItems();
    node = root->GetNode();

    if(!node){
        e.SetInfo(ERR_MASK_OPR,"select from item parser failed.");
        return false;
    }

    std::vector<std::string> using_cols;//for oracle join table
    std::string expr;

    for (auto tbi: tbis) {

        std::string tbalias = tbi->GetTableAliasReal();
        TableRef * tbiref = (TableRef*)tbi;
        if( tbiref->GetTableRefType() == TableRef::FUNCTION_TABLE_REF
            || tbiref->GetTableRefType() == TableRef::GENERATED_TABLE_REF){// function
            if(!tbalias.empty()){
                if(!expr.empty()){
                    expr += ", ";
                }
                expr += tbalias;
                expr += ".* ";
            } else {
                e.SetInfo(ERR_MASK_OPR,"function table/subquery  need alias while mask fields");
                return false;
            }
        } else if(tbiref->GetTableRefType() == TableRef::CTE_TABLE_REF) { //cte
            CteTableRef * ttbi = (CteTableRef*)tbi;
            if(!expr.empty()){
                expr += ", ";
            }
            if(!tbalias.empty()){
                expr += tbalias;
                expr += ".* ";
            } else {
                expr += ttbi->GetCteName();
                expr += ".* ";
            }
        } else {//table
            std::string tbname = tbi->GetTableName();
            std::string qualify = tbi->GetTableAliasName().empty()? tbi->GetTableObject():tbi->GetTableAliasName();

            MaskItemMap *pmap = NULL;
            auto it = map_condition->find(tbname);
            if (it != map_condition->end()) {
                pmap = it->second;// false;.
            }

            const MetadataVec & vecmeta = tbi->GetMetadata();
            if(vecmeta.size() == 0){
                if (pmap != NULL) {
                    e.SetInfo(ERR_MASK_OPR, "get the mask table's metadata failed.");
                    return false;
                }
                if(!expr.empty()){
                    expr += ", ";
                }
                if(!tbalias.empty()){
                    expr += tbalias;
                    expr += ".* ";
                } else {
                    expr += qualify.empty()?"":qualify+".";
                    expr += "* ";
                }
            }
            //jeff 2021-06-18
            std::vector<std::string> usingv =  tbiref->GetUsingColumns();// for oracle
            for(auto & col : usingv){
                bool bfind = false;
                for(auto & col2: using_cols){
                    if(CommonFun::CaseInsensitiveEquals(col, col2)){
                        bfind = true;
                    }
                }
                if(!bfind){
                    using_cols.push_back(col);
                }
            }

            for(size_t i = 0 ; i < vecmeta.size();++i){
                const Metadata & meta = vecmeta[i];
                if(usingv.size() > 0){// for oracle
                    bool continue_ = false;
                    for(auto & col : usingv){
                        if(CommonFun::CaseInsensitiveEquals(col, meta._col)){
                            continue_ = true;
                            break;
                        }
                    }
                    if(continue_){
                        continue;
                    }
                }
                if(!expr.empty()){
                    expr += ", ";
                }

                // To fix bug 70636
                std::string column_name = meta._col;
                if (EMDB_DB_SQLSERVER == dbtype) {
                    column_name = "[" + meta._col + "]";
                }

                if(pmap && pmap->find(meta._col) != pmap->end()){
                    EMColumnInfo info;
                    info._alias = false;
                    info._field = column_name;
                    Node *pnode = Node::makeTerminalNode(E_STRING, qualify + "." + column_name);
                    info._node = pnode;
                    expr +=  make_one_mask_expression(&info, pmap->find(meta._col)->second,dbtype, e);
                    delete(pnode);
                } else {
                    expr += qualify.empty()?"":qualify+".";
                    expr += column_name;
                }

            }
        }

    }
    if(expr.empty()){
        //e.SetInfo(ERR_MASK_OPR,"no table item, cannot mask.");
        return true;
    }
    //for oracle join table using cols
    std::string using_col_expr;
    for(auto & col:using_cols){
        using_col_expr += col;
        using_col_expr += ", ";
    }
    if(!using_col_expr.empty())
        expr = using_col_expr + expr;

    //swap expr and star
    Node * pnode_new = Node::makeTerminalNode(E_STRING, expr);
    Node * pnode = root->GetNode();
    *pnode = *pnode_new;
    delete(pnode_new);
    return true;
}

bool mask_star_field_bigquery(ISelectStmt * stmt, AsteriskExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, SqlException & e){// for BigQuery
    //select * replace(val as col ) from tb
    std::map<std::string, std::string> replace_map; // map<colname, mask_expression>
    std::string str_field;
    std::string mask_expression;

    Node * node = NULL;
    std::vector<const ITableItem *> tbis;
    tbis = root->GetTbItems();
    node = root->GetNode();

    if(!node){
        return false;
    }
    bool bret = false;

    for (auto tbi: tbis) {
        std::string tbname = tbi->GetTableName();
        std::string tbalias = tbi->GetTableAliasReal();

        MaskItemMap *pmap = NULL;
        auto it = map_condition->find(tbname);
        if (it != map_condition->end()) {
            pmap = it->second;// false;.
        }
        if (!pmap)
            continue;
        //construct mask expression ------ case when

        for(auto maskit: (*pmap)){
            std::string field_name = maskit.first;
            //if replace_expr already have this field
            auto itf = root->_replace._col2node.find(field_name);
            if(itf != root->_replace._col2node.end()){
                Node * expr_as_lb = itf->second;

                EMColumnInfo info;
                info._alias = true;
                info._node = expr_as_lb;
                std::string mask_expr = make_one_mask_expression(&info, maskit.second,dbtype, e);
//                delete(expr_as_lb->getChild(0));
//                delete(expr_as_lb->getChild(1));
                expr_as_lb->isTerminalToken = true;
                Node *pnode = Node::makeTerminalNode(E_STRING, mask_expr);
                (*expr_as_lb)=(*pnode);
                delete(pnode);
                //replace_map.insert(std::make_pair(maskit.first, mask_expr));
                bret = true;
            } else{
                
                bool continue_ = false;
                for(auto it: root->_exceptexpr._cols){
                    if(CommonFun::CaseInsensitiveEquals(it, field_name)){
                        continue_ = true;
                        break;
                    }
                }
                if(continue_){
                    continue;
                }
                EMColumnInfo info;
                info._alias = false;
                info._field = field_name;
                std::string mask_expr = make_one_mask_expression(&info, maskit.second,dbtype, e);
                replace_map.insert(std::make_pair(maskit.first, mask_expr));
            }
        }
    }
    
    //-----------
    if(replace_map.size() <= 0){
        return bret;
    }
    //-----------
    if(root->_replace._col2node.size() > 0 ){
        Node * nodel  = root->_replace._node->getChild(0);
        for(auto it: replace_map){

            Node *pnode = Node::makeTerminalNode(E_STRING, it.second);
            Node * node_new = Node::makeNonTerminalNode(E_ROW_EXPR_AS_LABEL_LIST, E_LIST_PROPERTY_CNT, pnode, nodel);
            node_new->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
            //root->_replace._node = node_new;
            nodel = node_new;
        }
        //Node * node_replace = root->_replace._node;//E_ROW_EXPR_AS_LABEL_LIST
        Node * node_rep = node->getChild(2);//E_REPLACE_EXPR
        node_rep->setChild(0, nodel);
    } else {
        //
        std::string replace_node ;
        replace_node += " REPLACE(";
        size_t i = replace_map.size();
        for(auto it: replace_map){
            replace_node += it.second;
            if(--i != 0){
                replace_node += ", ";
            }
        }
        replace_node += ")";

        Node *pnode = Node::makeTerminalNode(E_STRING, replace_node);
        node->SetChild(2, pnode);
    }

    return true;
}

bool is_dec(char c) {
    return '0' <= c && c <= '9';
}

bool is_num(const std::string & ss){
    unsigned int pos = 0;
    if(ss.length() <= 0) return false;
    while(pos < ss.length()) {
        if(!is_dec(ss[pos])
           && '.' != ss[pos]
           && ss[0] != '-' )
            return false;
        pos++;
    }
    return true;
}

std::string escape_chars(EMDB_DB_TYPE dbtype,const std::string &  src,char c){
    //assert(c == '"' || c == '`' || c == ']');
    std::string ret ;
    if(dbtype == EMDB_DB_ORACLE) {
        if(c == '\''){
            ret += "'";
            for (size_t i = 0; i < src.length(); ++i) {
                if (src[i] == '\'' ) {
                    ret += "'";
                }
                ret += src[i];
            }
            ret += "'";
        } else if(c == '"'){
            ret += "\"";
            for (size_t i = 0; i < src.length(); ++i) {
                if (src[i] == '"' ) {
                    //error, field not allow contaion '"'
                }
                ret += src[i];
            }
            ret += "\"";
        }
    } else {
        if(c == '\''){
            ret += "'";
            for (size_t i = 0; i < src.length(); ++i) {
                if (src[i] == '\'' || src[i] == '\\') {
                    ret += "\\";
                }
                ret += src[i];
            }
            ret += "'";
        } else if(c == '"'){
            ret += "\"";
            for (size_t i = 0; i < src.length(); ++i) {
                if (src[i] == '"' || src[i] == '\\') {
                    ret += "\\";
                }
                ret += src[i];
            }
            ret += "\"";
        }
    };


    return ret;
}

std::string get_default_symbols(EMDataType type, EMDB_DB_TYPE dbtype){
    std::string ret ;
    switch(type){
        case EMDATA_STRING:
        case  EMDATA_STRING2:
            ret =  "'****'";
            break;
        case EMDATA_NUMBER:
            ret = "-9999";
            break;
        case EMDATA_DATE:
            ret = "'1900-01-01'";
            break;
        case EMDATA_TIME:
            ret = "'00:00:00'";
            break;
        case EMDATA_TIMESTAMP:
        case EMDATA_TIMESTAMP_TZ_ORA:{
            ret = "'1900-01-01 00:00:00'";
            break;
        }
        case EMDATA_DSINTERVAL:{
            ret = "'0 00:00:00'";
        } break;
        case EMDATA_YMINTERVAL:{
            ret = "'0-0'";
        }break;
        default:
            ret = "'****'";
    }
    if(dbtype == EMDB_DB_ORACLE){
        ret = adjustment_mask_symbols_oracle(ret, type);
    }
    return ret;
}



std::string get_rand_string_for_dbtype(int start_,int end_,EMDB_DB_TYPE dbtype){
    std::string ret;
    switch(dbtype){
        case EMDB_DB_ORACLE:{
            char cret[1024] = {0};
            sprintf(cret, "CEIL(dbms_random.value(%d,%d))", start_, end_);
            ret = cret;
        } break;
        case EMDB_DB_HANA:
        case EMDB_DB_BIGQUERY:{  // same with EMDB_DB_HANA
            int irange = end_-start_;
            //ROUND(RAND()*irange)+ start_i
            char cret[1024] = {0};
            sprintf(cret, "ROUND(RAND()*%d)+ %d", irange, start_);
            ret = cret;
        } break;
        case EMDB_DB_DB2:
        {
            int irange = end_-start_;
            char cret[1024] = {0};
            sprintf(cret, "INT(RAND()*%d + %d)", irange, start_);
            ret = cret;
        } break;
        case EMDB_DB_SQLSERVER:{
            int irange = end_-start_;
            //ROUND(RAND()*irange)+ start_i
            char cret[1024] = {0};
            sprintf(cret, "ROUND(RAND()*%d,0)+ %d", irange, start_);
            ret = cret;
        }break;
        default: ret = "-9999"; break;

    }
    return ret;
}

std::string adjustment_mask_symbols_oracle(std::string & symbols, EMDataType type){
    std::string ret;
    switch (type){
        case EMDATA_STRING:{

        } break;
        case EMDATA_STRING2:{   // if is oracle   then datatype is NVARCHAR2/NCHAR
            ret = "TO_NCHAR("  + symbols + ")";
        } break;
        case EMDATA_DATE:{
            ret = "TO_DATE(" + symbols + ",'YYYY-MM-DD')";
        } break;
        case EMDATA_TIME:
        case EMDATA_TIMESTAMP:{
            ret = "TO_TIMESTAMP(" + symbols + ",'YYYY-MM-DD HH24:MI:SS.FF9')";
        } break;
        case EMDATA_TIMESTAMP_TZ_ORA:
        {
            ret = "TO_TIMESTAMP_TZ(" + symbols + ",'YYYY-MM-DD HH24:MI:SS.FF9 TZH:TZM')";
        }break;
        case EMDATA_YMINTERVAL:
        {
            ret = "TO_YMINTERVAL(" + symbols + ")";
        } break;
        case EMDATA_DSINTERVAL:{
            ret = "TO_DSINTERVAL(" + symbols + ")";
        } break;
        default:break;
    }
    if(ret.empty()){
        ret = symbols;
    }
    return ret;
}

std::string get_partial_string_for_dbtype(EMDB_DB_TYPE dbtype, const std::string& field, const std::string&symbols, int start, int end){
    std::string ret;
    char cret[255] = {0};
    unsigned int nprint=0;
    switch (dbtype){
        case EMDB_DB_ORACLE:
        case EMDB_DB_BIGQUERY:{
            if(start == 0){
                if(end == 0){
                    nprint = snprintf(cret, sizeof(cret), "%s",symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(%s,SUBSTR(%s,-%d)", symbols.c_str(), field.c_str(), end);
                }
            } else {
                if (end == 0) {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,0,%d),%s)", field.c_str(), start,
                                      symbols.c_str());
                } else {
                    if(dbtype == EMDB_DB_ORACLE ) {
                        nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,0,%d),CONCAT(%s,SUBSTR(%s,-%d)))", field.c_str(), start, symbols.c_str(), field.c_str(), end);
                    } else {
                        nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,0,%d),%s,SUBSTR(%s,-%d))", field.c_str(),start,symbols.c_str(),field.c_str(),end);
                    }
                }
            }
        } break;
        case EMDB_DB_SQLSERVER:{
            if(start == 0){
                if(end == 0){
                    nprint = snprintf(cret, sizeof(cret), "%s",symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(%s, SUBSTRING(%s,LEN(%s)-%d, %d))", symbols.c_str(), field.c_str(), field.c_str(), end, end);
                }
            } else {
                if (end == 0) {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTRING(%s,1,%d),%s)", field.c_str(), start, symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTRING(%s,1,%d),%s,SUBSTRING(%s,LEN(%s)-%d, %d))",
                            field.c_str(), start, symbols.c_str(), field.c_str(), field.c_str(), end-1, end);
                }
            }
        } break;
        case EMDB_DB_HANA:{
            if(start == 0){
                if(end == 0){
                    nprint = snprintf(cret, sizeof(cret), "%s",symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(%s, SUBSTR(%s,LENGTH(%s)-%d))", symbols.c_str(), field.c_str(), field.c_str(), end-1);
                }
            } else {
                if (end == 0) {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,0,%d),%s)", field.c_str(), start,
                                      symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTR(%s,0,%d),CONCAT(%s,SUBSTR(%s,LENGTH(%s)-%d)))",
                                      field.c_str(), start, symbols.c_str(), field.c_str(),field.c_str(), end-1);
                }
            }
        } break;        
        case EMDB_DB_DB2:{
            if(start == 0){
                if(end == 0){
                    nprint = snprintf(cret, sizeof(cret), "%s",symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(%s, SUBSTRING(%s,LENGTH(%s)-%d))", symbols.c_str(), field.c_str(), field.c_str(), end-1);
                }
            } else {
                if (end == 0) {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTRING(%s,0,%d),%s)", field.c_str(), start,
                                      symbols.c_str());
                } else {
                    nprint = snprintf(cret, sizeof(cret), "CONCAT(SUBSTRING(%s,0,%d),CONCAT(%s,SUBSTRING(%s,LENGTH(%s)-%d)))",
                                      field.c_str(), start, symbols.c_str(), field.c_str(),field.c_str(), end-1);
                }
            }
        } break;
        default: break;
    }

    //check error
    if (nprint>0 && nprint<sizeof(cret)){
        ret =  cret;
    }
    return ret;
}

int parser_num(const std::string & src, unsigned int &pos){
    unsigned int  ret=0;
    unsigned int len = (unsigned int)src.length();
    std::string val = "";
    bool bbreak = false;
    size_t zero_num = 0;
    while(pos<len)
    {

        char c = src[pos];
        switch (c){
            case '0':{
                zero_num++;
            }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{
                val += c;
            } break;
            default:{
                bbreak = true;
            } break;
        }
        if(bbreak){
            break;
        }
        pos++;
    }
    ret = atoi(val.c_str());
    if(ret == 0&& val.length() > zero_num){
        return -1;
    }
    return ret;
}

bool parser_date(const std::string & src, unsigned int &pos){
    unsigned int len = (unsigned int)src.length();
    bool year = false;
    bool mon = false;
    bool day = false;
    while(pos<len)
    {
        char c = src[pos];
        switch (c){
            case '-':{
                pos++;
            }break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{
                int val =  parser_num( src, pos);
                if(!year){
                    if(val>9999|| val<=0){
                        return false;
                    }
                    year = true;
                }else{
                    if(!mon){
                        if(val>12|| val<=0){
                            return false;
                        }
                        mon = true;
                    }else {
                        if(!day){
                            if(val>31|| val<=0){
                                return false;
                            }
                            day = true;
                        }else{
                            return false;
                        }
                    }
                }
            }break;
            default:
                return false;
        }

    }
    return day&&mon&&year;
}

bool parser_time(const std::string & src, unsigned int &pos){
    unsigned int len = (unsigned int)src.length();
    bool h = false;
    bool m = false;
    bool s = false;
    bool ms = false;
    while(pos<len)
    {
        char c = src[pos];
        switch (c){
            case ':':{
                pos++;
            }break;
            case ' ':{
                pos++;
            }break;
            case '.':{
                if(h && m && s){
                    pos++;
                } else{
                    return false;
                }
            }break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{
                int val =  parser_num( src, pos);
                if(!h){
                    if(val>23|| val<0){
                        return false;
                    }
                    h = true;
                }else{
                    if(!m){
                        if(val>59|| val<0){
                            return false;
                        }
                        m = true;
                    }else {
                        if(!s){
                            if(val>59|| val<0){
                                return false;
                            }
                            s = true;
                        }else{
                            if(!ms){
                                if(val>999|| val<0){
                                    return false;
                                }
                                ms = true;
                            }else{
                                return false;
                            }
                        }
                    }
                }
            }break;
            default:
                return false;
        }

    }
    return h&&m&&s;
}


bool check_date_time_format(EMDB_DB_TYPE dbtype, EMDataType type, const std::string & symbols){
    //
    if(dbtype != EMDB_DB_DB2){ // bug 65712 for db2
        return true;
    }
    if( EMDATA_DATE == type){
        unsigned int pos = 0;
        return parser_date(symbols,pos);
    }else if( EMDATA_TIMESTAMP == type || EMDATA_TIMESTAMP_TZ_ORA == type) {
        size_t ifind = symbols.find(' ');
        if(ifind != std::string::npos){
            std::string date = symbols.substr(0,ifind);
            unsigned  int pos = 0;
            bool bret = parser_date(date,pos);
            if(!bret)
                return false;
            bret = parser_time(symbols,pos);
            if(!bret)
                return false;
        } else {
            return false;
        }
    } else if(EMDATA_TIME == type ){
        unsigned int pos = 0;
        return parser_time(symbols,pos);
    }
    return true;
}

//oracle    example: SELECT CEIL(dbms_random.value(1,9)) as A, CONCAT('***',SUBSTR(CUSTA,0,2)) AS B FROM CUSTOMER
//bigquery  example: SELECT ROUND(RAND()*8)+1 AS A, CONCAT('***',SUBSTR(CUSTA,0,2)) AS B FROM CUSTOMER
//sqlserver example: SELECT ROUND(RAND()*8,0)+1 AS A, CONCAT(SUBSTRING(FIELD,1,start),'***',SUBSTRING(field,LEN(field)-end+1,end)) AS B FROM CUSTOMER
std::string get_mask_symbols(EMDB_DB_TYPE dbtype, EMDataType type, EMMaskFormat format_, const std::string & symbols, const std::string & field){
    std::string ret;
    switch(format_){
        case EMMASK_FULL:{
            if(symbols.empty()){
                return get_default_symbols(type, dbtype);
            }
            if(EMDATA_NUMBER == type){
                if(is_num(symbols)) {
                    ret =  symbols;
                } else {
                    ret = get_default_symbols(type, dbtype);
                }
            } else{
                if(check_date_time_format(dbtype, type, symbols)){
                    ret = escape_chars(dbtype, symbols, '\'');//"'" + symbols + "'";//
                } else {
                    ret = get_default_symbols(type, dbtype);
                }

            }
        } break;
        case EMMASK_RANDNUMBER:{
            if(EMDATA_NUMBER != type || symbols.empty()){
                return get_default_symbols(type, dbtype);
            }
            size_t pos =  symbols.find("/");
            if(std::string::npos != pos){
                std::string start_s = symbols.substr(0, pos);
                std::string end_s = symbols.substr(pos+1);
                int start_i = atoi(start_s.c_str());
                int end_i = atoi(end_s.c_str());
                if(start_i < end_i){
                    ret = get_rand_string_for_dbtype(start_i,end_i,dbtype);
                } else if(start_i == end_i){
                    ret = start_s;
                } else {
                    return get_default_symbols(type, dbtype);
                }
            } else {
                return get_default_symbols(type, dbtype);
            }
        } break;
        case EMMASK_PARTIAL:{
            if(EMDATA_STRING != type && EMDATA_STRING2 != type){
                return get_default_symbols(type, dbtype);
            }
            size_t pos1 =  symbols.find("/");
            size_t pos3 = symbols.rfind("/");
            if(pos3 - pos1 <= 1){
                return get_default_symbols(type, dbtype);
            }
            std::string pre_start = symbols.substr(0, pos1);
            std::string pre_end = symbols.substr(pos3+1);
            std::string midle_s = symbols.substr(pos1+1, pos3-pos1-1);
            midle_s = escape_chars(dbtype,midle_s, '\'');//"'" + symbols + "'";//

            int start_i = atoi(pre_start.c_str());
            int end_i = atoi(pre_end.c_str());
            if(start_i <0 || end_i <0){
                return get_default_symbols(type, dbtype);
            }
            ret =  get_partial_string_for_dbtype(dbtype, field, midle_s, start_i, end_i);

            if(ret.empty()){
                ret =  get_default_symbols(type, dbtype);
            }

        } break;
        case EMMASK_KEY:{
            ret =  get_default_symbols(type, dbtype);
        } break;
    }

    if(dbtype == EMDB_DB_ORACLE){
        ret = adjustment_mask_symbols_oracle(ret, type);
    }

    return ret;
}

std::string make_one_mask_expression(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, SqlException & e){
    std::string ret;

    assert(col);
    assert(maskitem);
    std::string alias;
    std::string field;
    if(col->_alias) {
        Node *pnode_field = col->_node->getChild(0);
        if (pnode_field) {
            field = pnode_field->serialize();
        }
        Node *pnode_alias = col->_node->getChild(1);
        if (pnode_alias) {
            alias = pnode_alias->serialize();
        }
    } else {
        const auto node = col->_node;

        if(node){
            field = node->serialize();
            
            const auto parent_node = node->GetParent();
            if(parent_node){
                const auto parent_node_type = parent_node->GetType();
                const static std::vector<NodeType> ignore_alias_list {
                        NodeType::E_OP_ADD, NodeType::E_OP_CNN, NodeType::E_OP_MINUS, NodeType::E_OP_MUL, NodeType::E_OP_DIV
                };
                if ( std::find(ignore_alias_list.cbegin(), ignore_alias_list.cend(), parent_node_type) == ignore_alias_list.cend())
                {
                    alias = col->_field;
                }
            } else {
                alias = col->_field;
            }

        } else {
            field = col->_field;
            alias = col->_field;
        }
    }
//    std::string ss = maskitem->_symbols;
//    if(!is_num(ss)) {
//        ss = escape_chars(dbtype,maskitem->_symbols, '\'');
//    }
    std::string ss = get_mask_symbols(dbtype,maskitem->_datatype, maskitem->_format, maskitem->_symbols, field);
    if(maskitem->_datatype == EMDATA_NONSUPPORT){
        maskitem->_condition.clear();
        //log
    }
    if(maskitem->_condition.empty()) {
        ret =  " " + ss + " ";
    } else {
        ret =  " CASE WHEN " + maskitem->_condition + " THEN "+ ss +" ELSE " + field +
                    " END ";
    }

    if (!alias.empty())
    {
        ret += " AS " + alias + " ";
    }

    return ret;
}

std::string make_one_mask_expression_no_as(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, SqlException & e){
    std::string ret;

    assert(col);
    assert(maskitem);

    std::string field;
    if(col->_node){
        field = col->_node->serialize();
    } else {
        field = col->_field;
    }

    std::string ss = get_mask_symbols(dbtype,maskitem->_datatype, maskitem->_format, maskitem->_symbols, field);
    if(maskitem->_datatype == EMDATA_NONSUPPORT){
        maskitem->_condition.clear();
        //log
    }
    if(maskitem->_condition.empty()) {
        ret =  ss;
    } else {
        ret =  " CASE WHEN " + maskitem->_condition + " THEN "+ ss +" ELSE " + field +
               " END ";
    }

    return ret;
}

bool mask_one_field(EMColumnInfo * col,  MaskItem * maskitem, EMDB_DB_TYPE dbtype, MaskOprType masktype, SqlException & e) {
    std::string node_str;
    if(MASK_2ASLABEL == masktype){
        node_str = make_one_mask_expression(col, maskitem,dbtype, e);
    } else if(MASK_DIRECT == masktype) {
        node_str = make_one_mask_expression_no_as(col, maskitem,dbtype, e);
    }

    Node *pnode = Node::makeTerminalNode(E_STRING, node_str);
    *(col->_node) = *pnode;
    delete (pnode);

    return true;
}

bool mask_fields_v3(ISelectStmt * stmt,RawExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, MaskOprType masktype, SqlException & e){
    bool bret = false;
    if (!map_condition || !root) return false;
    if(map_condition->size()==0) return false;
    switch (root->get_expr_type()) {
        case E_NULL:
        case E_INT:
        case E_DOUBLE:
        case E_BOOL:
        case E_TEMP_VARIABLE:
        case E_QUESTIONMARK:
        case E_STRING: {
//            ConstRawExpr* expr = (ConstRawExpr*)root;
        } break;
        case E_OP_NAME_FIELD: {
            RawExprBinaryRef *expr = (RawExprBinaryRef *) root;
            EMColumnInfo &field = expr->GetColInfo();

            if (field._node && field._node->GetType() == E_STRING) {//check masked.
                break;
            }

            const std::vector<const ITableItem *> &tbis = expr->GetTbItems();
            for (auto tbi: tbis) {
                std::string tbname = tbi->GetTableName();
                std::string tbalias = tbi->GetTableAliasReal();

                MaskItemMap *pmap = NULL;
                auto it = map_condition->find(tbname);
                if (it != map_condition->end()) {
                    pmap = it->second;// false;.
                }
                if (!pmap)
                    continue;

                if (!field._tb.empty()) {// check qualify.
                    if(IgnoreCaseCmp::compare(field._tb, tbname)!=0 &&IgnoreCaseCmp::compare(field._tb, tbalias)!=0 ) {
                        continue;
                    }
                }

                if(field._field.compare("*") == 0 )/*star*/
                {
                    e.SetInfo(ERR_MASK_OPR,"select all we not support now, and may cause data leakage");
                    break;
                }

                auto itfield = pmap->find(field._field);
                if (itfield != pmap->end()) {
                    MaskItem *pitem = const_cast<MaskItem *>(itfield->second);
                    //dynamic_cast<ISelectStmt*>(stmt)->GetSelectExpr();
                    const std::vector<EMColumnInfo *> &groups = stmt->GetGroupByItems();
                    if (groups.size() > 0) {// if  not have condition field in group's fields, we need convert to all mask(mask condition clear).
                        bool need_check = true;
                        if(dbtype == EMDB_DB_BIGQUERY){
                            for (auto itg: groups) {
                                if(IgnoreCaseCmp::compare(itfield->first, itg->_field) == 0){ // find mask field in group by
                                    if(itg->_tb.empty()){//if group by field no qualify(table/table alias)
                                        need_check = false;
                                    }
                                    break;
                                }
                            }
                        }
                        if(need_check){
                            for (auto itc:(pitem->_cond_fields)) {
                                bool bf = false;
                                for (auto itg: groups) {
                                    if (!itg->_tb.empty()) {//check qualify
                                        if (!CommonFun::CaseInsensitiveEquals(itg->_tb, tbalias) &&
                                            !CommonFun::CaseInsensitiveEquals(itg->_tb, tbname))
                                            continue;
                                    }
                                    if (str_case_cmp(itg->_field.c_str(), itc.c_str()) == 0) {
                                        bf = true;
                                        break;
                                    }
                                }
                                if (bf == false) {
                                    pitem->_condition.clear();
                                    break;
                                }
                            }
                        }
                        
                    }
                    bret = mask_one_field(&field, pitem, dbtype, masktype, e);
                }
            }
        }   break;
        case E_SELECT_EXPR_LIST: {
            RawExprMultiOp *expr = (RawExprMultiOp *) root;
            for (int i = 0; i < expr->get_expr_len(); ++i) {
                mask_fields_v3(stmt, expr->get_op_expr(i), map_condition,dbtype, masktype,e);
            }
        }
            break;
        case E_ALIAS: {//aaa AS bbb
            AsAliasExpr *as_ = (AsAliasExpr *) root;
            mask_fields_v3(stmt, as_->GetExpr(), map_condition,dbtype, masktype,e);
        } break;
        case E_STAR: {
            //return mask_star_field_bigquery(stmt, root,map_condition,e);
            e.SetInfo(ERR_MASK_OPR,"select all we not support now, and may cause data leakage");
        } break;
        case E_ASTERISK_EXPR:{
            AsteriskExpr * asterisk = (AsteriskExpr*) root;
            if(dbtype == EMDB_DB_BIGQUERY){
                return mask_star_field_bigquery(stmt, asterisk,map_condition,dbtype,e);
            } else {
                return mask_star_field_20210618(stmt, asterisk,map_condition,dbtype,e);
            }

        }
        case E_DIRECT_SELECT:
        case E_SELECT: {
//            RawExprScalarSubquery* expr = (RawExprScalarSubquery*)root;
        } break;
        case E_OP_ADD:
        case E_OP_MINUS:
        case E_OP_MUL:
        case E_OP_DIV:
        case E_OP_CNN: {
            RawExprBinaryOp* expr = (RawExprBinaryOp*)root;
            
            bret = mask_fields_v3(stmt, expr->get_first_op_expr(), map_condition, dbtype, masktype, e);
            bret = mask_fields_v3(stmt, expr->get_second_op_expr(), map_condition, dbtype, masktype, e);
        } break;
        case E_OP_REM:
        case E_OP_POW:
        case E_OP_MOD:
        case E_OP_LE:
        case E_OP_LT:
        case E_OP_EQ:
        case E_OP_GE:
        case E_OP_GT:
        case E_OP_NE:
        case E_OP_LIKE:
        case E_OP_NOT_LIKE:
        case E_OP_AND:
        case E_OP_OR:
        case E_OP_IS:
        case E_OP_IS_NOT:
        case E_OP_IN:
        case E_OP_NOT_IN:
        case E_OP_ASS:
        case E_OP_ASS_ADD:
        case E_OP_ASS_MINUS:
        case E_OP_ASS_MUL:
        case E_OP_ASS_DIV:
        case E_OP_ASS_REM:
        case E_OP_ASS_BIT_AND:
        case E_OP_ASS_BIT_OR:
        case E_OP_ASS_BIT_XOR:
        case E_OP_COLLATE: {
//            RawExprBinaryOp* expr = (RawExprBinaryOp*)root;
//            bret = mask_fields_v3(expr->get_first_op_expr(), map_condition);
//            bret = mask_fields_v3(expr->get_second_op_expr(), map_condition);
        }
            break;
        case E_OP_NOT:
        case E_OP_EXISTS:
        case E_OP_POS:
        case E_OP_NEG: {
//            RawExprUnaryOp* expr = (RawExprUnaryOp*)root;
//            bret = mask_fields_v3(expr->get_op_expr(), map_condition);
        }
            break;
        case E_OP_BTW:
        case E_OP_NOT_BTW: {
//            RawExprTripleOp* expr = (RawExprTripleOp*)root;
//            bret = mask_fields_v3(expr->get_first_op_expr(), map_condition);
//            bret = mask_fields_v3(expr->get_second_op_expr(), map_condition);
//            bret = mask_fields_v3(expr->get_third_op_expr(), map_condition);
        }
            break;
        case E_CASE: {
//            RawExprCaseOp* expr = (RawExprCaseOp*)root;
//            bret = mask_fields_v3(expr->get_arg_op_expr(), map_condition);
//            bret = mask_fields_v3(expr->get_default_expr(), map_condition);
//            for(int i = 0 ; i < expr->get_when_len(); ++i){
//                bret = mask_fields_v3(expr->get_when_op_expr(i), map_condition);
//            }
//            for(int i = 0 ; i < expr->get_then_len(); ++i){
//                bret = mask_fields_v3(expr->get_then_op_expr(i), map_condition);
//            }
        }
            break;
        case E_FUN_CALL: {
            const static std::vector<std::string> mask_parameter_func_list {
                "LEFT", "RIGHT", "REPLACE", "SUBSTRING", "EXTRACT", "LOWER", "UPPER", "LEN", "LENGTH", "TRIM", "CONVERT", "TO_CHAR",
                "CAST", "UNPIVOT", "RTRIM", "LTRIM",
                "ABS", "ROUND", "CEIL", "CEILING", "FLOOR", "LOG", "LOG10", "POWER", "SQRT",
                "datepart"
            };
            RawExprSysFun* expr = (RawExprSysFun*)root;
            const auto func_name = expr->get_func_name();

            for (const auto& item : mask_parameter_func_list)
            {
                if (IgnoreCaseCmp::compare(func_name, item) == 0)
                {
                    for(int i = 0 ; i < expr->get_func_len(); ++i){
                        bret = mask_fields_v3(stmt, expr->get_param_expr(i), map_condition, dbtype, MASK_DIRECT, e);
                    }
                    break;
                }
            }
        }
            break;
        case E_ARRAY_DEF:{
            ArrayDefExpr* expr = (ArrayDefExpr*)root;
            size_t len = expr->get_array_len();
            for(size_t i = 0 ; i < len;++i){
                bret = mask_fields_v3(stmt,  expr->get_param_expr(i), map_condition, dbtype, MASK_DIRECT,e);
            }
        } break;
        case E_STRUCT_DEF:{
            StructDefExpr* expr = (StructDefExpr*)root;
            size_t len = expr->get_array_len();
            MaskOprType struct_mask_type = MASK_2ASLABEL;
            if(!expr->_hav_struct_name || expr->_hav_datatype_def){ /* (a, b)   OR  STRUCT<...>(a,b) */
                struct_mask_type = MASK_DIRECT;
            }
            for(size_t i = 0 ; i < len; ++i){
                bret = mask_fields_v3(stmt,  expr->get_param_expr(i), map_condition, dbtype, struct_mask_type, e);
            }
        } break;
        default:{

        }break;
    }
    return bret;
}





/*
void ParserTable(Node* pnode, std::vector<EMTableInfo*> & _tables){
    //locate from  table  name and node
    if (pnode == NULL) return;
    switch (pnode->nodeType_) {
        case E_FROM_CLAUSE: {
            ParserTable(pnode->getChild(0), _tables);
        } break;
        case E_FROM_LIST: {
            ParserTable(pnode->getChild(0), _tables); // E_FROM_LIST
            //table_reference // table_primary(E_ALIAS)/joined_table(FromItemType::E_JOINED_TABLE)
            ParserTable(pnode->getChild(1), _tables);
        }break;
        case FromItemType::E_JOINED_TABLE_WITH_PARENS: {
            ParserTable(pnode->getChild(0), _tables);
        }break;
        case E_ALIAS: {
            Node * pnode_tb = pnode->getChild(0);
            Node * pnode_alias = pnode->getChild(1);
            if(pnode_tb && pnode_tb->GetType() == E_TABLE_IDENT) {
                EMTableInfo *info = new EMTableInfo(pnode);
                parser_bigquery_tbname(pnode_tb,info->_db, info->_schema, info->_tb);
                if(pnode_alias && pnode_alias->GetType() == E_IDENTIFIER){
                    info->_alias = pnode_alias->terminalToken_.str;
                }
                _tables.push_back(info);
            }
        }break;
        case E_TABLE_IDENT: {
            EMTableInfo *info = new EMTableInfo(pnode);
            parser_bigquery_tbname(pnode,info->_db, info->_schema, info->_tb);
            _tables.push_back(info);
        }break;
        case E_STRING: {
        }break;
        case E_IDENTIFIER: {
        }break;
        case FromItemType::E_JOINED_TABLE:
        {
            //table_reference // table_primary(E_ALIAS)/joined_table(FromItemType::E_JOINED_TABLE)
            ParserTable(pnode->getChild(1), _tables); // join table_reference
            ParserTable(pnode->getChild(2), _tables); //join table_reference
        }break;
        default:
            break;
    }
}
*/
/*
void ParserColumns(Node* pnode, std::vector<EMColumnInfo*>  & _fields) {
    switch (pnode->nodeType_) {
        case E_SELECT_EXPR_LIST: {
            ParserColumns( pnode->getChild(0), _fields);
            ParserColumns( pnode->getChild(1), _fields);
        } break;
        case E_PROJECT_STRING: {
            ParserColumns( pnode->getChild(0), _fields); // E_STAR / ...
        } break;
        case E_OP_NAME_FIELD: {//name_r....*
            EMColumnInfo *info = new EMColumnInfo(pnode);
            parser_bigquery_field( pnode, info->_db, info->_schema, info->_tb, info->_field);
            info->_alias = false;

            if(!info->_field.empty()) {
                _fields.push_back(info);
            }else {
                delete(info);
            }
        } break;
        case E_ALIAS: {//aaa AS bbb
            EMColumnInfo *info = new EMColumnInfo(pnode);
            parser_alias( pnode, *info);
            info->_alias = true;

            if(!info->_field.empty()) {
                _fields.push_back(info);
            } else {
                delete(info);
            }
        } break;
        case E_STAR: {
//            EMColumnInfo *info = new EMColumnInfo(pnode);
//            _fields.push_back(info);
//            _isstar = true;
            return;
        }
        default: break;
    }
    return ;
}
*/
/*
bool ParseGroupBy(Node * pnode, std::vector<EMColumnInfo*> & vec){
    switch (pnode->nodeType_) {
        case E_GROUP_BY: {
            //0
            ParseGroupBy(pnode->getChild(0),  vec);
        } break;
        case E_EXPR_LIST: {
            //0
            ParseGroupBy(pnode->getChild(0),  vec);
            //1
            ParseGroupBy(pnode->getChild(1),  vec);
        } break;
        case E_OP_NAME_FIELD: {
            EMColumnInfo *info = new EMColumnInfo(pnode);
            parser_bigquery_field( pnode, info->_db, info->_schema, info->_tb, info->_field);
            info->_alias = false;
            if(!info->_field.empty()) {
                vec.push_back(info);
            }else {
                delete(info);
            }
        } break;
        case E_EXPR_LIST_WITH_PARENS: {
            //0
            ParseGroupBy(pnode->getChild(0),  vec);
        } break;
        default:
            break;
    }
    return true;
}
*/