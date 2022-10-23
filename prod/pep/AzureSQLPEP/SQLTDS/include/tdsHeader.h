#ifndef TDS_HEADER_H
#define TDS_HEADER_H
#include <windows.h>
#include <stdint.h>
#include "MyBuffer.h"

#define TDS_STATUS_EOM  0x01    // end of package
#define TDS_STATUS_IGNORE 0x02  // ignore this package
#define TDS_STATUS_EVENT_NOTIFY 0x04
#define TDS_STATUS_RESET_CONNECTION 0x08  // 
#define TDS_STATUS_RESET_CONNECTION_KEEP_TRANSACTION 0x10

#pragma pack(push, 1)
struct tdsPacketHeader
{
    uint8_t PacketType;
    uint8_t PacketStatus;
    uint16_t PacketLength;
    uint16_t SPID;
    uint8_t PacketID;
    uint8_t Window;
};
#pragma pack(pop)

class tdsHeader : protected tdsPacketHeader
{
public:
	tdsHeader(uint8_t* pData);
	tdsHeader(void);
	~tdsHeader(void);
	uint8_t GetTdsPacketType() const { return PacketType; }
	uint8_t GetStatus() const { return PacketStatus; }
	void SetStatus(uint8_t s) { PacketStatus = s; }
	void SetLength(uint16_t l) { PacketLength = l; }
    uint16_t GetPacketLength() const { return PacketLength; }
	void SetPacketID(uint8_t id) { PacketID = id; }
    uint8_t GetPacketID() const { return PacketID; }

	void Parse(uint8_t* pBuf);
	void Parse(ReadBuffer& rBuf);
	
	bool IsEndOfMsg() const { return (PacketStatus & TDS_STATUS_EOM); }
	void Serialize(uint8_t* pBuf);

    void SetStatusIgnore(bool b) { m_status_ignore = b; }
    void SetStatusResetConn(bool b) { m_status_reset_conn = b; }
    void SetStatusResetConnKeepTrans(bool b) { m_status_reset_conn_keep_trans = b; }
    bool GetStatusIgnore() const { return m_status_ignore; }
    bool GetStatusResetConn() const { return m_status_reset_conn; }
    bool GetStatusResetConnKeepTrans() const { return m_status_reset_conn_keep_trans; }

private:
    bool m_status_ignore;
    bool m_status_reset_conn;
    bool m_status_reset_conn_keep_trans;
};

#endif //TDS_HEADER_H

