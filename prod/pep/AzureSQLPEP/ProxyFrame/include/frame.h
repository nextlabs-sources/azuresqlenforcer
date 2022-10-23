// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FRAME_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FRAME_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FRAME_EXPORTS
#define FRAME_API __declspec(dllexport)
#else
#define FRAME_API __declspec(dllimport)
#endif

#include "TcpSocket.h"
#include <windows.h>
#include <stdint.h>
#include <string>


#ifndef DEFINE_SERVICE_PARAMS
typedef struct ServiceParams {

    uint16_t     instance_port;
    std::wstring worker_name;

} InitParam, *PInitParam;
#define DEFINE_SERVICE_PARAMS
#endif

extern "C" 
{
// FOR TCP
FRAME_API void EntryPoint(PInitParam param);
FRAME_API bool Connect(const char* ip, const char* port, /*HMODULE hModule,*/ void* data);
FRAME_API bool BlockConnect(const char* ip, const char* port, /*HMODULE hModule,*/ boost::shared_ptr<TcpSocket>& tcpSocket, boost::system::error_code& error);
FRAME_API void SendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length);
FRAME_API void BlockSendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length, boost::system::error_code& error);
FRAME_API void Close(boost::shared_ptr<TcpSocket> tcpSocket);
}

