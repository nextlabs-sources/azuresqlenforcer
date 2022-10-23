#include "userattr_mgr.h"
#include "log.h"
#include <iostream>
#include "commfun.h"
#include "nanodbc.h"
#include "SqlException.h"
#ifdef WIN32
#include "ImplQueryUserAttributeAD.h"
#endif


class ImplQueryUserAttributeIni : public IQueryUserAttribute {
public:
    virtual bool QueryUserAttribute(UserAttr *pUser) override;
};

class ImplQueryUserAttributeDB : public IQueryUserAttribute {
public:
    ImplQueryUserAttributeDB(const char *cnn_str, const char *user_tbl);
    ~ImplQueryUserAttributeDB();
    virtual bool QueryUserAttribute(UserAttr *pUser) override;
private:
    bool Connect();
private:
    nanodbc::connection *m_cnn;
    nanodbc::statement  *m_stmt;
    nanodbc::string m_cnn_str;
    nanodbc::string m_user_tbl;

};


class ImplQueryUserAttributeDB_JDBC : public IQueryUserAttribute {
public:
    ImplQueryUserAttributeDB_JDBC(){}
    ~ImplQueryUserAttributeDB_JDBC(){}
    virtual bool QueryUserAttribute(UserAttr *pUser) override;

};
bool ImplQueryUserAttributeDB_JDBC::QueryUserAttribute(UserAttr *pUser){
    return true;
}



UserAttrMgr *theUserAttrMgr = NULL;

UserAttrMgr::UserAttrMgr(int mode, const char *param1, const char *param2, const char *param3, const char * p_separator, int interval) {
    if (interval < 10)
        interval = 10;
    m_nIntervalSecond = interval;
    _separator = p_separator;
    switch (mode) {
        case 0 : {
            m_impl = new ImplQueryUserAttributeIni;
        } break;
        case 1 : {
            if(CommonFun::StrCaseCmp("jdbc", param3) == 0) {
                m_impl = new ImplQueryUserAttributeDB_JDBC();
            } else {
                m_impl = new ImplQueryUserAttributeDB(param1, param2);
            }
        } break;
        case 2 : {
#ifdef WIN32
            //m_impl = make_ad_impl(param1, param2, param3);
            m_impl = make_ad_impl2(param2);
#else
            /* linux not support AD */
            m_impl = new ImplQueryUserAttributeIni;
#endif
        } break;
        default: {
            m_impl = new ImplQueryUserAttributeIni;
        } break;
    }
    if(CommonFun::StrCaseCmp("jdbc", param3) == 0) {
    } else {
        std::thread t(&UserAttrMgr::ThreadSync, this); //
        t.detach();
    }


}

IUserAttr *UserAttrMgr::GetUserAttr(const char *szUserName, SqlException* pExc) {
    IUserAttr *pUserAttr = NULL;

    //get from cache
    {
        std::shared_lock<std::shared_mutex> lockReadUser(m_mutexDicUser);

        auto itUser = m_dicUser.find(szUserName);
        if (itUser != m_dicUser.end())
            pUserAttr = itUser->second;
    }

    //if not cached, query it
    if (pUserAttr == NULL) {
        //create a new user
        pUserAttr = QueryAttrForNewUser(szUserName, pExc);
    }

    return pUserAttr;
}

IUserAttr *UserAttrMgr::QueryAttrForNewUser(const char *szUserName, SqlException* pExc) {
    std::unique_lock<std::shared_mutex> lockWriteUser(m_mutexDicUser);

    //first we query again, since other thread may added alread.
    std::string username(szUserName);
    if(username.empty()){
        return NULL;
    }
    auto itUser = m_dicUser.find(username);
    if (itUser != m_dicUser.end())
        return itUser->second;
    else {
        UserAttr *pNewUser = new UserAttr(szUserName);
        pNewUser->SetSeparator(_separator);
        m_dicUser[username] = pNewUser;

        bool bSuccess = QueryUserAttribute(pNewUser);
        if(!bSuccess) {
            //pExc->SetInfo(ERR_USERINFO, detail_err_userinfo);
            Log::WriteLog(log_error, "QueryUserAttribute %s", detail_err_userinfo.c_str());
        }

        return pNewUser;
    }
}

bool UserAttrMgr::QueryUserAttribute(UserAttr *pUser) {
    return m_impl->QueryUserAttribute(pUser);
}


