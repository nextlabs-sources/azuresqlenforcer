#ifndef TDS_HELPER_H
#define TDS_HELPER_H
#include "tdsPacket.h"
#include "tdsHeader.h"

namespace TDS
{
    

    void FillTDSHeader(unsigned char* uBuf,
                        TDS_MSG_TYPE msgType/*1 byte*/,
                        unsigned char Status/*1 byte*/,
                        unsigned short Length/*2 byte, include header*/,
                        unsigned short SPID = 0 /*2 byte*/,
                        unsigned char PacketID = 1/*1 byte*/,
                        unsigned char Window = 0);

    void GetFirstPreloginResponsePacket(unsigned char* szData, unsigned short* length);
    
	BOOL GetCompletePacket(unsigned char* buffer, unsigned short bufLen, unsigned char** tdsPacket, unsigned short* tdsPacketLen);

    unsigned short GetTdsPacketLength(unsigned char* buffer);
    void SetTdsPacketLength(uint8_t* pBuff, uint16_t len);

    TDS_MSG_TYPE GetTdsPacketType(unsigned char* buffer);
    uint8_t GetTdsPacketID(unsigned char* buffer);

    const wchar_t* GetTdsPacketTypeString(unsigned char* p);
    const wchar_t* GetTdsPacketTypeString(unsigned char c);

    BOOL IsEndOfMsg(unsigned char* buffer);
};


#endif