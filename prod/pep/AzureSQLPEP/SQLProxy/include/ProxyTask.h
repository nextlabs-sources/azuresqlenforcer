#pragma once

#ifndef PROXY_TASK_H
#define PROXY_TASK_H

#include <windows.h>
#include <list>
#include <stdint.h>
#include "ProxyChannel.h"
#include "tdsPacket.h"

class ProxyTask
{
public:
    ProxyTask(void* tcpsocket);
    ~ProxyTask();

    void Execute();
    void AddedTdsPacket(PBYTE pTdsPacket);
    void SetChannel(ProxyChannelPtr pChannel) { m_ProxyChannel = pChannel; }
    ProxyChannelPtr GetChannel() { return m_ProxyChannel; }

    void SetCompleteTask(BOOL bComp) { m_bCompleteTask = bComp; }
    BOOL IsCompleteTask() { return m_bCompleteTask; }
    
    BOOL IsServerMessage() { return m_IsServerMsg; }
    void SetIsServerMessage(bool b) { m_IsServerMsg = b; }
    
    // send original packets list or new packets list according to this flag.
    void SetUseNewListTdsPacket(BOOL isUseNewListTdsPacket) { m_bUseNewListTdsPacket = isUseNewListTdsPacket; }
    std::list<uint8_t*>& GetTdsPacketList() { return m_bUseNewListTdsPacket ? m_lstTdsPacketNew : m_lstTdsPacket; }
    
    // the packets need to send according to this flag.
    BOOL GetDispatchReady() const { return m_bDispatchReady; }
    void SetDispatchReady(BOOL isReady) { m_bDispatchReady = isReady; }

    uint32_t GetSessionID() const { return m_session_id; }
    void SetSessionID(uint32_t sid) { m_session_id = sid; }

    bool SetReq(boost::shared_ptr<ProxyTask> req)
    {
        if (IsServerMessage())
        {
            m_TdsTaskReq = req;
            return true;
        }
        return false;
    }
    boost::shared_ptr<ProxyTask> GetReq()
    {
        if (IsServerMessage())
        {
            return m_TdsTaskReq;
        }
        return nullptr;
    }

    TDS_MSG_TYPE GetTdsPacketType() const;

private:
    void ProcessClientMessage();
    void ProcessServerMessage();

    void ProcessLogin7Request();
    void ProcessFedauthRequest();
    void ProcessSSPIRequest();

    void ProcessServerResponse_TDS_LOGIN7();

    SECURITY_STATUS TLSHandShakeWithClient();
    SECURITY_STATUS TLSHandShakeWithServer(BOOL bInit);

    BOOL CheckClientKrbTicket(std::vector<BYTE>& data, PCtxtHandle pctx);
    BOOL CreateKrbAuthTicketForServer(std::vector<BYTE>& data, PCtxtHandle pctx);

private:
    void*               m_tcpSocket;
    bool                m_IsServerMsg;
    ProxyChannelPtr     m_ProxyChannel;
    std::list<uint8_t*> m_lstTdsPacket;
    std::list<uint8_t*> m_lstTdsPacketNew;
    BOOL                m_bCompleteTask; //mark it is a complete/incomplete tds task
    BOOL                m_bUseNewListTdsPacket;
    BOOL                m_bDispatchReady;
    boost::shared_ptr<ProxyTask> m_TdsTaskReq;
    uint32_t            m_session_id;
};

#endif