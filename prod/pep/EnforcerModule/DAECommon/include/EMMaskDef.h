#ifndef MASK_HEADER
#define MASK_HEADER

#include <map>
#include <set>
#include <string>
#include <EMDBType.h>

#ifdef WIN32
#pragma warning(disable:4996 26812)
#include <windows.h>
#include <Shlobj.h>
#else
#include <string.h>


#endif

struct IgnoreCaseCmp : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string& left, const std::string& right) const {
#ifndef WIN32
        return strcasecmp(left.c_str(), right.c_str()) < 0;
#else
        return stricmp(left.c_str(), right.c_str()) < 0;
#endif
    }
   static int compare(const std::string& left, const std::string& right) {
#ifndef WIN32
       return strcasecmp(left.c_str(), right.c_str());
#else
       return stricmp(left.c_str(), right.c_str());
#endif
    }

};
class SqlCommon {
public:
    static std::string EscapeQuotationSqlString(const std::string& src) {
        std::string ret = "";
        if (src.length() <= 0 || src[0] != '\'') {
            return ret;
        }
        std::string l = "'";
        size_t ifind = src.rfind(l);
        if (ifind > l.length()) {
            ret = src.substr(l.length(), ifind- l.length());
        }

        if (l.length() == 1) { // replace ''  to '
            while (true) {
                std::string::size_type   pos(0);
                if ((pos = ret.find("''")) != std::string::npos)
                    ret.replace(pos, 2, "'");
                else   break;
            }
        }

        return ret;
    }
};




enum EMDataType{
    EMDATA_STRING,
    EMDATA_NUMBER,
    EMDATA_DATE,
    EMDATA_TIME,
    EMDATA_TIMESTAMP,
    EMDATA_STRING2,
    EMDATA_TIMESTAMP_TZ_ORA,
    EMDATA_YMINTERVAL,
    EMDATA_DSINTERVAL,
    EMDATA_NONSUPPORT

};
typedef  std::map<std::string, EMDataType, IgnoreCaseCmp> DatatypeMap; // field - mask item
static DatatypeMap g_datatype_map
        {
                //
                {"STRING",EMDATA_STRING},
                {"NSTRING",EMDATA_STRING2},
                {"TIME",EMDATA_TIME},
                {"DATE",EMDATA_DATE},
                {"NUMBER",EMDATA_NUMBER},

                //ORACLE
                {"VARCHAR2",EMDATA_STRING},
                {"CHAR",EMDATA_STRING},
                {"NVARCHAR2",EMDATA_STRING2},
                {"NCHAR",EMDATA_STRING2},
                {"BINARY_DOUBLE",EMDATA_NUMBER},
                {"BINARY_FLOAT",EMDATA_NUMBER},
               // {"LONG",EMDATA_NUMBER},
                {"FLOAT",EMDATA_NUMBER},
                {"TIMESTAMP WITH TIME ZONE",EMDATA_TIMESTAMP_TZ_ORA},
                {"TIMESTAMP WITH LOCAL TIME ZONE",EMDATA_TIMESTAMP_TZ_ORA},
                {"TIMESTAMP",EMDATA_TIMESTAMP},
                {"INTERVAL YEAR TO MONTH",EMDATA_YMINTERVAL},
                {"INTERVAL DAY TO SECOND",EMDATA_DSINTERVAL},
                {"VARCHAR",EMDATA_STRING},
                {"CHARACTER",EMDATA_STRING},
                {"CHARACTER VARYING",EMDATA_STRING},
                {"CHAR VARYING",EMDATA_STRING},
                {"NCHAR VARYING",EMDATA_STRING2},
                {"NATIONAL CHARACTER VARYING",EMDATA_STRING2},
                {"NATIONAL CHARACTER",EMDATA_STRING2},
                {"NATIONAL CHAR",EMDATA_STRING2},
                {"NATIONAL CHAR VARYING",EMDATA_STRING2},
                {"SMALLINT",EMDATA_NUMBER},
                {"INT",EMDATA_NUMBER},
                {"NUMERIC",EMDATA_NUMBER},
                {"DECIMAL",EMDATA_NUMBER},
                {"DEC",EMDATA_NUMBER},
                {"INTEGER",EMDATA_NUMBER},
                {"DOUBLE PRECISION",EMDATA_NUMBER},
                {"REAL",EMDATA_NUMBER}

        };

