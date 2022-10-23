#include <windows.h>
#include <WinSock2.h>
#include "tdsSqlBatch.h"
#include "tdsPacket.h"
#include "TDSException.h"
#include "Log.h"

tdsSqlBatch::tdsSqlBatch(uint32_t sz)
    :m_maxPacketSz(sz)
    ,m_readHeader(false)
    ,m_has_smp(false)
{
}

tdsSqlBatch::~tdsSqlBatch(void)
{
}

void tdsSqlBatch::SetSQLText(const std::wstring& sql)
{
    if (sql.empty()) {
        LOGPRINT(CELOG_EMERG, L"SQL batch new sql is empty!!!! It will use original sql!");
        return;
    }

    m_wsSQLText = sql;
}

void tdsSqlBatch::SetSQLText(const wchar_t* sql)
{
    if (NULL == sql || !sql[0]) {
        LOGPRINT(CELOG_EMERG, L"SQL batch new sql is empty!!!! It will use original sql!");
        return;
    }

    m_wsSQLText = sql;
}

void tdsSqlBatch::Parse(uint8_t* pBuff)
{
    if (pBuff == NULL) return;

    uint8_t* tds_buff = pBuff;
    if (pBuff[0] == SMP_PACKET_FLAG) {
        m_has_smp = true;
        tds_buff = pBuff + SMP_HEADER_LEN;
    }

    tdsPacketHeader* pHeader = (tdsPacketHeader*)tds_buff;
    if (pHeader->PacketID == 1 && !m_readHeader) {
        if (m_has_smp)
            m_smp.Parse(pBuff);

        m_pkHeader.Parse(tds_buff);
        m_pkALLHeader.Parse(tds_buff + TDS_HEADER_LEN);

        if (m_pkHeader.GetPacketLength() > m_maxPacketSz)
            ThrowTdsException("Batch parse, packet length too long, len: %d, max_len: %d", m_pkHeader.GetPacketLength(), m_maxPacketSz);

        uint8_t* sqlBegin = tds_buff + TDS_HEADER_LEN + m_pkALLHeader.GetTotalLength();
        uint8_t* sqlEnd = tds_buff + m_pkHeader.GetPacketLength();
        m_wsSQLText.append((wchar_t*)sqlBegin, (wchar_t*)sqlEnd);

        m_readHeader = true;
    }
    else {
        tdsHeader hd;
        hd.Parse(tds_buff);

        if (m_pkHeader.GetPacketLength() > m_maxPacketSz)
            ThrowTdsException("Batch parse, packet length too long, len: %d, max_len: %d", hd.GetPacketLength(), m_maxPacketSz);

        if (hd.GetStatusIgnore() && hd.IsEndOfMsg())
            m_pkHeader.SetStatusIgnore(true);

        uint8_t* sqlBegin = tds_buff + TDS_HEADER_LEN;
        uint8_t* sqlEnd = tds_buff + hd.GetPacketLength();
        m_wsSQLText.append((wchar_t*)sqlBegin, (wchar_t*)sqlEnd);
    }
}

inline uint32_t tdsSqlBatch::EstimateNewSize(const std::wstring& strNewQuery)
{
    uint32_t dwLen = 0;
    // Packet header size
    dwLen += sizeof(tdsPacketHeader);

    // Batch all header
    dwLen += m_pkALLHeader.GetTotalLength();

    // SQL text len
    dwLen = dwLen + (uint32_t)strNewQuery.length() * sizeof(wchar_t);
    return dwLen;
}

inline uint32_t tdsSqlBatch::EstimateFragmentSize(uint32_t dwSQLTextLen)
{
    return sizeof(tdsPacketHeader) + dwSQLTextLen;
}

