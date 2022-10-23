#include "sql_collect.h"
#include "DAELog.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <list>
#include "commfun.h"
#include "nanodbc.h"
#include "QueryCloudAZSDKCppWrapper.h"
#include "EMDBConfig.h"

#pragma warning(push,0)
#pragma warning(disable:4702 26812 6388 26451 26498 26495 6387 6258 6001 6255 6031 26439 5204 5219 5045 4355 4548 28251)
#include<boost/bind.hpp>
#include<boost/thread.hpp>
#pragma warning(pop)

#pragma warning(push,0)
#pragma warning(disable:26812 5045 6001 )



#define REPORT_ATTR_NAME1 "userName"
#define REPORT_ATTR_NAME2 "originSQL"
#define REPORT_ATTR_NAME3 "newSQL"
#define REPORT_ATTR_NAME4 "enforcerResultCode"
#define REPORT_ACTION "EMDB_AUDIT_LOG"
#define REPORT_EF_BLOCK_HINT "policy deny"
#define REPORT_EF_SUCCEED_HINT "enforcer succeeded"
#define REPORT_EF_PARSE_FAILED "sql parse failed"
#define REPORT_ID "EMDB_AUDIT_LOG"



struct ImplSqlCollectLog : public ISqlCollect {

    virtual void collect_enforcer_result(const EnforcerResultData &data){
        if(data.oriSql.empty()||data.newSql.empty())
            return;
//       theLog->WriteLog(log_info, "--ori\n%s", data.oriSql.c_str());
//       theLog->WriteLog(log_info, "--new\n%s", data.newSql.c_str());
        size_t iLen = data.oriSql.length() + data.newSql.length() + 100;
        char * buf = new char[iLen];
        sprintf(buf, "\n--ori\n%s\n--new\n%s", data.oriSql.c_str(), data.newSql.c_str() );
// printf("\n--ori\n%s\n--new\n%s", data.oriSql.c_str(), data.newSql.c_str() );
// printf("%s",buf);
        theLog->WriteLog(log_info, "%s", buf);
        delete[] buf;
    }

    //ignore
    void collect_report_result(std::vector<std::shared_ptr<RequestInfo>>& datas, const std::string& old_sql, const std::string& new_sql) {};
    void collect_sql(const std::string & old_sql, const std::string & new_sql) {};
};

/*
struct Item {
    std::string sql_ori;
    std::string sql_new;
    int is_parsed_success_;
};

struct ImplSqlCollectDB : public ISqlCollect {
    ImplSqlCollectDB(const std::wstring sconnect, const std::wstring stable) : 
        cnn_str_(sconnect),
        sql_tbl_(stable),
        cnn_(nullptr),
        stmt_(nullptr)
        //sql_(L"IF (SELECT COUNT(*) FROM #emdbtable# WHERE sql_script = ? ) = 0 INSERT INTO NXL_USER.DBO.SQL_COLLECT VALUES (? , ?);") 
    {
        sql_.append(L"IF (SELECT COUNT(*) FROM ");
        sql_.append(stable);
        sql_.append(L" WHERE sql_script_ori = ? ) = 0 INSERT INTO ");
        sql_.append(stable);
        sql_.append(L" VALUES (?, ?, ?);");
        connect();
        std::thread t1(std::bind(&ImplSqlCollectDB::handle, this));
        t1.detach();
    }

    virtual void collect_enforcer_result(const EnforcerResultData &data){
        Item i; 
        i.sql_ori = data.oriSql;
        i.sql_new = data.newSql;
        i.is_parsed_success_ = data.bParseSucceed?1:0;//==0 ? 0 : 1;
        sql_collect_mutex_.lock();
        items_.push_back(i);
        sql_collect_mutex_.unlock();
        cond_.notify_one();
    }
private:
    bool connect() {
        if (cnn_) {
            delete(cnn_);
            cnn_ = nullptr;
        }
        try {
            cnn_ = new nanodbc::connection(cnn_str_);
        }
        catch (nanodbc::database_error const& e) {
            return false;
        }

        if (!cnn_->connected())
            return false;

        try {
            stmt_ = new nanodbc::statement(*cnn_);
        }
        catch (nanodbc::database_error const& e) {
            return false;
        }

        if (!stmt_->connected())
            return false;

        try {
            nanodbc::prepare(*stmt_, sql_);
        }
        catch (nanodbc::database_error const& e) {
            return false;
        }

        return true;
    }

    void handle() {
         while (true) {
            std::unique_lock<std::mutex> lk(sql_collect_mutex_);
            while (items_.empty())
                cond_.wait(lk);
            std::list<Item> tmp;
            tmp.swap(items_);
            lk.unlock();
            handle_(tmp);
         }
    }

    void handle_(const std::list<Item>& items) {
        for (auto it : items) {
            handle_one(it);
        }
    }

    void handle_one(const Item& item) {
        try {
            std::wstring s_ori ;
            CommonFun::FromUTF8(item.sql_ori, s_ori);
            std::wstring s_new ;
            CommonFun::FromUTF8(item.sql_new, s_new);
            stmt_->bind(0, s_ori.c_str());
            stmt_->bind(1, s_ori.c_str());
            stmt_->bind(2, s_new.c_str());
            stmt_->bind(3, &item.is_parsed_success_);

            nanodbc::execute(*stmt_);
            
        }
        catch (nanodbc::database_error const& e) {
            const char* err = e.what();
        }
    }

private:
    nanodbc::connection *cnn_;
    nanodbc::statement  *stmt_;

    std::list<Item> items_;

    std::mutex sql_collect_mutex_;
    std::condition_variable cond_;


    const std::wstring cnn_str_;
    const std::wstring sql_tbl_;
    std::wstring sql_;
};

*/


