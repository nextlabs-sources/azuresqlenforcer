#ifndef IODBC_EXCEPTION_H
#define IODBC_EXCEPTION_H

#include <stdio.h>
#include <string>


enum SQLERR_CODE{
    ERR_NULL = 0,
    ERR_DENY = 42000,
    ERR_CONFIG = 42001,//fatal error
    ERR_LIBRARY = 42002,//fatal error
    ERR_POLICY = 42003,//warnning
    ERR_INITCONTEXT = 42004,//fatal error
    ERR_USERINFO = 42005,//warnning
    ERR_PARSE, //warnning
    ERR_DENY_SELECT,
    ERR_POLICY_NOMATCH,
    ERR_POLICY_PARSER,
    ERR_MASK_OPR,
    ERR_FILTER_OPR,
    ERR_DBTYPE
};

const std::string detail_err_lib_sqlenfoercer = "Load library sqlenfoercer.dll failed";
const std::string detail_err_lib_odbc= "Load ODBCAPI failed";
const std::string detail_err_lib_sql2003 = "Load library sql2003.dll failed";
const std::string detail_err_lib_userattribute = "Load library userattribute.dll failed";
const std::string detail_err_lib_policymgr = "Load library policymgr.dll failed";
const std::string detail_err_ploicyinit = "Policy's initialization of failed";
const std::string detail_err_context_init1 = "Can't get the server name";
const std::string detail_err_context_init2 = "Can't get the driver Type";
const std::string detail_err_context_init3 = "Can't get the database name";
const std::string detail_err_context_init4 = "Can't determines the DB Type";
const std::string detail_err_context_init5 = "QueryODBCSessionInfo allocate SQLHSTMT failed";
const std::string detail_err_context_init6 = "QueryODBCSessionInfo  SQLExecDirect failed";

const std::string detail_err_userinfo = "Can't Read UserInfo";
const std::string detail_err_evaluation = "Denied by policy, value of 'effectType' is 'deny'";
const std::string detail_err_parse1 = "Sql parser module didn't well";
const std::string detail_err_parse2 = "Can't get user context info";
const std::string detail_err_parse3 = "Can't get sql tree";
const std::string detail_err_parse4 = "Sql script error";

struct SqlException{
    ///break or no according to config.ini "DAE_ERROR.break"
     bool IsBreak(){
         bool bBreak = false;
         switch (code){
             case ERR_DENY: // deny update/delete/insert
             case ERR_CONFIG:
             case ERR_LIBRARY:
             case ERR_INITCONTEXT: {
                 bBreak = true;
             } break;
             default:
                 break;
         }
        return bBreak;
    }
    bool IsDeny(bool except_deny){
        bool bdeny = false;
        switch (code){
            case ERR_DENY: // deny update/delete/insert
            case ERR_CONFIG:
            case ERR_LIBRARY:
            case ERR_INITCONTEXT:{
                bdeny = true;
            } break;
            case ERR_POLICY:
            case ERR_USERINFO:{
                if(except_deny){
                    bdeny = true;
                }
            } break;
            default:
                break;
        }
        return bdeny;
     }
    SqlException():code(ERR_NULL){
       
    }
    void SetInfo(SQLERR_CODE Code, const std::string & strDetil){
        code = Code;
        cdetail = strDetil;
    }
    SQLERR_CODE code;
    std::string cdetail;
};










#endif