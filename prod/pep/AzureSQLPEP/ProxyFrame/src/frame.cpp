// frame.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <set>
#include <map>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include "frame.h"
#include "FrameServer.h"
#include "SQLProxyWrapper.h"
#include "UdpSocket.h"
#include "INIReader.h"
#include "CommonFunc.h"
#include "Log.h"

using boost::asio::ip::tcp;

void ClientsProc();

boost::asio::io_context* pClientsioContext = nullptr;

std::string g_local_port;
std::string g_listen_ip;

void ReadConfig(const std::wstring& worker_name)
{
    char config_path[300] = { 0 };
    GetModuleFileNameA(NULL, config_path, 300);
    char* p = strrchr(config_path, '\\');
    if (p) *p = 0;
    strcat_s(config_path, "\\config\\config.ini");

    try
    {
        INIReader ini(config_path);
        std::string str_worker = ProxyCommon::UnicodeToUTF8(worker_name);

        g_local_port = ini.GetString(str_worker, "local_proxy_port", "1433");

        g_listen_ip = ini.GetString(str_worker, "local_proxy_listen_ip", "");
        if (g_listen_ip.empty())
            g_listen_ip = ini.GetString("TDSProxyWorkers", "local_proxy_listen_ip", "");

        if (g_listen_ip.empty())
            LOGPRINT(CELOG_WARNING, "local_proxy_listen_ip is empty. proxy will listen on system default ip.");
        else
        {
            boost::regex reg("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
            if (!boost::regex_match(g_listen_ip, reg))
            {
                LOGPRINT(CELOG_WARNING, "local_proxy_listen_ip: %s is not match ip format. proxy will listen on system default ip.", g_listen_ip.c_str());
                g_listen_ip = "";
            }
        }
    }
    catch (std::exception e)
    {
        LOGPRINT(CELOG_ERR, "ProxyFrame read config exception: %s", e.what());
    }
}

FRAME_API void EntryPoint(PInitParam param)
{
    SQLProxyWrapper::GetInstance()->SQLProxyInit();
    SQLProxyWrapper::GetInstance()->SetInstancePort(param->instance_port);

    ReadConfig(param->worker_name);

	pClientsioContext = new boost::asio::io_context;
    boost::thread client_thread(ClientsProc);
    client_thread.detach();

	//https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio/reference/io_context/stop.html
	while (true)
	{
		try
		{
			boost::asio::io_context ioContext;
            boost::asio::ip::tcp::endpoint endp(boost::asio::ip::tcp::v4(), (unsigned short)atoi(g_local_port.c_str()));
            if (!g_listen_ip.empty())
                endp = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(g_listen_ip), (unsigned short)atoi(g_local_port.c_str()));
            
            FrameServer fs(ioContext, endp);

			std::vector<boost::shared_ptr<boost::thread>> threads; 
			for (std::size_t i = 0; i < boost::thread::hardware_concurrency() * 4; i++)
			{
				boost::shared_ptr<boost::thread> t(new boost::thread(boost::bind(&boost::asio::io_service::run, &ioContext)));
				threads.push_back(t);
			}

			ioContext.run();

			for (std::size_t i = 0; i < threads.size(); i++)
			{
				threads[i]->join();
			} 
		}
		catch (std::exception& e)
		{
			e;
		}
	}

	return;
}

FRAME_API void Close(boost::shared_ptr<TcpSocket> tcpSocket)
{
    Sleep(1);
    try
    {
        tcpSocket->socket().cancel();
        tcpSocket->socket().shutdown(boost::asio::socket_base::shutdown_both);
    }
    catch (...) {}

	boost::asio::socket_base::linger lingerOption(true, 10);
	tcpSocket->socket().set_option(lingerOption);
	tcpSocket->socket().close();
}

FRAME_API bool Connect(const char* ip, const char* port, void* data)
{
	boost::asio::io_service io_service;  

	boost::asio::ip::tcp::resolver resolver(io_service);  
	boost::asio::ip::tcp::resolver::query query(ip, port);
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query); 

	auto tcpSocket = boost::make_shared<TcpSocket>(*pClientsioContext);
	tcpSocket->SetData(data);
	tcpSocket->Connect(iterator);
	return true;
}

FRAME_API bool BlockConnect(const char* ip, const char* port, boost::shared_ptr<TcpSocket>& tcpSocket, boost::system::error_code& error)
{
	boost::asio::io_service io_service;  

	boost::asio::ip::tcp::resolver resolver(io_service);  
	boost::asio::ip::tcp::resolver::query query(ip, port);
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query, error);

	boost::shared_ptr<TcpSocket> newTcpSocket(new TcpSocket(*pClientsioContext/*, events*/));
	newTcpSocket->SetData(tcpSocket.get()?tcpSocket->GetData():NULL);
	boost::asio::connect(newTcpSocket->socket(), iterator, error);

	if (!error)  
	{
		tcpSocket = newTcpSocket;
		newTcpSocket->Read();
		return true;
	} 
	else
	{
		return false;
	}
}

FRAME_API void SendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length)
{
	tcpSocket->Write(data, length);
}

FRAME_API void BlockSendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length, boost::system::error_code& error)
{
	tcpSocket->BlockWrite(data, length, error);
}

void ClientsProc()
{
	while (true)
	{
		try
		{
			boost::asio::io_context::work worker(*pClientsioContext); 

			std::vector<boost::shared_ptr<boost::thread>> threads; 
			for (std::size_t i = 0; i < boost::thread::hardware_concurrency() * 4; i++)
			{
				boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, pClientsioContext)));
				threads.push_back(thread);
			}

			pClientsioContext->run();

			for (std::size_t i = 0; i < threads.size(); i++)
			{
				threads[i]->join();
			} 
		}
		catch (std::exception& e)
		{
			e;
		}
	}
}
