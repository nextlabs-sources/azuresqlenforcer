#include "userattrwrapper.h"
#include "EMDBConfig.h"
#include "commfun.h"
#include "DAELog.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>

#endif

bool UserAttrWrapper::LoadUserAttributeModule()
{
      //get sqlparser component path
    std::string strInstallPath = EMDBConfig::GetInstance().get_global_install_path();
#ifdef WIN32
#ifdef _WIN64
    //std::string strLibPath = strInstallPath + "\\Common\\bin64\\userattribute.dll";
    std::string strLibPath = "userattribute.dll";
#else
    std::string strLibPath = strInstallPath + "\\Common\\bin32\\userattribute.dll";
#endif
#else
    std::string strLibPath = strInstallPath + "/Common/bin64/libuserattribute.so";
#endif


    //load library
    void* hModule = CommonFun::LoadShareLibrary(strLibPath.c_str());
    if (hModule==NULL) {
        #ifdef WIN32
        theLog->WriteLog(log_fatal, "load userattribute.dll failed, dwlasterror=0x%x", GetLastError() );
        #else
        char * err = dlerror();
        theLog->WriteLog(log_fatal, "load libuserattribute.so failed, dlerror=%s", err!=NULL ? err : "NULL" );
        #endif 
        return false;
    }

    //get function address
    UserAttrInit = (UserAttrInitFun)CommonFun::GetProcAddress(hModule, "UserAttrInit");
    GetUserAttr = (GetUserAttrFun)CommonFun::GetProcAddress(hModule, "GetUserAttr");
    UserAttrInit_SDK = (UserAttrInit_SDKFun)CommonFun::GetProcAddress(hModule, "UserAttrInit_SDK");
    GetUserAttr_SDK = (GetUserAttr_SDKFun)CommonFun::GetProcAddress(hModule, "GetUserAttr_SDK");
    AddUserAttrValue_SDK = (AddUserAttrValue_SDKFun)CommonFun::GetProcAddress(hModule, "AddUserAttrValue_SDK");
    FreeUserAttr_SDK = (FreeUserAttr_SDKFun)CommonFun::GetProcAddress(hModule, "FreeUserAttr_SDK");

    m_bWell = (UserAttrInit != NULL && GetUserAttr != NULL &&UserAttrInit_SDK != NULL&& GetUserAttr_SDK != NULL&& AddUserAttrValue_SDK != NULL);

    return m_bWell;

}