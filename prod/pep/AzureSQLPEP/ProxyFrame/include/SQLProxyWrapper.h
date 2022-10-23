#pragma once

#ifndef __SQL_PROXY_WRAPPER_H__
#define __SQL_PROXY_WRAPPER_H__

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

class TcpSocket;

typedef void(*InitFunc)();
typedef void(*ServerStartEventFunc)(boost::shared_ptr<TcpSocket> tcpSocket);
typedef void(*EndEventFunc)(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
typedef void(*ReceiveDataEventFunc)(boost::shared_ptr<TcpSocket> tcpSocket, uint8_t* data, int length);
typedef void(*SendDataCompleteEventFunc)(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
typedef void(*ConnectCompleteEventFunc)(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
typedef void(*SetInstancePortFunc)(uint16_t port);
typedef uint32_t (*TdsGetCurrentPacketLengthFunc)(uint8_t* pBuff, uint32_t len);

class SQLProxyWrapper
{
public:
    static SQLProxyWrapper* GetInstance();
    static void Release();

    void SQLProxyInit();
    void ServerStartEvent(boost::shared_ptr<TcpSocket> tcpSocket);
    void EndEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
    void ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, uint8_t* data, int length);
    void SendDataCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
    void ConnectCompleteEvent(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);
    void SetInstancePort(uint16_t port);
    uint32_t TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len);

private:
    SQLProxyWrapper();
    ~SQLProxyWrapper();

    void Load();

private:
    static SQLProxyWrapper* m_pThis;

    InitFunc                m_SQLProxyInitF;
    ServerStartEventFunc    m_ServerStartEventF;
    EndEventFunc            m_EndEventF;
    ReceiveDataEventFunc    m_ReceiveDataEventF;
    SendDataCompleteEventFunc m_SendDataCompleteEventF;
    ConnectCompleteEventFunc  m_ConnectCompleteEventF;
    SetInstancePortFunc     m_SetInstancePortF;
    TdsGetCurrentPacketLengthFunc   m_TdsGetCurrentPacketLengthF;
};

#endif