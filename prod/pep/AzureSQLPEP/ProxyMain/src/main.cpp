// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "main.h"
#include "UdpSocket.h"
#include "INIReader.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>

struct WorkerParam;
struct InstanceQueryResponse;

SERVICE_STATUS_HANDLE g_service_handle = NULL;
int g_guard_elapsed_time = 0;
HANDLE g_guard_thread = NULL;
HANDLE g_udp_query_thread = NULL;
std::vector<WorkerParam> g_workers;
std::vector<InstanceQueryResponse> g_inst_resp;

struct WorkerParam
{
    std::string name;
    std::string remote_server;
    std::string instance;
    uint16_t    local_port;
    HANDLE      process_handle;
};

struct InstanceQueryResponse
{
    uint16_t    id;
    std::string server_name;
    std::string instance_name;
    std::string is_clustered;
    std::string version;
    uint16_t    instance_port;
};

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

size_t UdpSend(boost::asio::mutable_buffer& recv_buff, const char* ep, const char* port, uint8_t* data, uint32_t len, boost::system::error_code& ec)
{
    if (nullptr == ep || 0 == port || nullptr == data || len == 0)
        return 0;

    boost::asio::io_service query_service;
    boost::asio::ip::udp::resolver resolver(query_service);
    boost::asio::ip::udp::resolver::query query(ep, port);
    boost::asio::ip::udp::resolver::iterator remote_ep_iter = resolver.resolve(query, ec);
    boost::asio::ip::udp::resolver::iterator iter_end;

    boost::asio::io_service io_service;
    boost::asio::ip::udp::endpoint local_endp(boost::asio::ip::udp::v4(), 0);

    UdpSocket udpsock(io_service, local_endp);

    for (; remote_ep_iter != iter_end; remote_ep_iter++)
    {
        udpsock.Send(boost::asio::buffer(data, len), remote_ep_iter->endpoint(), ec);
        if (!ec)
        {
            size_t recv_len = udpsock.Receive(boost::asio::buffer(recv_buff), boost::posix_time::seconds(10), ec);
            if (!ec)
            {
                return recv_len;
            }
        }
    }

    return 0;
}

bool QueryInstancePort(const std::string& instance, const std::string& server, InstanceQueryResponse& resp, boost::system::error_code& ec)
{
    bool query_result = false;
    std::vector<std::string> params;
    boost::array<uint8_t, 2048> query_buff;
    uint8_t* query_data = new uint8_t[instance.length() + 1];
    query_data[0] = 0x04;
    memcpy(query_data + 1, instance.c_str(), instance.length());

    try
    {
        size_t len = UdpSend(boost::asio::buffer(query_buff), server.c_str(), "1434", query_data, instance.length() + 1, ec);
        if (!ec && len > 3)
        {
            uint8_t* p = query_buff.data();
            if (p[0] == 0x05)
            {
                resp.id = *(uint16_t*)(p + 1);

                std::string str;
                str.append((char*)(p + 3), (char*)(p + len));

                boost::split(params, str, boost::algorithm::is_any_of(";"), boost::token_compress_on);

                for (size_t i = 0; i < params.size(); i++)
                {
                    if (_stricmp(params[i].c_str(), "ServerName") == 0) {
                        resp.server_name = params[i + 1];
                    }
                    else if (_stricmp(params[i].c_str(), "InstanceName") == 0) {
                        resp.instance_name = params[i + 1];
                    }
                    else if (_stricmp(params[i].c_str(), "IsClustered") == 0) {
                        resp.is_clustered = params[i + 1];
                    }
                    else if (_stricmp(params[i].c_str(), "Version") == 0) {
                        resp.version = params[i + 1];
                    }
                    else if (_stricmp(params[i].c_str(), "tcp") == 0) {
                        resp.instance_port = (uint16_t)(uint32_t)atoi(params[i + 1].c_str());
                    }
                }

                query_result = true;

                DebugLog("[TDSProxyMain] instance: %s  port: %d\n", instance.c_str(), resp.instance_port);
            }
        }
    }
    catch (std::exception e)
    {
        DebugLog("[TDSProxyMain] query port of instance %s exception: %s\n", instance.c_str(), e.what());
        query_result = false;
    }

    delete[] query_data;
    return query_result;
}

