//
// Created by jeff on 2020/3/27.
//

#ifndef HOOK_EMS4HEXCEPTION_H
#define HOOK_EMS4HEXCEPTION_H

#include <string>

#define S4H_SUCCESS 0
#define S4H_INVALID_CONNECTION -1
#define S4H_INVALID_TABLE_NAME -2
#define S4H_BIND_PARAM_ERROR -3
#define S4H_SQL_PARSE_FAIL -4
#define S4H_SQL_QUERY_PC_FAIL -5

#define POLICY_ENGINE_SUCCESS 0
#define POLICY_ENGINE_FAIL -100
#define POLICY_ENGINE_TYPE_ERROR -101
#define POLICY_ENGINE_MODULE_NOT_INIT -102
#define POLICY_ENGINE_INVALID_HANDLE -103
#define POLICY_ENGINE_CCCONNECT_ERROR -104

class S4HException {
public:
    S4HException():_code(0){}
    int _code;
    std::string _detail;
    void SetError(int code, const std::string & detail) {
        _code = code;
        _detail = detail;
    }
};


#endif //HOOK_EMS4HEXCEPTION_H
