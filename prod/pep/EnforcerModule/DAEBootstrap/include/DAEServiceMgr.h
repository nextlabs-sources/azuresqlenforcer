#pragma once
#include <Windows.h>
#include <string>

class DAEServiceMgr {
public:
    DAEServiceMgr(const char* exe_name, const char* service_name);
    ~DAEServiceMgr();
    bool OpenSCMgr();
    bool CreateDaeService();
    bool StartDaeService();
    void StopDaeService();
    void CloseHandles();
public:
    bool OpenExe(const std::string& name);
public:
    SC_HANDLE hSCManager = 0;
    SC_HANDLE hSCService = 0;
    const char* _exe_name;
    const char* _service_name;

};