DWORD WINAPI ListenUdpQuery(void*)
{
    try
    {
        boost::system::error_code errcode;
        boost::array<uint8_t, 2048> recv_buff;
        boost::asio::io_service io_service;
        boost::asio::ip::udp::endpoint local_endp(boost::asio::ip::udp::v4(), 1434);
        boost::asio::ip::udp::socket udpsock(io_service, local_endp);

        while (1)
        {
            boost::asio::ip::udp::endpoint remote_endp;
            size_t len = udpsock.receive_from(boost::asio::buffer(recv_buff), remote_endp, 0, errcode);
            if (!errcode)
            {
                if (recv_buff.front() != 0x4)
                    continue;

                uint8_t* p = recv_buff.data();

                std::string str;
                str.append((char*)(p + 1), (char*)(p + len));

                for each (const WorkerParam& param in g_workers)
                {
                    if (_stricmp(param.instance.c_str(), str.c_str()) == 0)
                    {
                        for each (const InstanceQueryResponse& resp in g_inst_resp)
                        {
                            if (_stricmp(str.c_str(), resp.instance_name.c_str()) == 0)
                            {
                                std::string respstring =
                                    "ServerName;" + resp.server_name +
                                    ";InstanceName;" + resp.instance_name +
                                    ";IsClustered;" + resp.is_clustered +
                                    ";Version;" + resp.version +
                                    ";tcp;" + std::to_string(param.local_port) + ";;";

                                uint8_t* resp_data = new uint8_t[respstring.length() + 3];
                                resp_data[0] = 0x05;
                                *(uint16_t*)(resp_data + 1) = resp.id;
                                memcpy(resp_data + 3, respstring.c_str(), respstring.length());

                                udpsock.send_to(boost::asio::buffer(resp_data, respstring.length() + 3), remote_endp, 0, errcode);

                                delete[] resp_data;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    catch (std::exception e)
    {
        DebugLog("[TDSProxyMain] ListenUdpQuery thread exception: %s\n", e.what());

        CloseHandle(g_udp_query_thread);
        g_udp_query_thread = CreateThread(NULL, 0, ListenUdpQuery, NULL, 0, NULL);
        return 0;
    }

    CloseHandle(g_udp_query_thread);
    return 0;
}

HANDLE CreateWorkerProcess(const std::string& worker_name, uint16_t inst_port)
{
    DebugLog("[TDSProxyMain] Try to create process of worker %s\n", worker_name.c_str());

    char strWorkDir[MAX_PATH] = { 0 };
    char strModule[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, strWorkDir, MAX_PATH);
    char* p = strrchr(strWorkDir, L'\\');
    if (p) *p = 0;

    strcpy_s(strModule, strWorkDir);
    strcat_s(strModule, "\\ProxyWorker.exe");

    std::string cmd = "-w \"" + worker_name + "\" -p " + std::to_string(inst_port);

    STARTUPINFOA si;
    memset(&si, 0, sizeof(STARTUPINFOA));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));

    BOOL bCreate = CreateProcessA(strModule, (char*)cmd.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NO_WINDOW, NULL, strWorkDir, &si, &pi);
    if (!bCreate) {
        DebugLog("[TDSProxyMain] create worker %s process failed!, err: %d\n", worker_name.c_str(), GetLastError());
        return NULL;
    }

    DebugLog("[TDSProxyMain] start worker %s process [%d]", worker_name.c_str(), pi.dwProcessId);
    CloseHandle(pi.hThread);
    return pi.hProcess;
}

DWORD WINAPI GuardThread(void*)
{
    DebugLog("[TDSProxyMain] start Guard Thread\n");

    while (1)
    {
        Sleep(g_guard_elapsed_time * 1000);

        for (size_t i = 0; i < g_workers.size(); i++)
        {
            DWORD dwCode = 0;
            if (g_workers[i].process_handle)
            {
                GetExitCodeProcess(g_workers[i].process_handle, &dwCode);
            }

            if ((dwCode != STILL_ACTIVE && dwCode != 0) || g_workers[i].process_handle == NULL)
            {
                CloseHandle(g_workers[i].process_handle);

                uint16_t inst_port = 0;
                if (!g_workers[i].instance.empty())
                {
                    for each (const InstanceQueryResponse& resp in g_inst_resp)
                    {
                        if (_stricmp(resp.instance_name.c_str(), g_workers[i].instance.c_str()) == 0)
                        {
                            inst_port = resp.instance_port;
                            break;
                        }
                    }
                }

                g_workers[i].process_handle = CreateWorkerProcess(g_workers[i].name, inst_port);
            }
        }
    }

    return 0;
}

void StartWorkers()
{
    char strModule[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, strModule, MAX_PATH);
    char* p = strrchr(strModule, L'\\');
    if (p) *p = 0;
    strcat_s(strModule, "\\config\\config.ini");

    try
    {
        std::string workers;
        INIReader reader(strModule);
        if (reader.ParseError() == -1)
        {
            DebugLog("[TDSProxyMain] config.ini open failed!");
            return;
        }
        else if (reader.ParseError() != 0)
        {
            DebugLog("[TDSProxyMain] config.ini parse failed!");
            return;
        }

        workers = reader.GetString("TDSProxyWorkers", "workers", "");
        g_guard_elapsed_time = reader.GetInteger("TDSProxyWorkers", "guard_elapsed_time", 5);

        std::vector<std::string> names;
        boost::split(names, workers, boost::is_any_of(","), boost::token_compress_on);

        if (workers.empty() || names.size() == 0) {
            DebugLog("[TDSProxyMain] there is no workers\n");
            return;
        }

        for (std::string s : names)
        {
            boost::trim(s);
            if (s.empty())
                continue;

            WorkerParam param;
            param.name = s;
            param.local_port = 0;
            param.process_handle = NULL;

            param.remote_server = reader.GetString(s.c_str(), "remote_server", "");
            param.instance = reader.GetString(s.c_str(), "remote_server_instance", "");
            param.local_port = (uint16_t)(uint32_t)reader.GetInteger(s.c_str(), "local_proxy_port", 0);

            if (!param.remote_server.empty() && param.local_port != 0) {
                if (param.instance.empty()) {
                    param.process_handle = CreateWorkerProcess(s, 0);
                }
                else {
                    boost::system::error_code err;
                    InstanceQueryResponse resp;
                    resp.id = 0;
                    resp.instance_port = 0;

                    if (QueryInstancePort(param.instance, param.remote_server, resp, err) && !err)
                    {
                        g_inst_resp.push_back(resp);
                        param.process_handle = CreateWorkerProcess(s, resp.instance_port);
                    }
                    else {
                        DebugLog("[TDSProxyMain] query port of instance %s failed, %s\n", param.instance.c_str(), err.message().c_str());
                    }
                }
                g_workers.push_back(param);
            }
        }

        g_guard_thread = CreateThread(NULL, 0, GuardThread, NULL, 0, NULL);
        g_udp_query_thread = CreateThread(NULL, 0, ListenUdpQuery, NULL, 0, NULL);
    }
    catch (std::exception e)
    {
        DebugLog("[TDSProxyMain] start workers exception: %s\n", e.what());
    }
}

void SetRestartOnFailure()
{
    SC_HANDLE hsc = OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_ALL_ACCESS);
    if (hsc == NULL)
    {
        OutputDebugStringW(L"[ProxyMain] OpenSCManager failed!\n");
        return;
    }

    SC_HANDLE hservice = OpenServiceW(hsc, L"DAE for SQL Server Service", SC_MANAGER_ALL_ACCESS);
    if (hservice == NULL)
    {
        CloseServiceHandle(hsc);
        OutputDebugStringW(L"[ProxyMain] OpenService [DAE for SQL Server Service] failed!\n");
        return;
    }

    SC_ACTION actions[1] = { SC_ACTION_RESTART , 5000 };
    SERVICE_FAILURE_ACTIONSW failure_action = {0};

    failure_action.dwResetPeriod = 1800; // 30min
    failure_action.cActions = 1;
    failure_action.lpsaActions = (SC_ACTION*)&actions;

    if (!ChangeServiceConfig2W(hservice, SERVICE_CONFIG_FAILURE_ACTIONS, &failure_action))
    {
        OutputDebugStringW(L"[ProxyMain] ChangeServiceConfig2W failed!\n");
    }

    CloseServiceHandle(hservice);
    CloseServiceHandle(hsc);
}

void setServiceStatus(DWORD status)
{
    SERVICE_STATUS serviceStatus;
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwWaitHint = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
    serviceStatus.dwCurrentState = status;

    SetServiceStatus(g_service_handle, &serviceStatus);
}

VOID WINAPI ServiceHandler(DWORD controlCode)
{
    switch (controlCode)
    {
    case SERVICE_CONTROL_PAUSE:
        setServiceStatus(SERVICE_PAUSED);           
        break;

    case SERVICE_CONTROL_SHUTDOWN:
    case SERVICE_CONTROL_STOP:
        {
            if (g_udp_query_thread) {
                TerminateThread(g_udp_query_thread, 0);
                CloseHandle(g_udp_query_thread);
                g_udp_query_thread = NULL;
            }

            if (g_guard_thread) {
                TerminateThread(g_guard_thread, 0);
                CloseHandle(g_guard_thread);
                g_guard_thread = NULL;
            }

            for each (const WorkerParam& worker in g_workers)
            {
                if (worker.process_handle) {
                    TerminateProcess(worker.process_handle, 0);
                    CloseHandle(worker.process_handle);
                }
            }
            g_workers.clear();
        }

        setServiceStatus(SERVICE_STOPPED);          
        break;
    default:
        break;
    }
}

int ConsoleMain(DWORD argc, LPWSTR *argv)
{
    StartWorkers();
	
    getchar();
	return 0;
}

VOID WINAPI ServiceMain(DWORD argc, LPWSTR *argv)
{
	g_service_handle = RegisterServiceCtrlHandlerW(L"", &ServiceHandler);
	if (g_service_handle == 0)
	{
		OutputDebugStringW(L"RegisterServiceCtrlHandlerW failed\n");
		exit(-1);
	}

	OutputDebugStringW(L"RegisterServiceCtrlHandlerW successfully\n");

	setServiceStatus(SERVICE_RUNNING);

    StartWorkers();
}

const wchar_t* GetModeFromArgument(DWORD argc, LPWSTR *argv)
{
	wchar_t* szMode = L"service";
	for (DWORD i=0; i<argc; i++)
	{
		if (wcsicmp(argv[i], L"-mode") == 0)
		{
			i++;
			if (i<argc)
			{
			    szMode = argv[i];
			}
		}
	}

	return szMode;
}

int wmain(DWORD argc, LPWSTR *argv)
{
	const wchar_t* wszMode = GetModeFromArgument(argc, argv);

	//get mode from argument
	if (wcsicmp(wszMode, L"console")==0)
	{
		OutputDebugStringW(L"Proxymain run as console mode.\n");
		ConsoleMain(argc, argv);
	}
	else
	{
		OutputDebugStringW(L"Proxymain run as serice mode.\n");

		const SERVICE_TABLE_ENTRYW serviceTable[] = {
			{ L"NxlSqlEnforcer", ServiceMain },
			{ NULL, NULL }
		};

		if (!StartServiceCtrlDispatcherW(&serviceTable[0]))
		{
			OutputDebugStringW(L"StartServiceCtrlDispatcherW failed\n");
			return 0;
		}

		OutputDebugStringW(L"StartServiceCtrlDispatcherW successfully\n");
	}

	return 0;
}