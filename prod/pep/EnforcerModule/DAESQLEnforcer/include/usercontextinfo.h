#ifndef USER_CONTEXT_INFO_H
#define USER_CONTEXT_INFO_H
#include <string>
#include "Interface.h" //interface for sql parse.
#include "iuserattr.h"
#include "emdb_handle.h"
#include "commfun.h"
#include "EMMaskDef.h"

class CEnforcerMgr;
struct SqlException;


class UserContextInfo :public CEMDBHandle
{
public:
   UserContextInfo(const char* szUserName, const char* szPwd);
   ~UserContextInfo();

   bool InitUserAttr(SqlException * pExc);

   virtual bool IsType(uint64_t value){
     return m_magicNumber == value;
   }

public:
    IUserAttr* UserAttribute() { return m_pUserAttr; }

    std::string GetDBServer() { return m_strDBServer; }
    void SetDBServer(const char* szServer) ;
    
    std::string GetCurrentDB() { return m_strCurrentDB; }
    void SetCurrentDB(const char* szDB);

    std::string GetSchema() { return m_strSchema; }
    void SetSchema(const char* szSchema);

    std::string GetClientAppName() { return m_client_app_name; }
    void SetClientAppName(const char* szClientAppName);

    std::string GetClientHostName() { return m_client_host_name; }
    void SetClientHostName(const char* szClientHostName);

    std::string GetClientIp() { return m_client_ip; }
    void SetClientIp(const char* szClientIp);

    std::string GetUsername(){return m_strUserName;}

    void SetEMDBType(EMDB_DB_TYPE type);
    EMDB_DB_TYPE GetEMDBType(){ return m_emdbtype;}

    bool GetTableMetadata(const std::string& dbname, const std::string& schema, const std::string& table, MetadataVec & vec) ;

private:
  std::string m_strUserName;
  std::string m_strPwd;
  IUserAttr* m_pUserAttr;

  std::string m_strDBServer;
  std::string m_strCurrentDB;
  std::string m_strSchema; //for sql server.

  std::string m_client_app_name;
  std::string m_client_host_name;
  std::string m_client_ip;

  EMDB_DB_TYPE m_emdbtype;

  QueryMetadataFunc _meta_func = nullptr;
  std::map<std::string,MetadataVec*> _map_tb2matadata;



  friend class CEnforcerMgr;
};

#endif 