void UserAttrMgr::ThreadSync() {
    Log::WriteLog(log_info, "UserAttrMgr::ThreadSync start");
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(m_nIntervalSecond));
        Sync();
    }
}

void UserAttrMgr::Sync() {
    std::unique_lock<std::shared_mutex> lockWriteUser(m_mutexDicUser);
    for (auto it : m_dicUser) {
        UserAttr tmp(it.first.c_str());
        tmp.SetSeparator(_separator);
        Log::WriteLog(log_info, "UserAttrMgr::Sync name:%s", it.first.c_str());
        bool bSuccess = QueryUserAttribute(&tmp);
        if (bSuccess) {
            auto second = dynamic_cast<UserAttr*>(it.second);
            if (second) 
                second->UpdateAttr(tmp);
        }
    }
}

#define BIG_QUERY_HARD_CODE

bool ImplQueryUserAttributeIni::QueryUserAttribute(UserAttr* pUser) {
#ifndef USE_INI_USER 
    return true;
#else
    try {
#ifdef BIG_QUERY_HARD_CODE
        //pUser->AddAttr("user_name", pUser->GetUserName().c_str());
#endif
        using namespace boost::property_tree;
        ptree lvptProperties;
#ifdef WIN32
        ini_parser::read_ini("C:\\ProgramData\\Nextlabs\\DAE\\Oracle\\config\\userattr.ini", lvptProperties);
#else
        ini_parser::read_ini("/home/joy_wu/DAE/Oracle/config/userattr.ini", lvptProperties);
#endif
        //walk through the config file
        ptree::iterator itSection = lvptProperties.begin();
        if (itSection == lvptProperties.end()) {
            return true;
        }
        for (; itSection != lvptProperties.end(); ++itSection) {
            std::string strSection = itSection->first;

            if (CommonFun::StrCaseCmp(strSection.c_str(), pUser->GetUserNameAttr().c_str()) != 0)
                continue;
            //read values from section
            basic_ptree<std::string, std::string> lvbtItems = itSection->second;
            for (basic_ptree<std::string, std::string>::iterator lvitem = lvbtItems.begin(); lvitem != lvbtItems.end(); lvitem++) {
                pUser->AddAttr((*lvitem).first.data(), (*lvitem).second.data().c_str());
                Log::WriteLog(log_info, "Read user attribute for %s, %s=%s", strSection.c_str(), (*lvitem).first.data(),
                    (*lvitem).second.data().c_str());
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;


#endif
}

ImplQueryUserAttributeDB::ImplQueryUserAttributeDB(const char *cnn_str, const char *user_tbl) :
    m_cnn(nullptr), m_stmt(nullptr) {

    CommonFun::FromUTF8(cnn_str, m_cnn_str);
    CommonFun::FromUTF8(user_tbl, m_user_tbl);
    Connect();
}

ImplQueryUserAttributeDB::~ImplQueryUserAttributeDB() {
    delete(m_cnn);
    m_cnn = nullptr;
}

bool ImplQueryUserAttributeDB::Connect() {
//    if (m_cnn) {
//        delete(m_cnn);
//        m_cnn = nullptr;
//    }
//    try {
//        std::string con_str;
//        CommonFun::ToUTF8(m_cnn_str, con_str);
//        Log::WriteLog(log_info, "user attr odbc cnn string: %s", con_str.c_str());
//        m_cnn = new nanodbc::connection(m_cnn_str);
//    }
//    catch (nanodbc::database_error const& e) {
//        std::string con_str;
//        CommonFun::ToUTF8(m_cnn_str, con_str);
//        Log::WriteLog(log_error, "ImplQueryUserAttributeDB::Connect error, the connect_str is %s, err is %s",
//            con_str.c_str(), e.what());
//        return false;
//    }
//
//    if (!m_cnn->connected())
//        return false;

//    try {
//        m_stmt = new nanodbc::statement(*m_cnn);
//    }
//    catch (nanodbc::database_error const& e) {
//        std::string con_str;
//        CommonFun::ToUTF8(m_cnn_str, con_str);
//        Log::WriteLog(log_error, "ImplQueryUserAttributeDB::Connect stmt error, the connect_str is %s, err is %s",
//            con_str.c_str(), e.what());
//        return false;
//    }
//
//    if (!m_stmt->connected())
//        return false;

    return true;
}

bool get_table_col_from_config(const nanodbc::string & src, nanodbc::string &table, nanodbc::string & col ){
    size_t pos = src.rfind('.');
    if(pos != std::string::npos){
        col = src.substr(pos+1);
        table = src.substr(0, pos);
    } else {
        return false;
    }
    return true;
}

bool ImplQueryUserAttributeDB::QueryUserAttribute(UserAttr* pUser) {
    //ADD BY JEFF
//    if (!m_cnn) return false;
//    try {
//        nanodbc::string query = NANODBC_TEXT("SELECT * FROM ")+ m_user_tbl+ NANODBC_TEXT(" WHERE user_name=?");
//        nanodbc::prepare(*m_stmt, query);
//
//        nanodbc::string user_name ;
//        CommonFun::FromUTF8(pUser->GetUserName(), user_name);
//        m_stmt->bind(0, user_name.c_str());
//
//        nanodbc::result results = nanodbc::execute(*m_stmt);

//    if (m_cnn) {
//        delete(m_cnn);
//        m_cnn = nullptr;
//    }
//
//    try {
//        std::string con_str;
//        CommonFun::ToUTF8(m_cnn_str, con_str);
//        Log::WriteLog(log_info, "user attr odbc cnn string: %s", con_str.c_str());
//        m_cnn = new nanodbc::connection(m_cnn_str);
//    }
//    catch (nanodbc::database_error const& e) {
//        std::string con_str;
//        CommonFun::ToUTF8(m_cnn_str, con_str);
//        Log::WriteLog(log_error, "ImplQueryUserAttributeDB::Connect error, the connect_str is %s, err is %s",
//                      con_str.c_str(), e.what());
//        return false;
//    }
//

//
//
//    if (!m_cnn){
//        Log::WriteLog(log_info, "QueryUserAttribute m_cnn == nullptr");
//        return false;
//    }
    try {

        std::string con_str;
        CommonFun::ToUTF8(m_cnn_str, con_str);
        nanodbc::connection conn(m_cnn_str);
        //conn.connect(m_cnn_str);
        if (!conn.connected()){
            Log::WriteLog(log_error, "QueryUserAttribute connection error");
            return  false;
        }


        nanodbc::string user_name ;
        CommonFun::FromUTF8(pUser->GetUserNameAttr(), user_name);
        nanodbc::string table_,column_;
        bool b = get_table_col_from_config(m_user_tbl,table_,column_);
        if(!b){
            Log::WriteLog(log_info, "config: user_info format is error.");
            return false;
        }
        nanodbc::string query = NANODBC_TEXT("SELECT * FROM ")+ table_+ NANODBC_TEXT(" WHERE ")+ column_+ NANODBC_TEXT(" = '") + user_name +NANODBC_TEXT("'");
        std::string ss;
        CommonFun::ToUTF8(query, ss);

        nanodbc::result results = nanodbc::execute(conn, query);

        short columns = results.columns();
        nanodbc::string const null_value = NANODBC_TEXT("null");
		bool bNull = true;
        while (results.next()) {
            for (short col = 0; col < columns; ++col) {
                nanodbc::string value = results.get<nanodbc::string>(col, null_value);
                nanodbc::string colname = results.column_name(col);

                // is_null() should be called after get() is called
                // because there is a bug/limitation in ODBC drivers for SQL Server
                if (!results.is_null(col))
                {
                    std::string keyA ;
                    CommonFun::ToUTF8(colname, keyA);
                    std::string valueA ;
                    CommonFun::ToUTF8(value, valueA);
                    pUser->AddAttr(keyA.c_str(), valueA.c_str());
                    Log::WriteLog(log_info, "Read user attribute for %s, %s=%s", pUser->GetUserNameAttr().c_str(), keyA.c_str(), valueA.c_str() );
                }
   			}
			bNull = false;
            break;  /* user_name is primary key */
        }
		if(bNull)
			return false;
    }
    catch (nanodbc::database_error const& e) {
        std::string use_tbl;
        CommonFun::ToUTF8(m_user_tbl, use_tbl);
        Log::WriteLog(log_error, "ImplQueryUserAttributeDB::QueryUserAttribute error, the user tbl is %s, err is %s", 
            use_tbl.c_str(), e.what());

        return false;
    }
    return true;
}