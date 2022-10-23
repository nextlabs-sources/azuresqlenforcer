#ifndef TDS_SQL_BATCH_H
#define TDS_SQL_BATCH_H
#include "tdsAllHeaders.h"
#include "tdsPacket.h"
#include "SMP.h"
#include <string>
#include <list>

class tdsSqlBatch
{
public:
	tdsSqlBatch(uint32_t sz);
	~tdsSqlBatch(void);

public:
    void Parse(uint8_t* pBuff);
	const std::wstring& GetSQLText() { return m_wsSQLText; }
    void SetSQLText(const std::wstring& sql);
    void SetSQLText(const wchar_t* sql);

    void SetSmpLastRequestSeqnum(uint32_t seqnum) { m_smp_last_seqnum = seqnum; }
    void SetSmpLastRequestWndw(uint32_t wndw) { m_smp_last_wndw = wndw; }

    /* Estimate packet size for one packet which contains one sql text. It may larger than 4096 */
    inline uint32_t EstimateNewSize(const std::wstring& strNewQuery);

    /* Estimate packet size for one of multi packets which contain one sql text. It also may larger than 4096 */
    inline uint32_t EstimateFragmentSize(uint32_t dwSQLTextLen);

    /* Serialize buff for one packet or multi packets which contain one sql text. Every packet should smaller than 4096 */
    void Serialize(std::list<uint8_t*>& packs);

private:
    bool            m_has_smp;
    bool            m_readHeader;
    uint32_t        m_smp_last_seqnum;
    uint32_t        m_smp_last_wndw;
    uint32_t        m_maxPacketSz;
    SMPHeader       m_smp;
    tdsHeader       m_pkHeader;
	tdsAllHeaders   m_pkALLHeader;
	std::wstring    m_wsSQLText;
};

#endif 

