//
// Created by jeff on 2021/4/22.
//

#ifndef EMDB_SQLENFORCER_CREATE_H
#define EMDB_SQLENFORCER_CREATE_H
#ifdef WINDOWS_APP_DEMO
#include <string>
class UserContextInfo;
class  SqlException;
namespace DAE{
    struct CrtDrpTableInfo{
        std::string _table;
        std::string _schema;
        std::string _db;
        bool _iscreate = false;
    };

    int is_create_or_drop_stmt(const std::string& sql,CrtDrpTableInfo & tbinfo);
    bool deny_create_or_drop(CrtDrpTableInfo & tbinfo, UserContextInfo* userinfo, SqlException * pExc);
}

#endif


#endif //EMDB_SQLENFORCER_CREATE_H
