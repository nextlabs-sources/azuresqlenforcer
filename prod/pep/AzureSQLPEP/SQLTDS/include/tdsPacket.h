#ifndef TDS_PACKET_H
#define TDS_PACKET_H

#include "tdsHeader.h"

#define  TDS_HEADER_LEN 8
#define  TDS_HEADER_LEN_OFFSET 2

enum TDS_MSG_TYPE
{
    TDS_Unkown       = 0,
	TDS_SQLBATCH     = 0x01,
    TDS_PRELOGIN_OLD = 0x02,
	TDS_RPC          = 0x03,
	TDS_RESPONSE     = 0x04,
    TDS_ATTENTION_SIGNAL = 0x06,
    TDS_BULK_LOAD_DATA = 0x07,
	TDS_FEDAUTH      = 0x08,
    TDS_Transaction_manager_request = 0x0e,
    TDS_LOGIN7       = 0x10,
    TDS_SSPI_LOGIN7  = 0x11,
	TDS_PRELOGIN     = 0x12,
};

enum TDS_TOKEN_TYPE
{
    TDS_TOKEN_UNKNOWN = 0x00,
    TDS_TOKEN_ALTMETADATA = 0x88,
    TDS_TOKEN_ALTROW = 0xD3,
    TDS_TOKEN_COLMETADATA = 0x81,
    TDS_TOKEN_COLINFO = 0xA5,
    TDS_TOKEN_DONE = 0xFD,
    TDS_TOKEN_DONEPROC = 0xFE,
    TDS_TOKEN_DONEINPROC = 0xFF,
    TDS_TOKEN_ENVCHANGE = 0xE3,
    TDS_TOKEN_ERROR = 0xAA,
    TDS_TOKEN_FEATUREEXTACK = 0xAE,// ; (introduced in TDS 7.4)
    TDS_TOKEN_FEDAUTHINFO = 0xEE,// ; (introduced in TDS 7.4)
    TDS_TOKEN_INFO = 0xAB,
    TDS_TOKEN_LOGINACK = 0xAD,
    TDS_TOKEN_NBCROW = 0xD2,// ; (introduced in TDS 7.3)
    TDS_TOKEN_OFFSET = 0x78,
    TDS_TOKEN_ORDER = 0xA9,
    TDS_TOKEN_RETURNSTATUS = 0x79,
    TDS_TOKEN_RETURNVALUE = 0xAC,
    TDS_TOKEN_ROW = 0xD1,
    TDS_TOKEN_SESSIONSTATE = 0xE4,// ; (introduced in TDS 7.4)
    TDS_TOKEN_SSPI = 0xED,
    TDS_TOKEN_TABNAME = 0xA4
};

extern const wchar_t* const TDS_MSG_TYPE_STRING[19];

#define NTOHL(p) ntohl(*(DWORD*)p) 
#define NTOHS(p) ntohs(*(USHORT*)p)
#define HTONL(p) htonl(*(DWORD*)p)
#define HTONS(p) htons(*(USHORT*)p)

class tdsPacket
{
public:
	tdsPacket(PBYTE pData);
	tdsPacket(void);
	virtual ~tdsPacket(void);

	TDS_MSG_TYPE GetTdsPacketType() const { return (TDS_MSG_TYPE)hdr.GetTdsPacketType(); }
public:
	void Parse(PBYTE pBuf);

protected:
    tdsHeader   hdr;
};

#endif //TDS_PACKET_H