static DatatypeMap g_datatype_map_ms
        {
                // base
                {"STRING",EMDATA_STRING},
                {"NSTRING",EMDATA_STRING2},
                {"TIME",EMDATA_TIME},
                {"DATE",EMDATA_DATE},
                {"NUMBER",EMDATA_NUMBER},
                // sqlserver
                {"bigint",EMDATA_NUMBER},
                { "numeric",EMDATA_NUMBER},
                {"bit",EMDATA_NUMBER},
                {"smallint",EMDATA_NUMBER},
                {"decimal",EMDATA_NUMBER},
                {"smallmoney",EMDATA_NUMBER},
                {"int",EMDATA_NUMBER},
                {"tinyint",EMDATA_NUMBER},
                {"money",EMDATA_NUMBER},

                { "float",EMDATA_NUMBER},
                {"real",EMDATA_NUMBER},

                //{"date",EMDATA_DATE},
                {"datetimeoffset",EMDATA_TIMESTAMP},
                {"datetime2",EMDATA_TIMESTAMP},
                {"smalldatetime",EMDATA_TIMESTAMP},
                {"datetime",EMDATA_TIMESTAMP},
                //{"time",EMDATA_TIME},
               // {"timestamp"      //    nonsupport
                {"char",EMDATA_STRING},
                {"varchar",EMDATA_STRING},
                {"text",EMDATA_STRING},
                {"nchar",EMDATA_STRING},
                {"nvarchar",EMDATA_STRING},
                {"ntext",EMDATA_STRING},
               // {"binary",EMDATA_STRING},
               // {"varbinary",EMDATA_STRING},
                {"image",EMDATA_STRING}
//                {"cursor"
//                {"rowversion"
//                {"hierarchyid"
//                {"uniqueidentifier"
//                {"sql_variant"
//                {"xml"
//                {"Geometry"
//                {"Geography"
//                {"table"
        };
enum EMMaskFormat{
    EMMASK_FULL,
    EMMASK_RANDNUMBER,
    EMMASK_PARTIAL,
    EMMASK_KEY
};

struct MaskItem {
    std::string _condition;
    std::string _symbols;
    EMDataType _datatype;
    EMMaskFormat _format;
    std::string _qulify;
    std::set<std::string> _cond_fields;
    //
    static EMDataType GetDataType(EMDB_DB_TYPE type,const std::string & val){
        EMDataType ret = EMDATA_STRING;
        if(type == EMDB_DB_SQLSERVER){
            auto it = g_datatype_map_ms.find(val);
            if(it != g_datatype_map_ms.end()){
                ret = it->second;
            } else {
                ret = EMDATA_NONSUPPORT;
            }
        } else {
            auto it = g_datatype_map.find(val);
            if(it != g_datatype_map.end()){
                ret = it->second;
            } else {
                ret = EMDATA_NONSUPPORT;
            }
        }

        return ret;
    }
    static EMMaskFormat GetMaskFormat(const std::string & val){
        EMMaskFormat ret = EMMASK_FULL;
        if(val.compare("FullMask") == 0){
            ret = EMMASK_FULL;
        } else if(val.compare("RandNumber") == 0){
            ret = EMMASK_RANDNUMBER;
        }
        else if(val.compare("PartialMask") == 0){
            ret = EMMASK_PARTIAL;
        }
        else if(val.compare("KeyMask") == 0){
            ret = EMMASK_KEY;
        }
        return ret;
    }

};
typedef  std::map<std::string, MaskItem*, IgnoreCaseCmp> MaskItemMap; // field - mask item
typedef  std::map<std::string, MaskItemMap*, IgnoreCaseCmp> MaskConditionMap; // table - mask columns
struct Metadata {
    std::string _col;
    EMDataType _type{ EMDATA_STRING };
};
#include <vector>
#include <functional>
typedef std::vector<Metadata> MetadataVec;
typedef std::function<bool (const std::string &,MetadataVec & )> QueryMetadataFunc;

#ifdef _WIN32
typedef struct _Metadata_c {
    char* _col;
    EMDataType _type{ EMDATA_STRING };
} Metadata_c;

typedef bool (CALLBACK *QueryMetadataFunc_c)(const char*, Metadata_c**, int*);

struct DAEBindingParam {
    std::string _type;
    std::string _value;
};
typedef std::vector<DAEBindingParam> VecBindSqlParams;
#endif

#endif