class ImplSqlCollectReport : public ISqlCollect{
public:
    ImplSqlCollectReport(bool binit){
        if(binit){
            boost::thread t(boost::bind(&ImplSqlCollectReport::output_value_post_to_cc, this));
            t.detach();
        }
        _init = binit;
    }

    ~ImplSqlCollectReport(){}
    virtual void collect_enforcer_result(const EnforcerResultData &data){
        if(data.oriSql.empty()||data.newSql.empty())
            return;
//       theLog->WriteLog(log_info, "--ori\n%s", data.oriSql.c_str());
//       theLog->WriteLog(log_info, "--new\n%s", data.newSql.c_str());
        size_t iLen = data.oriSql.length() + data.newSql.length() + 100;
        char * buf = new char[iLen];
        sprintf(buf, "\n--ori\n%s\n--new\n%s", data.oriSql.c_str(), data.newSql.c_str() );
// printf("\n--ori\n%s\n--new\n%s", data.oriSql.c_str(), data.newSql.c_str() );
// printf("%s",buf);
        theLog->WriteLog(log_info, "%s", buf);
        delete[] buf;
    }


    void output_value_post_to_cc(){

        while (true)
		{

            std::vector<std::shared_ptr<RequestInfo>> listTemp;
			std::unique_lock < std::mutex > uniqlock(m_mutex);

			m_cond.wait(uniqlock,
				[this] {
                    return !m_list_result.empty();
				//return !m_list_result.empty() && !m_list_result[0]._ori.empty();
			});
            listTemp.swap(m_list_result);
			uniqlock.unlock();

            size_t ilen = listTemp.size();
            const IPolicyRequest ** reqs = (const IPolicyRequest**)calloc(ilen, sizeof(IPolicyRequest*));
            //memset(reqs, 0, ilen*sizeof(IPolicyRequest*));
            for(size_t i = 0 ; i < ilen; ++i){
                RequestInfo & reqinfo = *(listTemp[i].get());
               
                std::string username = "user";
                auto it = reqinfo._users2.find("id");
                if(it != reqinfo._users2.end()){
                    username = it->second.get()->strValue;
                }
                // CommonFun::StrCaseCmp("id",it.first.c_str())
                reqs[i] = create_request(reqinfo, username);
            }
            if(ilen==1){
                IPolicyResult* result = NULL;
                theLog->WriteLog(log_info, "auditlog query_single_before");
                QueryStatus  status = single_query_pc(reqs[0], &result);
                theLog->WriteLog(log_info, "auditlog query_single_end");
                free_pc_request(const_cast<IPolicyRequest*>(reqs[0]));
                free(reqs);
                if (QS_S_OK != status ) {
                    theLog->WriteLog(log_error, "auditlog query_single failed");
                }
                free_pc_result(result);

            } else if(ilen > 1){
                IPolicyResult ** result = (IPolicyResult**)calloc(ilen,sizeof(IPolicyResult*));
                theLog->WriteLog(log_info, "auditlog query_multi_before");
                QueryStatus status= multi_query_pc(&reqs[0], (int)ilen, result);
                theLog->WriteLog(log_info, "auditlog query_multi_end");
                for (size_t i = 0; i < ilen; ++i){
                    free_pc_request(const_cast<IPolicyRequest*>(reqs[i]));
                }
                free(reqs);

                if (QS_S_OK != status ) {
                    theLog->WriteLog(log_error, "auditlog multi query pc failed");
                    return;
                }
                for (size_t i = 0; i < ilen; ++i){
                    if (result && result[i] != NULL) {
                        free_pc_result(result[i]);
                    }
                    
                }
                free(result);
            }
        }
    }

public:
    void collect_report_result(std::vector<std::shared_ptr<RequestInfo>> & datas, const std::string & old_sql, const std::string & new_sql) {
        if(!_init){
            return;
        }
        for (auto& it : datas) {
            {
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(old_sql, XACML_string);
                it->_rescs2.insert(std::make_pair("OriSQL", ptrb));
            }
            {
                std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(new_sql, XACML_string);
                it->_rescs2.insert(std::make_pair("NewSQL", ptrb));
            }
        }
        

        m_mutex.lock();
        if (m_list_result.size() < 10) {
            for (auto& it : datas) {
                m_list_result.push_back(it);
            }
        }
        m_mutex.unlock();
        m_cond.notify_one();
    };
    //virtual void collect_sql(const std::string & old_sql, const std::string & new_sql) {
    //    if(!_init){
    //        return;
    //    }
    //    m_mutex.lock();
    //    for(size_t  i = 0 ; i < m_list_result.size();++i){
    //        if(m_list_result[i]._ori.empty() ){
    //            m_list_result[i]._ori = old_sql;
    //        }
    //        if(m_list_result[i]._new.empty()){
    //            m_list_result[i]._new = new_sql;
    //        }
    //    }
    //    m_mutex.unlock();
    //    m_cond.notify_one();
    //}
private:

