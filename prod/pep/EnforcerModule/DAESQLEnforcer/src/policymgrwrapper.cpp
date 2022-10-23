 #include "policymgrwrapper.h"
 #include "commfun.h"
 #include "EMDBConfig.h"
 #include "DAElog.h"

 #ifdef WIN32
 #include <windows.h>
 #else
 #include <dlfcn.h>
 #endif

 bool load_ssl_lib(const std::string& module_name) {
     std::string module_path = CommonFun::GetSSLModule(module_name);
     void *module = CommonFun::LoadShareLibrary(module_path.c_str());
     if (module==NULL) {
         theLog->WriteLog(log_fatal, LOAD_MODULE_FAILED, module_path.c_str(), CommonFun::ShareLibraryError().c_str());
         return false;
     }
     return true;
 }



 bool CCPolicyMgrWrapper::LoadPolicyMgr()
 {
     //get openssl
     if (!load_ssl_lib("libeay32"))
         return false;
     if (!load_ssl_lib("ssleay32"))
         return false;
     const std::string strInstallPath = EMDBConfig::GetInstance().get_global_install_path();
#ifdef WIN32
#ifdef _WIN64
     //std::string strPath = strInstallPath + "\\Common\\bin64\\DAECCPolicyMgr.dll";
     std::string strPath = "DAECCPolicyMgr.dll"; 
#else
     std::string strPath = strInstallPath + "\\Common\\bin32\\DAECCPolicyMgr.dll";
#endif
#else
     std::string strPath = strInstallPath + "/Common/bin64/libDAECCPolicyMgr.so";
#endif
     //load library
     void* hModule = CommonFun::LoadShareLibrary(strPath.c_str());
     if (hModule==NULL) {
         #ifdef WIN32
         theLog->WriteLog(log_fatal, "load DAECCPolicyMgr.dll failed, dwlasterror=0x%x", GetLastError() );
         #else 
         theLog->WriteLog(log_fatal, "load libDAECCPolicyMgr.so failed, dlerror=%s", dlerror()!=NULL ? dlerror() : "NULL" );
         #endif 
         return false;
     }
     //get function address
     PolicyInit = (PolicyInit_Func)CommonFun::GetProcAddress(hModule, "PolicyInit");
     GetTablePolicyinfo = (GetTablePolicyinfo_Func)CommonFun::GetProcAddress(hModule, "GetTablePolicyinfo");
     Exit = (Exit_Func)CommonFun::GetProcAddress(hModule, "Exit");
     UpdateSyncInterval = (UpdateSyncInterval_Func)CommonFun::GetProcAddress(hModule, "UpdateSyncInterval"); 
     IsNeedEnforcer = (IsNeedEnforcer_Func)CommonFun::GetProcAddress(hModule, "IsNeedEnforcer");
     BindUpdateNotification = (BindUpdateNotification_Func)CommonFun::GetProcAddress(hModule, "BindUpdateNotification");

     m_bWell = (PolicyInit != NULL) &&
               (GetTablePolicyinfo != NULL) &&
               (Exit != NULL) &&
               (UpdateSyncInterval != NULL) &&
               (IsNeedEnforcer != NULL) &&
               (BindUpdateNotification != NULL);
     return m_bWell;
 }