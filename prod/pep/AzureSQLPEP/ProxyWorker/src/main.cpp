#include "frame.h"
#include <windows.h>
#include <stdlib.h>
#include <string>

#include "INIReader.h"
#include "CommonFunc.h"

void DebugLog(const char* fmt, ...)
{
    char msg[1024] = { 0 };

    va_list args;
    va_start(args, fmt);
     _vsnprintf(msg, _countof(msg) - 1, fmt, args);
    va_end(args);

    OutputDebugStringA(msg);
}

void DebugLog(const wchar_t* fmt, ...)
{
    wchar_t msg[1024] = { 0 };

    va_list args;
    va_start(args, fmt);
    _vsnwprintf(msg, _countof(msg) - 1, fmt, args);
    va_end(args);

    OutputDebugStringW(msg);
}

void ParseArgument(DWORD argc, LPWSTR *argv, PInitParam param)
{
    for (DWORD i = 0; i < argc; i++)
    {
        if (_wcsicmp(argv[i], L"-w") == 0)
        {
            i++;
            if (i < argc)
            {
                if (argv[i])
                    param->worker_name = argv[i];
            }
        }
        else if (_wcsicmp(argv[i], L"-p") == 0)
        {
            i++;
            if (i < argc)
            {
                if (argv[i])
                    param->instance_port = (uint16_t)_wtoi(argv[i]);
            }
        }
    }
}

bool IsWorkerConfigExist(const std::wstring& worker)
{
    char config_path[300] = { 0 };
    GetModuleFileNameA(NULL, config_path, 300);
    char* p = strrchr(config_path, '\\');
    if (p) *p = 0;
    strcat_s(config_path, "\\config\\config.ini");

    try
    {
        INIReader reader(config_path);
        std::string remote_server = reader.GetString(ProxyCommon::WStringToString(worker).c_str(), "remote_server", "");
        return !remote_server.empty();
    }
    catch (std::exception e)
    {
        DebugLog("[TDSProxyWorker] read config.ini exception: %s\n", e.what());
    }

    return false;
}

void LoadProxyFrame(InitParam& param)
{
    char config_path[300] = { 0 };
    GetModuleFileNameA(NULL, config_path, 300);
    char* p = strrchr(config_path, '\\');
    if (p) *p = 0;
    strcat_s(config_path, "\\frame.dll");

    HMODULE hmod = LoadLibraryA(config_path);
    if (hmod == NULL) {
        DebugLog("[TDSProxyWorker] Load frame.dll failed!\n");
        return;
    }

    void (*pfEntryPoint)(PInitParam param) = NULL;
    pfEntryPoint = (void(*)(PInitParam param))GetProcAddress(hmod, "EntryPoint");
    if (pfEntryPoint == NULL) {
        DebugLog("[TDSProxyWorker] get EntryPoint from frame.dll failed!\n");
        return;
    }

    pfEntryPoint(&param);
}

int wmain(DWORD argc, LPWSTR *argv)
{
    InitParam param;
    param.instance_port = 0;

    ParseArgument(argc, argv, &param);

    if (param.worker_name.empty()) {
        DebugLog(L"[TDSProxyWorker] worker name must not be empty!\n");
        return 0;
    }

    if (!IsWorkerConfigExist(param.worker_name)) {
        DebugLog(L"[TDSProxyWorker] specify work name: %s is not exist\n", param.worker_name.c_str());
        return 0;
    }

    LoadProxyFrame(param);

    return 0;
}