// DAESQLEnforcer.cpp : Defines the entry point for the application.
//

#include "DAESQLEnforcer.h"
#include "DAESqlServerSDK.h"
#include "PolicyExport.h"
#include "microtest.h"
#include "enforcerwrapper.h"

using namespace std;
using namespace DAESqlServer;
//void DAECCPolicyMgr_test();
void SQLEnforcer_test(const char * sql);





//TEST(PolicyMgrTest) 
//void PolicyMgrTest()
//{
//
//    const std::string cchost = "https://cc202103.qapf1.qalab01.nextlabs.com";
//    const std::string ccport = "443";
//    const std::string ccuser = "administrator";
//    const std::string ccpwd = "12345Blue!";
//    const std::string model = "daeoracle";
//    const std::string tag = "DAE";
//    unsigned int sync_interval_seconds = 10;
//    S4HException exc;
//    
//    bool binit =  PolicyInit(cchost, ccport, ccuser, ccpwd, model, tag, sync_interval_seconds, exc, NULL);
//
//    TablePolicyInfoMap tbs_map;
//
//    bool ret = GetTablePolicyinfo(tbs_map,  exc);
//    UpdateSyncInterval(3600);
//    ret = Exit();
//
//   
//}

//TEST(SQLEnforcerCheck) {
//    unsigned len = 1024;
//    char* detail = new char[len];// = { 0 };
//    EnforcerCheck( detail, len);
//    cout << detail  << endl;
//    //cc check
//    // 
//    //nginx proxy check
//    // 
//    //get table metadata db config check
//
//
//    delete[] detail;
//}
#include <chrono>
using namespace std::chrono;
#include <thread>


#include <string>
#include <iostream>
#include <fstream>
#include <fstream>
#include <Windows.h>
void SQLEnforcer_test_forever(size_t id);

TEST(SQLEnforcer1) {
    bool bret = DAEEnforcerMgr::Instance()->DAESqlServerInit("sqlserver");
    if (!bret) {
        return;
    }
    //
    //std::ifstream t("C:\\WorkSpace\\TempSource\\x86setdatatypes.txt");
    //std::string val86((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    //std::ifstream t2("C:\\WorkSpace\\TempSource\\x64setdatatypes.txt");
    //std::string val64((std::istreambuf_iterator<char>(t2)), std::istreambuf_iterator<char>());
    //int i = 0;
    //int pos = 0;
    //int line = 1;
    //for (; i < val64.length(); ++i) {
    //    if (val64[i] == '\n') {
    //        pos = i;
    //        line++;
    //    }
    //    if (val86[i] != val64[i]) {
    //        std::string print64 = val64.substr(pos + 1, 47);
    //        std::string print86 = val86.substr(pos + 1, 47);
    //        printf("i=%d  line=%d\n,x64:%s,x86:%s\n", i, line,print64.c_str(), print86.c_str());
    //    }
    //}
    SQLEnforcer_test("SELECT C_MKTSEGMENT FROM guest.customer_mask WHERE 'DAE_APP_USER' <> 'CIC1CA';");
    SQLEnforcer_test("select a.*,b.*,c.*\n"
        "from GUEST.CUSTOMER_MASK a, GUEST.CUSTOMER2 b, GUEST.CUSTOMER3 c\n"
        "where a.customer_id = b.customer_id and b.customer_id = c.customer_id and a.c_mktsegment != 'Catering'\n"
        "order by a.c_mktsegment");
    SQLEnforcer_test("SELECT *	 FROM (GUEST.customer_mask a right join GUEST.ORDERS b on a.C_PRIVILEGE_LEVEL != b.O_SHIPPRIORITY)  inner join GUEST.CUSTOMER2 c  USING (c_privilege_level,c_custkey) ");
    printf("aaa");
    SQLEnforcer_test("SELECT * FROM (GUEST.customer_mask a right join GUEST.ORDERS b on\n"
        "a.C_PRIVILEGE_LEVEL != b.O_SHIPPRIORITY)\n"
        "inner join GUEST.CUSTOMER2 c\n"
        "USING(c_privilege_level, c_custkey)");
}