    std::vector<std::shared_ptr<RequestInfo>> m_list_result;
    //EMAuditlogInfo _result;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	bool _init;

};



struct ImplSqlCollectNull :public ISqlCollect {
    virtual ~ImplSqlCollectNull() {}
    virtual void collect_enforcer_result(const EnforcerResultData &data) { }
    //ignore
    void collect_report_result(std::vector<std::shared_ptr<RequestInfo>>& datas, const std::string& old_sql, const std::string& new_sql) {};
    void collect_sql(const std::string & old_sql, const std::string & new_sql) {};
};

ISqlCollect *make_sql_collector(bool init_) {
    unsigned int iSwitch = EMDBConfig::GetInstance().get_log_switch_report();

    if (iSwitch == 0)
    {
        return new ImplSqlCollectNull();
    }
    else if(iSwitch == LOG_CIPHER){
        return new ImplSqlCollectLog(); 
    }
    else{
        return new ImplSqlCollectReport(init_);
    }

    /*
    int iMode = 2;
    std::string smode = (*theConfig)[CFG_LOG_MODE];
    if(!smode.empty()){
         iMode = atoi(smode.c_str());
    }
   
    switch (iMode) {
        case 0: { 
                    return new ImplSqlCollectLog; 
                } break;
        case 1: { 
                    if((*theConfig)[CFG_LOG_PARAM1].empty()||(*theConfig)[CFG_LOG_PARAM2].empty())
                        return new ImplSqlCollectLog;
                    std::wstring parm1,parm2 ;
                    CommonFun::FromUTF8((*theConfig)[CFG_LOG_PARAM1], parm1);
                    CommonFun::FromUTF8((*theConfig)[CFG_LOG_PARAM2], parm1);
                    return new ImplSqlCollectDB(parm1.c_str(), \
                        parm2.c_str()); 
                } break;
        case 2:{
                    return new ImplSqlCollectReport((*theConfig)[CFG_POLICY_JPCHOST],\
                        (*theConfig)[CFG_POLICY_JPCPORT],\
                        (*theConfig)[CFG_POLICY_CCHOST],\
                        (*theConfig)[CFG_POLICY_CCPORT],\
                        (*theConfig)[CFG_POLICY_JPCUSER],\
                        (*theConfig)[CFG_POLICY_JPCPWD]);
                } break;
        default: return new ImplSqlCollectLog;
    }
    */ 
}

#pragma warning(pop)