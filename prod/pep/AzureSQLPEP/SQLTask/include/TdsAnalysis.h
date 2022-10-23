#ifndef TDS_ANALYSIS_H
#define TDS_ANALYSIS_H

#include <windows.h>
#include <list>
#include <stdint.h>
#include "SQLTaskExports.h"

class TdsAnalysis
{
public:
    TdsAnalysis();
	~TdsAnalysis(void);

public:
    void ProcessClientMessage(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets);
    void ProcessServerMessage(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets, uint8_t requestType);
    void SetMaxPacketSize(uint32_t sz) { m_maxPackSize = sz; }
    uint32_t GetMaxPacketSize() { return m_maxPackSize; }
    void SetEnforceParams(EnforceParams* p) { m_params = p; };

private:
    void ProcessClientBatch(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets);
    void ProcessClientRPC(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets);
    void DoSQLEnforce(const std::wstring& oriSqlText, std::wstring& newSqlText);

private:
    uint32_t m_maxPackSize;
    EnforceParams* m_params;
};

#endif 

