//
// Created by jeff on 2021/3/18.
//

#ifndef EMDB_SQLENFORCER_SP_H
#define EMDB_SQLENFORCER_SP_H
#ifdef WINDOWS_APP_DEMO


#include <string>
#include <vector>
#include <map>
struct EMDB_SP_TABLES{
    std::string _db;
    std::string _schema;
    std::string _table;
    std::string _alias;
    std::string _paramname;
    std::string _condition;//(param real value)
};

struct EMDB_SP_INFO{
    std::string _call_str;
    std::string _qualifier;
    std::string _ori_sp;
    std::string _new_sp;
    std::map<unsigned int,EMDB_SP_TABLES> _params;

};

bool EMDBSP_parser_sp_cmd(const std::string & sql, EMDB_SP_INFO & info);
bool EMDBSP_carete_indirect_sql(std::map<unsigned int,std::string> & sqls, const EMDB_SP_INFO & info);
bool EMDBSP_load_new_sp(EMDB_SP_INFO & info);
bool EMDBSP_resolve_new_sp(const std::map<unsigned int,std::string> & sqls, EMDB_SP_INFO & info);
std::string  EMDBSP_output_new_sp(const EMDB_SP_INFO & info);

#endif







#endif //EMDB_SQLENFORCER_SP_H
