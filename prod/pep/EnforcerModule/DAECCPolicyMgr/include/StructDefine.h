#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H



#include <string>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include "s4hexception.h"

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(push, 0)
#pragma warning(disable:26812 5045 4548 4996)
#include <windows.h>
#include <Shlobj.h>
#else
#include <string.h>
#endif


typedef short PolicyEngineReturn;
typedef void* PolicyEngineHandle;

#define EM_ACTION_SELECT "SELECT"
#define EM_ACTION_UPDATE "UPDATE"
#define EM_ACTION_DELETE "DELETE"
#define EM_ACTION_INSERT "INSERT"
#define EM_ACTION_CONFIG "SECURITY_CONFIG"
#define EM_ACTION_UPSERT "UPSERT"
#define EM_ACTION_DROP   "DROP"
#define EM_ACTION_CREATE "CREATE"

typedef int(*WriteLog_Fun)(int nLogLevel, const char* szMsg);

enum class POLICY_ENGINE_HANDLE_TYPE { PE_STRING_LIST, PE_SUBJECT, PE_HOST, PE_APPLICATION, PE_RESOURCE };

enum POLICY_ENGINE_MATCH_RESULT { PE_NO_MATCHED = 0, PE_NEED_MORE_WORK /* todo */ };

//#define STRING_OP 4 /*if op < STRING_OP then must  be no string type*/
//class SQLCondition {
//public:
//    enum SQL_OP {SQL_GT = 0, SQL_GTEQ, SQL_LT, SQL_LTEQ, SQL_EQ, SQL_NEQ} ;
//    static SQL_OP TransformOp(const std::string & sop, S4HException & e) {
//        if(sop.length() == 1) {
//            if(sop[0] == '=') {
//                return SQL_EQ;
//            } else if (sop[0] == '>') {
//                return SQL_GT;
//            } else if (sop[0] == '<') {
//                return SQL_LT;
//            }
//        } else if (sop.length() == 2) {
//            if (sop[0] == '>' && sop[1] == '=') {
//                return SQL_GTEQ;
//            } else if (sop[0] == '<' && sop[1] == '=') {
//                return SQL_LTEQ;
//            } else if (sop[0] == '!' && sop[1] == '=') {
//                return  SQL_NEQ;
//            }
//        }
//        if(!sop.empty())
//            e._code = POLICY_ENGINE_FAIL;
//        return SQL_GT;
//    }
//    //static SQL_OP TransformOp(const std::string & sop, S4HException & e) ;
//    std::string                      _column1;
//    SQL_OP                              _op1;
//    std::string                      _constant_value1;
//    std::string                      _column2;
//    SQL_OP                              _op2;
//    std::string                      _constant_value2;
//    std::string                      _column3;
//    SQL_OP                              _op3;
//    std::string                      _constant_value3;
//};

struct IgnoreCaseCmpCC : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string& left, const std::string& right) const {
#ifndef _WIN32
        return strcasecmp(left.c_str(), right.c_str()) < 0;
#else
        return stricmp(left.c_str(), right.c_str()) < 0;
#endif
    }
//    static int compare(const std::string& left, const std::string& right) {
//#ifndef WIN32
//        return strcasecmp(left.c_str(), right.c_str());
//#else
//        return stricmp(left.c_str(), right.c_str());
//#endif
//    }

};


//Tables-based policy information
enum EnumAction
{
    ACT_None=0,
    ACT_SELECT = 1, // 0001
    ACT_UPDATE = 2, // 0010
    ACT_DELETE = 4,   // 0100
    ACT_INSERT = 8, // 1000
    ACT_UPSERT = 16,  //10000
    ACT_DROP   = 32,  //100000
    ACT_CREATE = 64,  //1000000
};
class TablePolicyInfo{
public:
    TablePolicyInfo():_bfilter(false),_bmask(false),_bdup(false),_actions(ACT_None){}
    //std::string _table;
    bool _bfilter;
    bool _bmask;
    bool _bdup;
    unsigned char _actions;//enum value
    static EnumAction GetEnumActionFromString(const std::string & action){
        if(action==EM_ACTION_SELECT)     return ACT_SELECT;
        if(action==EM_ACTION_UPDATE)     return ACT_UPDATE;
        if(action==EM_ACTION_DELETE)     return ACT_DELETE;
        if(action==EM_ACTION_INSERT)     return ACT_INSERT;
        if(action==EM_ACTION_UPSERT)     return ACT_UPSERT;
        if(action==EM_ACTION_DROP)       return ACT_DROP;
        if(action==EM_ACTION_CREATE)     return ACT_CREATE;
        return ACT_None;
    }
    //std::string _dbname;
    //std::string _schema;
};


typedef std::map<std::string , std::string> EMDictionary;
typedef std::vector<std::string> EMStrVector;
typedef std::vector<EMStrVector*> EMVec2StrVector;
typedef std::set<std::string> EMStrSet;
typedef std::vector<EMStrSet> EMStrSetVec;
typedef std::map<std::string, TablePolicyInfo, IgnoreCaseCmpCC>  TablePolicyInfoMap;



struct TableMetaDataItem
{
    std::string _col;
    std::string _type;
};

struct TableMetaData {
    std::string _db;
    std::string _sc;
    std::string _tb;
    std::vector<TableMetaDataItem> _metadata;
};

typedef std::shared_ptr<TableMetaData> TableMetaDataPtr;
typedef std::shared_ptr<std::vector<TableMetaDataPtr>>  TableMetaDataPtrVecPtr;
typedef std::map<std::string, TableMetaDataPtrVecPtr, IgnoreCaseCmpCC>  TableMetaDataPtrVecPtrMap;

#pragma warning(pop)

#endif