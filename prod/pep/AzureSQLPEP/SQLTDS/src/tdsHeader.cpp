#include <WinSock2.h>
#include "tdsHeader.h"


tdsHeader::tdsHeader(uint8_t* pData)
{
    m_status_ignore = false;
    m_status_reset_conn = false;
    m_status_reset_conn_keep_trans = false;

	Parse(pData);
}

tdsHeader::tdsHeader(void)
{
    m_status_ignore = false;
    m_status_reset_conn = false;
    m_status_reset_conn_keep_trans = false;

	PacketLength = 0;
}

tdsHeader::~tdsHeader(void)
{
}

void tdsHeader::Parse(uint8_t* pData)
{
    tdsPacketHeader* pPack = (tdsPacketHeader*)pData;
    this->PacketType    = pPack->PacketType;
    this->PacketStatus  = pPack->PacketStatus;
    this->PacketLength  = ntohs(pPack->PacketLength);
    this->SPID          = ntohs(pPack->SPID);
    this->PacketID      = pPack->PacketID;
    this->Window        = pPack->Window;

    if (this->PacketStatus & TDS_STATUS_IGNORE) {
        m_status_ignore = true;
    }
    if (this->PacketStatus & TDS_STATUS_RESET_CONNECTION) {
        m_status_reset_conn = true;
    }
    if (this->PacketStatus & TDS_STATUS_RESET_CONNECTION_KEEP_TRANSACTION) {
        m_status_reset_conn_keep_trans = true;
    }
}

void tdsHeader::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(PacketType);
	rBuf.ReadAny(PacketStatus);
	rBuf.ReadAny(PacketLength);
    PacketLength = ntohs(PacketLength);
	rBuf.ReadAny(SPID);
	SPID = ntohs(SPID);
	rBuf.ReadAny(PacketID);
	rBuf.ReadAny(Window);

    if (this->PacketStatus & TDS_STATUS_IGNORE) {
        m_status_ignore = true;
    }
    if (this->PacketStatus & TDS_STATUS_RESET_CONNECTION) {
        m_status_reset_conn = true;
    }
    if (this->PacketStatus & TDS_STATUS_RESET_CONNECTION_KEEP_TRANSACTION) {
        m_status_reset_conn_keep_trans = true;
    }
}

void tdsHeader::Serialize(uint8_t* uBuf)
{
    tdsPacketHeader* pPack = (tdsPacketHeader*)uBuf;
    pPack->PacketType   = this->PacketType;
    pPack->PacketStatus = this->PacketStatus;
    pPack->PacketLength = htons(this->PacketLength);
    pPack->SPID         = htons(this->SPID);
    pPack->PacketID     = this->PacketID;
    pPack->Window       = this->Window;
}
