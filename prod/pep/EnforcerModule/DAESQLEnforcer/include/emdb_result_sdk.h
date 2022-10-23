#ifndef EMDBRESULT_H
#define EMDBRESULT_H
#include "stdint.h"
#include <string>
#include "emdb_sdk.h"
#include "emdb_handle.h"

class CEMDBResult :public CEMDBHandle
{
public:
	CEMDBResult();
	~CEMDBResult();

	virtual bool IsType(uint64_t value){
		return m_magicNumber == value;
	}
	EMDBResultCode GetCode(){return m_code;};
	const char* GetDetail(){return m_strDetail.c_str();};
	void SetValue(EMDBResultCode code, const std::string & detail ){
		m_code = code;
		m_strDetail = detail;
	};
	// void Clear(){
	// 	m_code = EMDB_BLOCK_THIS_TEXT;
	// 	m_strDetail.clear();
	// }

private:
	EMDBResultCode m_code;
	std::string m_strDetail;

};

//#include "emdb_bigquery_sdk.h"
//class CEMDB_BQ_Result
//{
//public:
//    CEMDB_BQ_Result():_code(EMDB_IGNORE){}
//    ~CEMDB_BQ_Result(){}
//
//
//    EMDB_BQ_ResultCode GetCode(){return _code;};
//    const char* GetSQL(){
//        if(_sql.empty())
//            return NULL;
//        return _sql.c_str();
//    };
//    const char* GetError(){
//        if(_error.empty())
//            return NULL;
//        return _error.c_str();
//    }
//    void SetValue(EMDB_BQ_ResultCode code, const std::string & sql ,const std::string & error){
//        _code = code;
//        _sql = sql;
//        _error = error;
//    };
//
//private:
//    EMDB_BQ_ResultCode _code;
//    std::string _error;
//    std::string _sql;
//
//};


#endif

