#include "sqlparserwrapper.h"
#include "commfun.h"
#include "DAELog.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

CSqlparserWrapper::CSqlparserWrapper() : 
  ParseSql(NULL), DestroyParseResult(NULL), CreatePlan(NULL),
  VisitPlan(NULL), DestroyPlan(NULL), CompressSql(NULL), BeautifySql(NULL), m_bWell(false)
{}

bool CSqlparserWrapper::LoadParser()
{
    std::string strParserPath = CommonFun::GetSqlParserModule();
    //load library
    void* hModule = CommonFun::LoadShareLibrary(strParserPath.c_str());
    if (hModule==NULL) {
        theLog->WriteLog(log_fatal, LOAD_MODULE_FAILED, strParserPath.c_str(), CommonFun::ShareLibraryError().c_str());
        return false;
    }

    //get function address
    this->ParseSql              = (ParseSqlFun)             CommonFun::GetProcAddress(hModule, "ParseSql");
    this->DestroyParseResult    = (DestroyParseResultFun)   CommonFun::GetProcAddress(hModule, "DestroyParseResult");
    this->CreatePlan            = (CreatePlanFun)           CommonFun::GetProcAddress(hModule, "CreatePlan");
    this->VisitPlan             = (VisitPlanFun)            CommonFun::GetProcAddress(hModule, "VisitPlan");
    this->DestroyPlan           = (DestroyPlanFun)          CommonFun::GetProcAddress(hModule, "DestroyPlan");
    this->CompressSql           = (CompressSqlFun)          CommonFun::GetProcAddress(hModule, "CompressSql");
    this->BeautifySql           = (BeautifySqlFun)          CommonFun::GetProcAddress(hModule, "BeautifySql");


    m_bWell = this->ParseSql!=NULL &&
           this->DestroyParseResult!=NULL &&
           this->CreatePlan!=NULL &&
           this->VisitPlan!=NULL &&
           this->DestroyPlan!=NULL &&
           this->CompressSql!=NULL &&
           this->BeautifySql!=NULL ;

    return m_bWell;

}