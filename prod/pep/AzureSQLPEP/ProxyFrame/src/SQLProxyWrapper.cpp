#include <windows.h>
#include "SQLProxyWrapper.h"

SQLProxyWrapper* SQLProxyWrapper::m_pThis = nullptr;

SQLProxyWrapper* SQLProxyWrapper::GetInstance()
{
    if (m_pThis == nullptr)
    {
        m_pThis = new SQLProxyWrapper;
        m_pThis->Load();
    }

    return m_pThis;
}

void SQLProxyWrapper::Release()
{
    if (m_pThis)
    {
        delete m_pThis;
        m_pThis = nullptr;
    }
}

SQLProxyWrapper::SQLProxyWrapper()
{
    m_SQLProxyInitF = nullptr;
    m_ServerStartEventF = nullptr;
    m_EndEventF = nullptr;
    m_ReceiveDataEventF = nullptr;
    m_SendDataCompleteEventF = nullptr;
    m_ConnectCompleteEventF = nullptr;
    m_SetInstancePortF = nullptr;
    m_TdsGetCurrentPacketLengthF = nullptr;
}

SQLProxyWrapper::~SQLProxyWrapper()
{

}

void SQLProxyWrapper::Load()
{
    WCHAR module_path[300] = { 0 };
    GetModuleFileNameW(NULL, module_path, 300);
    WCHAR* p = wcsrchr(module_path, L'\\');
    if (p) *p = 0;
    wcscat_s(module_path, L"\\SQLProxy.dll");

    HMODULE hModule = LoadLibraryW(module_path);
    if (hModule)
    {
        m_SQLProxyInitF = (InitFunc)GetProcAddress(hModule, "Init");
        m_ServerStartEventF = (ServerStartEventFunc)GetProcAddress(hModule, "ServerStartEvent");
        m_EndEventF = (EndEventFunc)GetProcAddress(hModule, "EndEvent");
        m_ReceiveDataEventF = (ReceiveDataEventFunc)GetProcAddress(hModule, "ReceiveDataEvent");
        m_SendDataCompleteEventF = (SendDataCompleteEventFunc)GetProcAddress(hModule, "SendDataCompleteEvent");
        m_ConnectCompleteEventF = (ConnectCompleteEventFunc)GetProcAddress(hModule, "ConnectCompleteEvent");
        m_SetInstancePortF = (SetInstancePortFunc)GetProcAddress(hModule, "SetInstancePort");
        m_TdsGetCurrentPacketLengthF = (TdsGetCurrentPacketLengthFunc)GetProcAddress(hModule, "TdsGetCurrentPacketLength");

        if (m_SQLProxyInitF == nullptr ||
            m_ServerStartEventF == nullptr ||
            m_EndEventF == nullptr ||
            m_ReceiveDataEventF == nullptr ||
            m_SendDataCompleteEventF == nullptr ||
            m_ConnectCompleteEventF == nullptr ||
            m_SetInstancePortF == nullptr ||
            m_TdsGetCurrentPacketLengthF == nullptr)
        {
            OutputDebugStringW(L"Get function address from SQLProxy.dll failed!\n");
            return;
        }
    }
    else
    {
        OutputDebugStringW(L"Load SQLProxy.dll failed!\n");
    }
}

void SQLProxyWrapper::SQLProxyInit()
{
    if (m_SQLProxyInitF)
        m_SQLProxyInitF();
}

void SQLProxyWrapper::ServerStartEvent(boost::shared_ptr<TcpSocket> tcpSocket)
{
    if (m_ServerStartEventF)
        m_ServerStartEventF(tcpSocket);
}

void SQLProxyWrapper::EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{
    if (m_EndEventF)
        m_EndEventF(tcpSocket, error);
}

void SQLProxyWrapper::ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, uint8_t* data, int length)
{
    if (m_ReceiveDataEventF)
        m_ReceiveDataEventF(tcpSocket, data, length);
}

void SQLProxyWrapper::SendDataCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{
    if (m_SendDataCompleteEventF)
        m_SendDataCompleteEventF(tcpSocket, error);
}

void SQLProxyWrapper::ConnectCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error)
{
    if (m_ConnectCompleteEventF)
        m_ConnectCompleteEventF(tcpSocket, error);
}

void SQLProxyWrapper::SetInstancePort(uint16_t port)
{
    if (m_SetInstancePortF)
        m_SetInstancePortF(port);
}

uint32_t SQLProxyWrapper::TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len)
{
    if (m_TdsGetCurrentPacketLengthF)
        return m_TdsGetCurrentPacketLengthF(pBuff, len);

    return 0;
}