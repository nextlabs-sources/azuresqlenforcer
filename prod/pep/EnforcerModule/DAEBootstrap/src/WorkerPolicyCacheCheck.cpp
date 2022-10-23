#include <windows.h>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include "WorkerPolicyCacheCheck.h"
#include "logger_class.h"
#include "EMDBConfig.h"
#include "CommFun.h"
#include "INIReader.h"

using daebootstrap::Logger;

void ReadSubProcThread(std::string worker, HANDLE hr, HANDLE hw, HANDLE hp, bool* result)
{
    Logger::Instance().Info("wait for worker %s CC check compelete.....", worker.c_str());

    std::string output;
   
    std::thread t([](std::string* str, HANDLE h){

        char buff[1024];
        DWORD read_bytes = 0;
        while (1)
        {
            if (!ReadFile(h, buff, 1024, &read_bytes, NULL)) {
                if (read_bytes > 0)
                    str->append(buff, buff + read_bytes);

                break;
            }

            if (read_bytes > 0)
                str->append(buff, buff + read_bytes);
        }
    }, &output, hr);
    t.detach();

    while (1)
    {
        DWORD exit_code = 0;

        ::Sleep(1000);

        GetExitCodeProcess(hp, &exit_code);
        if (exit_code != STILL_ACTIVE) {
            CloseHandle(hp);
            CloseHandle(hw);
            CloseHandle(hr);
            break;
        }
    }

    if (output.empty()) {
        if (result)
            *result = true;
    }
    else {
        if (result)
            *result = false;
        Logger::Instance().Error(output);
    }
}

bool CreateWorkerSubProcess(const std::string& worker)
{
    char proc[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, proc, MAX_PATH);
    char* p = strrchr(proc, '\\');

    std::string dir;
    dir.append(&proc[0], p);

    SECURITY_ATTRIBUTES sa = {0};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE read_handle = NULL;
    HANDLE write_handle = NULL;
    if (!CreatePipe(&read_handle, &write_handle, &sa, 0)) {
        Logger::Instance().Error("create pipe for worker %s sub process failed! err: %d", worker.c_str(), GetLastError());
        return false;
    }

    SetHandleInformation(read_handle, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = {0};
    si.cb = sizeof(STARTUPINFOA);
    si.hStdInput = NULL;
    si.hStdOutput = write_handle;
    si.hStdError = write_handle;
    si.dwFlags = STARTF_USESTDHANDLES;
    PROCESS_INFORMATION pi = {0};

    std::string cmd = "-sub -w " + worker;
    BOOL b = CreateProcessA(proc, const_cast<char*>(cmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, const_cast<char*>(dir.c_str()), &si, &pi);
    if (!b)
    {
        Logger::Instance().Error("Create worker sub process failed! err: %d", GetLastError());
        return false;
    }

    CloseHandle(pi.hThread);

    bool result = false;
    ReadSubProcThread(worker, read_handle, write_handle, pi.hProcess, &result);

    if (result)
        Logger::Instance().Info("CC policy cache for worker %s is ok\n", worker.c_str());
    else
        Logger::Instance().Info("There are something wrong in CC policy cache for worker %s\n", worker.c_str());

    return result;
}

bool WorkerPolicyCacheCheck()
{
    std::string conf_path = CommonFun::GetConfigFilePath();

    try
    {
        std::vector<std::string> workers;
        std::string workerstring;
        INIReader reader(conf_path);

        workerstring = reader.GetString("TDSProxyWorkers", "workers", "");
        if (workerstring.empty())
        {
            Logger::Instance().Error("'Workers' in section 'TDSProxyWorkers' can not be empty!");
            return false;
        }

        boost::split(workers, workerstring, boost::is_any_of(","), boost::token_compress_on);

        for (std::string s : workers)
        {
            boost::trim(s);
            if (s.empty())
                continue;

            auto it = boost::find_if(s, boost::is_any_of("[].*\\/:?\"<>|"));
            if (it != s.end())
            {
                Logger::Instance().Error("Worker name can not contain these character '*[].\\/:?\"<>|'");
                return false;
            }

            if (!CreateWorkerSubProcess(s))
                return false;
        }
    }
    catch (std::exception e)
    {
        Logger::Instance().Error("read config %s exception: %s", conf_path.c_str(), e.what());
        return false;
    }
}