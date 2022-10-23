#include "usercontextinfo.h"
#include "userattrwrapper.h"
#include "policymgrwrapper.h"
#include "DAELog.h"
#include "table_metadata_cache_manager.h"
//#define DAEFORORACLE_20220111

UserContextInfo::UserContextInfo(const char* szUserName, const char* szPwd):
CEMDBHandle(HANDLE_USERCONTEXTINFO),
m_strUserName(szUserName),
m_strPwd(szPwd),
m_pUserAttr(nullptr)
{
   m_emdbtype  = EMDB_DB_TYPE::EMDB_DB_UNKNOW;
   //EncryptPwd();   
}

UserContextInfo::~UserContextInfo() { /*delete(m_pUserAttr); //m_pUserAttr managed by userattribute module*/
    for (auto it : _map_tb2matadata) {
        delete(it.second);
    }
    _map_tb2matadata.clear();
}

bool UserContextInfo::InitUserAttr(SqlException * pExc)
{
    if(UserAttrWrapper::Instance()->GetUserAttr){
        m_pUserAttr = UserAttrWrapper::Instance()->GetUserAttr(m_strUserName.c_str(), pExc);
    }

    return (nullptr==m_pUserAttr)?false:true;
}


void UserContextInfo::SetDBServer(const char* szServer)
{
    if (!CommonFun::CaseInsensitiveEquals(m_strDBServer, szServer))
    {
        m_strDBServer = szServer;
        theLog->WriteLog(log_info, "set current server name:%s", m_strDBServer.c_str() );
    }
}

void UserContextInfo::SetCurrentDB(const char* szDB)
{
    if (!CommonFun::CaseInsensitiveEquals(m_strCurrentDB, szDB))
    {
        m_strCurrentDB = szDB;
        theLog->WriteLog(log_info, "set current database:%s",  m_strCurrentDB.c_str() );
    }
}

void UserContextInfo::SetSchema(const char* szSchema)
{
    if (!CommonFun::CaseInsensitiveEquals(m_strSchema, szSchema))
    {
        m_strSchema = szSchema;
        theLog->WriteLog(log_info, "set current schema :%s",  m_strSchema.c_str() );
    }
}

void UserContextInfo::SetClientAppName(const char* szClientAppName)
{
    if (!CommonFun::CaseInsensitiveEquals(m_client_app_name, szClientAppName))
    {
        m_client_app_name = szClientAppName;
        theLog->WriteLog(log_info, "set Current app name: %s", m_client_app_name.c_str());
    }
}

void UserContextInfo::SetClientHostName(const char* szClientHostName)
{
    if (!CommonFun::CaseInsensitiveEquals(m_client_host_name, szClientHostName))
    {
        m_client_host_name = szClientHostName;
        theLog->WriteLog(log_info, "set current host name: %s", m_client_host_name.c_str());
    }
}

void UserContextInfo::SetClientIp(const char* szClientIp)
{
    if (!CommonFun::CaseInsensitiveEquals(m_client_ip, szClientIp))
    {
        m_client_ip = szClientIp;
        theLog->WriteLog(log_info, "set current client ip: %s", m_client_ip.c_str());
    }
}

void UserContextInfo::SetEMDBType(EMDB_DB_TYPE type){
    if (m_emdbtype != type)
    {
        m_emdbtype = type;
        theLog->WriteLog(log_info, "set current dbtype :%d (1 MSSQL,2 ORACLE,3 HANA)",  (int)type);
    }
}

bool UserContextInfo::GetTableMetadata(const std::string& dbname, const std::string& schema, const std::string& table, MetadataVec& vec) {
    const auto table_metadata = TableMetadataCacheManager::Instance().GetTableMetadata(dbname, schema, table);

    for (const auto& column : table_metadata.GetColumns())
    {
        Metadata data;
        data._col = column.GetColumnName();
        data._type = column.GetColumnType();
        vec.push_back(data);
    }

    {//---log
        std::string log = "table:"+ dbname + "." + schema + "." + table + " metadata:";
        for(auto & it:vec){
            log+=it._col;
            log+="  ";
        }
        theLog->WriteLog(log_info, log.c_str());
    }

    return vec.size()>0;
}
