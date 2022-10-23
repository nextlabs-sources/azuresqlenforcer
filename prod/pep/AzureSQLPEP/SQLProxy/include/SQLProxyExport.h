#ifndef SQLPROXY_EXPORT_H
#define SQLPROXY_EXPORT_H

#ifdef SQLPROXY_EXPORT
#define SQLPROXY_API __declspec(dllexport)
#else
#define SQLPROXY_API __declspec(dllimport)
#endif

#include <windows.h>
#include <string>

#include "frame.h"

extern "C"
{
	//implement the interface defined by  TCPFrame
	SQLPROXY_API void Init();
	SQLPROXY_API void ServerStartEvent(boost::shared_ptr<TcpSocket> tcpSocket);
	SQLPROXY_API void EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
	SQLPROXY_API void ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length);
	SQLPROXY_API void SendDataCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
	SQLPROXY_API void ConnectCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
    SQLPROXY_API uint32_t TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len);
    SQLPROXY_API void SetInstancePort(uint16_t port);
    SQLPROXY_API void ProxyLogW(int nLogLevel, const char* file, int line, const wchar_t* logstr);
    SQLPROXY_API void ProxyLogA(int nLogLevel, const char* file, int line, const char* logstr);
};

#endif 


