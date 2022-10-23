//
// Created by jeff on 2021/4/22.
//


#include "sqlenforcer_create.h"
#ifdef WINDOWS_APP_DEMO
#include "commfun.h"
#include <set>
#include <string.h>
#include <IQueryCloudAZ.h>
#include "QueryCAZPolicyOpr.h"
#include "usercontextinfo.h"
#include <iuserattr.h>
#include "DAELog.h"

namespace DAE {

    bool is_digit(char c) { return '0' <= c && c <= '9'; }

    bool is_underline(char c) { return '_' == c; }

    bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }

    bool is_write(char c) {
        return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f';
    }

    bool is_op(char c) {
        return c == '>' || c == '<' || c == '=' || c == '!';
    }

    bool is_paren(char c) {
        return c == '(' || c == ')' || c == '[' || c == ']';
    }


    struct IgnoreCaseCmpare : public std::binary_function<std::string, std::string, bool> {
        bool operator()(const std::string &left, const std::string &right) const {
#ifndef WIN32
            return strcasecmp(left.c_str(), right.c_str()) < 0;
#else
            return stricmp(left.c_str(), right.c_str()) < 0;
#endif
        }

        static int compare(const std::string &left, const std::string &right) {
#ifndef WIN32
            return strcasecmp(left.c_str(), right.c_str());
#else
            return stricmp(left.c_str(), right.c_str());
#endif
        }

    };

    static std::set<std::string, IgnoreCaseCmpare> g_reserved_begin = {
            "CREATE", "TABLE", "DROP", "IF", "EXISTS", "COLUMN", "ROW"
    };

    std::string parser_identifier(const std::string &src, unsigned int &p, char c) {
        bool bquote = (c == '`' || c == '"');
        if (bquote) {
            p++;
        }
        size_t len = src.length();
        std::string ret;
        while (p < len) {
            if (!bquote) { //
                if (is_write(src[p]) || is_op(src[p]) || is_paren(src[p]) || src[p] == '.') {
                    break;
                } else if (is_underline(src[p]) || is_letter(src[p]) || src[p] == '-' || is_digit(src[p])) {

                } else {
                    return "";

                }
            } else {
                if (src[p] == c ) {
                    p++;
                    break;
                }
            }
            ret += src[p];
            p++;
        }
        return ret;
    }
//std::string parser_string(const std::string &src,unsigned int & p, char c){
//    if(src[p] != c)
//        return "";
//    p++;
//    size_t len = src.length();
//    std::string ret;
//    bool bend = false;
//    while(p < len){
//        if(src[p] == c && src[p-1] != c){
//            p++;
//            bend = true;
//            break;
//        }
//        ret += src[p];
//        p++;
//    }
//    if(!bend){
//        return "";
//    }
//    return ret;
//}

