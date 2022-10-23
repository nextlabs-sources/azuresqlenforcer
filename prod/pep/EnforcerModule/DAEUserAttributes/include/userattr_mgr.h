#ifndef USER_ATTR_MGR_H
#define USER_ATTR_MGR_H
#include "userattr.h"
#include <string>
#include <map>
#include <thread>
#include <shared_mutex>

class IQueryUserAttribute {
public:
    virtual ~IQueryUserAttribute() {}
    virtual bool QueryUserAttribute(UserAttr *pUser) = 0;
};
struct SqlException;
class UserAttrMgr
{
    public:
    UserAttrMgr(int mode, const char *param1, const char *param2, const char *param3, const char * p_separator, int interval);

    public:
    IUserAttr* GetUserAttr(const char* szUserName, SqlException* pExc);

    
    protected:
    IUserAttr* QueryAttrForNewUser(const char* szUserName, SqlException* pExc);
    bool QueryUserAttribute(UserAttr* pUser);

    void ThreadSync();
    void Sync();

    private:
    std::map<std::string, IUserAttr*> m_dicUser;
    std::shared_mutex m_mutexDicUser;

    IQueryUserAttribute *m_impl;
    std::string _separator;
    int m_nIntervalSecond;
};



extern UserAttrMgr* theUserAttrMgr;

#endif 