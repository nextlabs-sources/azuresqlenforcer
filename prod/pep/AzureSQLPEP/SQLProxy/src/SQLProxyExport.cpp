#include "SQLProxyExport.h"
#include "ProxyManager.h"
#include "ProxyDataMgr.h"
#include "Log.h"

HMODULE g_hThisModule;

void ProxyLogW(int nLogLevel, const char* file, int line, const wchar_t* logstr)
{
    theLog.WriteLog(nLogLevel, file, line, L"%s", logstr);
}

void ProxyLogA(int nLogLevel, const char* file, int line, const char* logstr)
{
    theLog.WriteLog(nLogLevel, file, line, logstr);
}

void SetInstancePort(uint16_t port)
{
    ProxyManager::GetInstance()->SetInstancePort(port);
}

void Init()
{
	ProxyManager::GetInstance()->Init();
}

void ServerStartEvent(boost::shared_ptr<TcpSocket> tcpSocket)
{
	//convert boost::shared_ptr<TcpSocket> to void* because we did't want other file to include "frame.h"
	//we don't need to call the member of TcpSocket class. so just take it is a void*
	ProxyManager::GetInstance()->ServerStartEvent(tcpSocket);
}

void EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{
	ProxyManager::GetInstance()->EndEvent(tcpSocket, error);
}

void ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length)
{
    theProxyDataMgr->ReceiveDataEvent(tcpSocket, data, length);
}

void SendDataCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{

}

void ConnectCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{

}

uint32_t TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len)
{
    return theProxyDataMgr->TdsGetCurrentPacketLength(pBuff, len);
}

BOOL APIENTRY DllMain( HMODULE hModule,  
					  DWORD  ul_reason_for_call,  
					  LPVOID lpReserved  
					  )  
{  
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hThisModule = hModule;
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}

	return TRUE;
}  