//std::string parser_num(const std::string &src,unsigned int & p, char c){
//    size_t len = src.length();
//    std::string ret;
//    while(p < len){
//        if(is_write_(src[p]) || is_op_(src[p]) || src[p] == ','|| src[p] == ';')
//            break;
//        switch (src[p]){
//            case '1':
//            case '2':
//            case '3':
//            case '4':
//            case '5':
//            case '6':
//            case '7':
//            case '8':
//            case '9':
//            case '0':
//            case '-':
//            case '.':
//            case 'e':{
//                ret += src[p];
//            }break;
//            default:{
//                //error;
//                return "";
//            }break;
//        }
//        p++;
//    }
//    return ret;
//}

    void parser_table(const std::string &sql, unsigned int &pos, CrtDrpTableInfo &tbinfo) {
        size_t ilen = sql.length();
        std::string val1;
        std::string val2;
        std::string val3;
        bool bnext = true;
        bool bbreak = false;
        while (pos < ilen) {
            char c = sql[pos];
            switch (c) {
                case '\n':
                case '\t':
                case '\r':
                case '\f':
                case ' ': {
                    pos++;
                } break;
                case '.': {
                    pos++;
                    bnext = true;
                }break;
                case '"': {
                    if (bnext) {
                        if (val1.empty()) {
                            val1 = parser_identifier(sql, pos, '"');

                        } else if (val2.empty()) {
                            val2 = parser_identifier(sql, pos, '"');
                        } else if (val3.empty()) {
                            val3 = parser_identifier(sql, pos, '"');
                        }
                        bnext = false;
                    } else {
                        bbreak = true;
                    }

                }
                    break;
                default: {

                    if (bnext) {
                        if (val1.empty()) {
                            val1 = parser_identifier(sql, pos, ' ');
                        } else if (val2.empty()) {
                            val2 = parser_identifier(sql, pos, ' ');
                        } else if (val3.empty()) {
                            val3 = parser_identifier(sql, pos, ' ');
                        }
                        bnext = false;
                    } else {
                        bbreak = true;
                    }
                } break;

            }
            if (bbreak) {
                break;
            }

        }
        if (!val3.empty()) {
            tbinfo._table = val3;
            tbinfo._schema = val2;
            tbinfo._db = val1;
        } else if (!val2.empty()) {
            tbinfo._table = val2;
            tbinfo._schema = val1;
        } else if (!val1.empty()) {
            tbinfo._table = val1;
        }
        return;

    }

    int is_create_or_drop_stmt(const std::string &sql, CrtDrpTableInfo &tbinfo) {
        size_t ilen = sql.length();
        unsigned int pos = 0;
        bool bret = true;
        // CREATE TABLE DROP TABLE IF EXISTS
        while (pos < ilen) {
            char c = sql[pos];
            switch (c) {
                case '\n':
                case '\t':
                case '\r':
                case '\f':
                case ' ': {
                    pos++;
                }
                    break;
                case '/': {
                    pos++;
                    if (sql[pos] == '*') {
                        pos++;
                        while (pos + 1 < ilen) {
                            if (sql[pos] == '*' && sql[pos + 1] == '/') {
                                pos+=2;
                                break;

                            }
                            pos++;
                        }
                    }
                }
                    break;
//            case '"':{
//                parser_table(sql, pos, tbinfo);
//                return  true;
//            } break;
                default: {
                    //int istart = pos;
                    std::string val = parser_identifier(sql, pos, ' ');
                    if (bret) {// first identifier need is CREATE / DROP
                        if (CommonFun::StrCaseCmp(val.c_str(), "CREATE") == 0) {
                            //
                            tbinfo._iscreate = true;
                            bret = false;
                        } else if (CommonFun::StrCaseCmp(val.c_str(), "DROP") == 0) {
                            //
                            tbinfo._iscreate = false;
                            bret = false;
                        } else {
                            return false;
                        }

                    }
                    if (g_reserved_begin.find(val) != g_reserved_begin.end()) {
                        if (CommonFun::StrCaseCmp(val.c_str(), "TABLE") == 0 ||
                            CommonFun::StrCaseCmp(val.c_str(), "EXISTS") == 0) {
                            //
                            parser_table(sql, pos, tbinfo);
                            if (g_reserved_begin.find(tbinfo._table) != g_reserved_begin.end()){
                                tbinfo._table.clear();
                                break;
                            }
                            return true;
                        }
                    } else {
                        return false;
                    }
                }

            }
        }
        return false;
    }


    bool deny_create_or_drop(CrtDrpTableInfo & tbinfo, UserContextInfo* userinfo, SqlException * pExc){
        const IPolicyRequest * reqs = nullptr;
        std::string  user = userinfo->GetUsername();

        {
            RequestInfo reqinfo;
            reqinfo._action = tbinfo._iscreate?"CREATE":"DROP";

            //set schema,table name
            std::string server = userinfo->GetDBServer();
            std::string database = userinfo->GetCurrentDB();
            std::string schema = userinfo->GetSchema();
            std::string table ;
            if(!tbinfo._db.empty()){
                database = tbinfo._db;
            }
            if(!tbinfo._schema.empty()){
                schema = tbinfo._schema;
            }
            if(!tbinfo._table.empty()){
                table = tbinfo._table;
            }
            //ResourceAttrValue * attr = NULL;
            {
                //attr = new ResourceAttrValue(server, XACML_string);
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(server, XACML_string);

                reqinfo._rescs2["server"] = ptrb;
            }
            {
               // attr = new ResourceAttrValue(database, XACML_string);
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(database, XACML_string);
                reqinfo._rescs2["database"] = ptrb;
            }
            {
               // attr = new ResourceAttrValue(schema, XACML_string);
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(schema, XACML_string);
                reqinfo._rescs2["schema"] = ptrb;
            }
            {
               // attr = new ResourceAttrValue(table, XACML_string);
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(table, XACML_string);
                reqinfo._rescs2["table"] = ptrb;
            }

            IUserAttr * uaser_attr = userinfo->UserAttribute();
            uaser_attr->CopyDictionary2(reqinfo._users2);
            if(user.empty() || user.compare("null") == 0){
                auto it = reqinfo._users2.find("id");
                if(it != reqinfo._users2.end()){
                    user = it->second.get()->strValue;
                } else {
                    user = "user";
                }
            }
            reqs = create_request(reqinfo, user);
        }
        if(!reqs){
            pExc->SetInfo(ERR_POLICY, "create policy request failed.");
            theLog->WriteLog(log_error, "create policy request  failed");
            return true;
        }
        IPolicyResult* result = NULL;
        QueryStatus  status = single_query_pc(reqs, &result);
/*
        auto st = system_clock::now();
        auto ed = system_clock::now();
        auto duration = duration_cast<microseconds>(ed-st);
        double  timed = double (duration.count())*microseconds::period::num / microseconds::period::den;
        theLog->WriteLog(log_error, "query_single_time:%lf",timed);
*/
        free_pc_request(const_cast<IPolicyRequest*>(reqs));

        if (QS_S_OK != status ) {
            pExc->SetInfo(ERR_POLICY, "single_query_pc failed.");
            theLog->WriteLog(log_error, "single_query_pc failed");
            return true;
        }
        bool deny = result->GetEnforcement() == PolicyEnforcement::Deny;
        free_pc_result(result);

        return deny;
    }

}


#endif