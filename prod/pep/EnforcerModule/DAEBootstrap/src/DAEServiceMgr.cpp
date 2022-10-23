
#include "DAEServiceMgr.h"
#include "Commfun.h"
#include <ShellAPI.h>

DAEServiceMgr::DAEServiceMgr(const char* exe_name, const char* service_name):_exe_name(exe_name),_service_name(service_name){
    
}

DAEServiceMgr::~DAEServiceMgr()
{
    CloseHandles();
}


bool DAEServiceMgr::OpenSCMgr() {
	hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL) {
		return false;
	}
	return true;
}

bool DAEServiceMgr::CreateDaeService() {
	std::string folder = CommonFun::GetProgramDataFolder();
	folder += _exe_name;


    hSCService = ::CreateService(hSCManager,
        _service_name,
        _service_name,
        SC_MANAGER_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        folder.c_str(),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    if (hSCService == NULL) {
        return false;
    }

    do{ //set description
        SERVICE_DESCRIPTION sd;
        sd.lpDescription = TEXT(const_cast<char*>(_service_name));
        ::ChangeServiceConfig2(hSCService, SERVICE_CONFIG_DESCRIPTION, &sd);
    } while (0);


    do{  //set error recovery 
        const uint32_t restart_times = 6;
        SERVICE_FAILURE_ACTIONS sfaction;
        SC_ACTION scactions[restart_times];

        for (uint32_t i = 0; i < restart_times; i++)
        {
            scactions[i].Delay = i<2 ? 1000: 60 * 1000; //first and second  -> 1second other 60second
            scactions[i].Type = SC_ACTION_RESTART; //
        }
        sfaction.dwResetPeriod = 60 * 60 * 24; //1day
        sfaction.lpRebootMsg = NULL;
        sfaction.lpCommand = NULL;
        sfaction.cActions = restart_times;
        sfaction.lpsaActions = scactions;

        ::ChangeServiceConfig2(hSCService, SERVICE_CONFIG_FAILURE_ACTIONS, &sfaction);
    } while (0);


	return true;
}
bool DAEServiceMgr::StartDaeService() {

    if (hSCService != 0) {
        ::CloseServiceHandle(hSCService);
        hSCService = NULL;
    }

    hSCService = ::OpenService(hSCManager, _service_name,
        SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
    if (hSCService == NULL)
    {
        return false;
    }
    // get status
    SERVICE_STATUS status;
    if (::QueryServiceStatus(hSCService, &status) == FALSE)
    {
        ::CloseServiceHandle(hSCService);
        hSCService = NULL;
        return false;
    }

     if (status.dwCurrentState == SERVICE_STOPPED)
    {
        // 
        if (::StartService(hSCService, NULL, NULL) == FALSE)
        {
            ::CloseServiceHandle(hSCService);
            hSCService = NULL;
            return false;
        }
        // 
        while (::QueryServiceStatus(hSCService, &status) == TRUE)
        {
            ::Sleep(status.dwWaitHint);
            if (status.dwCurrentState == SERVICE_RUNNING)
            {
                ::CloseServiceHandle(hSCService);
                hSCService = NULL;
                return true;
            }
        }
     }
     else {
         ::CloseServiceHandle(hSCService);
         hSCService = NULL;
         return true;
     }
     return false;
}

void DAEServiceMgr::StopDaeService() {
    if (hSCService != 0) {
        ::CloseServiceHandle(hSCService);
        hSCService = NULL;
    }

    hSCService = ::OpenService(hSCManager, _service_name,
        SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
    if (hSCService == NULL)
    {
        return;
    }

    // get status
    SERVICE_STATUS status;
    if (::QueryServiceStatus(hSCService, &status) == FALSE)
    {
        ::CloseServiceHandle(hSCService);
        hSCService = NULL;
        return;
    }

    //check status
    if (status.dwCurrentState == SERVICE_RUNNING)
    {
        // 
        if (::ControlService(hSCService,
            SERVICE_CONTROL_STOP, &status) == FALSE)
        {
            ::CloseServiceHandle(hSCService);
            hSCService = NULL;
            return;
        }
        // 
        while (::QueryServiceStatus(hSCService, &status) == TRUE)
        {
            if (status.dwCurrentState == SERVICE_STOPPED)
            {
                ::CloseServiceHandle(hSCService);
                hSCService = NULL;
                return;
            }

            ::Sleep(status.dwWaitHint);
        }
    }
    ::CloseServiceHandle(hSCService);
    hSCService = NULL;


}

void DAEServiceMgr::CloseHandles() {
    if(hSCService != 0)
        ::CloseServiceHandle(hSCService);
    if (hSCManager != 0)
        ::CloseServiceHandle(hSCManager);
}





bool DAEServiceMgr::OpenExe(const std::string& name)
{
    std::string folder = CommonFun::GetProgramDataFolder();
    folder += name;

    SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = __TEXT(folder.c_str());
    ShExecInfo.lpParameters = __TEXT("");//
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;
    return ::ShellExecuteEx(&ShExecInfo);
}