void tdsSqlBatch::Serialize(std::list<uint8_t*>& packs)
{
    if (m_wsSQLText.empty())
        return;

    DWORD dwEstimateSize = EstimateNewSize(m_wsSQLText);

    if (dwEstimateSize > m_maxPacketSz) {
        DWORD dwNeed = dwEstimateSize;
        DWORD dwSQLTextLen = m_wsSQLText.length() * sizeof(wchar_t);
        uint32_t smp_seqnum = m_smp_last_seqnum + 1;
        BYTE btPackID = 1;
        bool bPackStart = true;
        while (dwNeed > sizeof(tdsPacketHeader))
        {
            uint8_t* buff = NULL;
            uint32_t maloc_len = 0;
            uint32_t tds_len = 0;
            if (dwNeed > m_maxPacketSz) {
                maloc_len = m_maxPacketSz;
                tds_len = m_maxPacketSz;
            }
            else {
                maloc_len = dwNeed;
                tds_len = dwNeed;
            }

            if (m_has_smp)
                maloc_len += SMP_HEADER_LEN;

            buff = new uint8_t[maloc_len];
            uint8_t* tds_buff = buff;
            if (m_has_smp) {
                SMP::SerializeSMPHeader(buff, m_smp.m_flags, m_smp.m_sid, maloc_len, smp_seqnum++, m_smp_last_wndw);
                tds_buff = buff + SMP_HEADER_LEN;
            }

            m_pkHeader.Serialize(tds_buff);
            if (bPackStart)
            {
                bPackStart = false;
                tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
                pHead->PacketLength = htons((USHORT)tds_len);
                pHead->PacketID = btPackID++;
                if (dwNeed > m_maxPacketSz) {
                    if (pHead->PacketStatus & 0x01)
                        pHead->PacketStatus = 0x04;
                }
                else {
                    pHead->PacketStatus = 0x01;
                    if (m_pkHeader.GetStatusIgnore())
                        pHead->PacketStatus |= TDS_STATUS_IGNORE;
                }

                if (m_pkHeader.GetStatusResetConn())
                    pHead->PacketStatus |= TDS_STATUS_RESET_CONNECTION;
                else if (m_pkHeader.GetStatusResetConnKeepTrans())
                    pHead->PacketStatus |= TDS_STATUS_RESET_CONNECTION_KEEP_TRANSACTION;

                m_pkALLHeader.Serialize(tds_buff + TDS_HEADER_LEN);

                memcpy(tds_buff + TDS_HEADER_LEN + m_pkALLHeader.GetTotalLength(),
                    ((PBYTE)m_wsSQLText.c_str()) + (m_wsSQLText.length() * sizeof(wchar_t) - dwSQLTextLen),
                    tds_len - TDS_HEADER_LEN - m_pkALLHeader.GetTotalLength());

                dwSQLTextLen = dwSQLTextLen - (tds_len - TDS_HEADER_LEN - m_pkALLHeader.GetTotalLength());
            }
            else
            {
                tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
                pHead->PacketLength = htons((USHORT)tds_len);
                pHead->PacketID = btPackID++;
                if (dwNeed > m_maxPacketSz) {
                    if (pHead->PacketStatus & 0x01)
                        pHead->PacketStatus = 0x04;
                }
                else {
                    pHead->PacketStatus = 0x01;
                    if (m_pkHeader.GetStatusIgnore())
                        pHead->PacketStatus |= TDS_STATUS_IGNORE;
                }

                memcpy(tds_buff + TDS_HEADER_LEN,
                    ((PBYTE)m_wsSQLText.c_str()) + (m_wsSQLText.length() * sizeof(wchar_t) - dwSQLTextLen),
                    tds_len - TDS_HEADER_LEN);

                dwSQLTextLen = dwSQLTextLen - (tds_len - TDS_HEADER_LEN);
            }

            packs.push_back(buff);

            dwNeed = EstimateFragmentSize(dwSQLTextLen);
        }
    }
    else {
        uint32_t maloc_len = dwEstimateSize;
        if (m_has_smp)
            maloc_len += SMP_HEADER_LEN;

        PBYTE buff = new BYTE[maloc_len];
        uint8_t* tds_buff = buff;
        if (m_has_smp) {
            SMP::SerializeSMPHeader(buff, m_smp.m_flags, m_smp.m_sid, maloc_len, m_smp_last_seqnum + 1, m_smp_last_wndw);
            tds_buff = buff + SMP_HEADER_LEN;
        }

        m_pkHeader.Serialize(tds_buff);
        tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
        pHead->PacketLength = htons((USHORT)dwEstimateSize);
        pHead->PacketStatus = 0x01;
        pHead->PacketID = 1;

        if (m_pkHeader.GetStatusIgnore())
            pHead->PacketStatus |= TDS_STATUS_IGNORE;

        if (m_pkHeader.GetStatusResetConn())
            pHead->PacketStatus |= TDS_STATUS_RESET_CONNECTION;
        else if (m_pkHeader.GetStatusResetConnKeepTrans())
            pHead->PacketStatus |= TDS_STATUS_RESET_CONNECTION_KEEP_TRANSACTION;

        m_pkALLHeader.Serialize(tds_buff + TDS_HEADER_LEN);

        memcpy(tds_buff + TDS_HEADER_LEN + m_pkALLHeader.GetTotalLength(),
            (PBYTE)m_wsSQLText.c_str(),
            m_wsSQLText.length() * sizeof(wchar_t));
        
        packs.push_back(buff);
    }
}