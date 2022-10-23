#pragma once

#ifndef PROXY_DATA_ANALYSIS_H
#define PROXY_DATA_ANALYSIS_H

#include <list>
#include <stdint.h>
#include "SQLTaskExports.h"

class ProxyDataAnalysis
{
public:
    static ProxyDataAnalysis* GetInstance()
    {
        static ProxyDataAnalysis* pInst = nullptr;
        if (pInst == nullptr)
        {
            pInst = new ProxyDataAnalysis;
        }

        return pInst;
    }

    bool Init();
    bool IsInited() { return m_isInit; }

    void ProcessClientPacket(
        const std::list<uint8_t*>& oldPackets,
        std::list<uint8_t*>& newPackets,
        uint32_t maxPackSize,
        EnforceParams& enforceParams);
    
    void ProcessServerPacket(
        const std::list<uint8_t*>& oldPackets,
        std::list<uint8_t*>& newPackets,
        uint8_t requestType,
        uint32_t maxPackSize,
        EnforceParams& enforceParams);
    
private:
    ProxyDataAnalysis();
    ~ProxyDataAnalysis();

    
    pf_ProcessClientPacket  m_processClient;
    pf_ProcessServerPacket  m_processServer;
    pf_init                 m_taskInit;

    bool                    m_isInit;
};

#endif