//TEST(SQLEnforcer) 
void TEST11111()
{


    bool bret = DAEEnforcerMgr::Instance()->DAESqlServerInit("sqlserver");
    if (!bret) {
        return;
    }

    printf("-------------one--thread------\n");


    SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3 as A WHERE 'DAE_APP_USER' <> 'Joy'");
    SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3  WHERE 'DAE_APP_USER' <> 'Joy'");
    SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3 as b WHERE 'DAE_APP_USER' <> 'Joy'");
    SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3  WHERE 'ddd' <> ? and 'DAE_APP_USER' <> ? and 'ppp' <> ?");

    //SQLEnforcer_test("insert into guest.CUSTOMER4 values('1','2','3');");
    //SQLEnforcer_test("SELECT   \"select * from guest.customer3\".C_MKTSEGMENT FROM guest.customer3 AS \"select * from guest.customer3\"");
    //SQLEnforcer_test("WITH \"__JDBC_ROWIDS__\" AS (SELECT COLUMN_VALUE ID, ROWNUM NUM FROM TABLE(:rowid0))\n"
    //                "SELECT \"__JDBC_ORIGINAL__\".*\n"
    //                "FROM (select rowid as \"__Oracle_JDBC_internal_ROWID__\", CUSTOMER_ID, C_MKTSEGMENT, C_PRIVILEGE_LEVEL FROM guest.customer3 where 'DAE_APP_USER'<>:rows ) \"__JDBC_ORIGINAL__\", \"__JDBC_ROWIDS__\"\n"
    //                "WHERE \"__JDBC_ORIGINAL__\".\"__Oracle_JDBC_internal_ROWID__\"(+) = \"__JDBC_ROWIDS__\".ID\n"
    //    "ORDER BY \"__JDBC_ROWIDS__\".NUM");
    //SQLEnforcer_test("update guest.CUSTOMER4 set c = 'sss' ");
    //SQLEnforcer_test("delete from  guest.CUSTOMER4 where c = 123;");

    printf("-------------2--thread------\n");

    for (size_t i = 0; i < 2; i++) {
        std::thread threadi(&SQLEnforcer_test_forever,i);
        threadi.detach();
    }

    Sleep(10000 * 1000);

}

void SQLEnforcer_test_forever(size_t id) {
    size_t times = 0;
    do {
        SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3 as A WHERE 'DAE_APP_USER' <> 'Joy'");
        SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3  WHERE 'DAE_APP_USER' <> 'Joy'");
        SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3 as b WHERE 'DAE_APP_USER' <> 'Joy'");
        SQLEnforcer_test("SELECT CUSTOMER_ID, C_MKTSEGMENT FROM guest.customer3  WHERE 'ddd' <> ? and 'DAE_APP_USER' <> ? and 'ppp' <> ?");
        //SQLEnforcer_test("delete from  guest.CUSTOMER4 where c = 123;");
        Sleep(1000);
        printf("------times:%ld\n------ %d------", times++, id);
    } while (1);
}




void SQLEnforcer_test(const char * sqlori)
{

    DAE_ResultCode code;
  
    //int attr_size = 2;
    DAE_UserProperty* puserattr = new DAE_UserProperty[2];// (DAE_UserProperty*)calloc(attr_size, sizeof(DAE_UserProperty));
    puserattr[0]._key = "auth_sid"; 
    puserattr[0]._value = "administrator";
    puserattr[1]._key = "SERVICE_NAME";
    puserattr[1]._value = "ORCLCDB";


    DAEResultHandle result = NULL;
    bool bret = DAEEnforcerMgr::Instance()->DAENewResult(&result);

    DAE_BindingParam* params = new DAE_BindingParam[3];
    params[0]._value = "JOY";
    params[1]._value = "Joy";
    params[2]._value = "JOY";

    unsigned len = 1024;
    bret = DAEEnforcerMgr::Instance()->EnforceSQL_simpleV2(   sqlori, puserattr, 2, params, 3, result);

    const char* sqlnew = NULL;
    const char* db = NULL;
    bret = DAEEnforcerMgr::Instance()->DAEGetResult(result, &code, &sqlnew,&db);
    switch (code) {
    case DAE_ResultCode::DAE_ENFORCED: {
            cout << "---NEWSQL----" << sqlnew << endl;//new sql :  deny / filter / mask
        } break;
    case DAE_ResultCode::DAE_FAILED: {
            cout << "---ERROR----" << sqlnew << endl;//error 
        } break;
    case DAE_ResultCode::DAE_IGNORE: {
            cout << "---IGENORE----" << sqlnew << endl;//reson
        } break;
        default: {
            cout << sqlnew << endl;//error is sqlnew
        }
    }

    bret = DAEEnforcerMgr::Instance()->DAEFreeResult(result);
    
    delete[] puserattr;
    delete[] params;
    return;
}



TEST_MAIN()