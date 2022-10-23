#ifndef EMDB_DB_TYPE_H
#define EMDB_DB_TYPE_H

enum EMDB_DB_TYPE
{
    EMDB_DB_UNKNOW=0,
    EMDB_DB_SQLSERVER,
    EMDB_DB_ORACLE,
    EMDB_DB_HANA,
    EMDB_DB_MYSQL,
    EMDB_DB_BIGQUERY,
    EMDB_DB_DB2,
    EMDB_DB_REDSHIFT
};

EMDB_DB_TYPE get_dbtype_by_drivername(const std::string& strDriverName);

#endif 