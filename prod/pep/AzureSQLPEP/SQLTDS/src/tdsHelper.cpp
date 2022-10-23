#include <windows.h>
#include <WinSock2.h>
#include "tdsHelper.h"

namespace TDS
{
    void FillTDSHeader(unsigned char* uBuf,
                        TDS_MSG_TYPE msgType/*1 byte*/,
                        unsigned char Status/*1 byte*/,
                        unsigned short Length/*2 byte, include header*/,
                        unsigned short SPID/*2 byte*/,
                        unsigned char PacketID/*1 byte*/,
                        unsigned char Window)
    {
        uBuf[0] = msgType;
        uBuf[1] = Status;
        *(unsigned short*)(&uBuf[2]) = htons(Length);
        *(unsigned short*)(&uBuf[4]) = SPID;
        uBuf[6] = PacketID;
        uBuf[7] = Window;
    }

    void GetFirstPreloginResponsePacket(unsigned char* szData, unsigned short* length)
    {
        const static unsigned char uPreLoginData[] = { 0x00, 0x00, 0x24, 0x00, 0x06, 0x01, 0x00, 0x2A,
            0x00, 0x01, 0x02, 0x00, 0x2B, 0x00, 0x01, 0x03,
            0x00, 0x2C, 0x00, 0x00, 0x04, 0x00, 0x2C, 0x00,
            0x01, 0x05, 0x00, 0x2D, 0x00, 0x00, 0x06, 0x00,
            0x2D, 0x00, 0x01, 0xFF, 0x0C, 0x00, 0x01, 0xF4,
            0x00, 0x00, 0x01, 0x00, 0x00, 0x01 };

        TDS::FillTDSHeader(szData, TDS_RESPONSE, 1, TDS_HEADER_LEN + sizeof(uPreLoginData));

        //pack
        memcpy(szData + TDS_HEADER_LEN, uPreLoginData, sizeof(uPreLoginData));

        *length = TDS_HEADER_LEN + sizeof(uPreLoginData);

        return;
    }
    
	BOOL GetCompletePacket(unsigned char* buffer, unsigned short bufLen, unsigned char** tdsPacket, unsigned short* tdsPacketLen)
    {
        if (bufLen < TDS_HEADER_LEN)//the ATTENTION have no data
        {
            return FALSE;
        }

        unsigned short tdsLen = TDS::GetTdsPacketLength(buffer);
        if (bufLen < tdsLen)
        {
            return FALSE;
        }
        else
        {//have complete tds packet
            *tdsPacket = buffer;
            *tdsPacketLen = tdsLen;
            return TRUE;
        }
    }

    unsigned short GetTdsPacketLength(unsigned char* buffer)
    {
        return ntohs(*(unsigned short*)(buffer + TDS_HEADER_LEN_OFFSET));
    }

    void SetTdsPacketLength(uint8_t* pBuff, uint16_t len)
    {
        tdsPacketHeader* pPack = (tdsPacketHeader*)pBuff;
        pPack->PacketLength = htons(len);
    }

    TDS_MSG_TYPE GetTdsPacketType(unsigned char* buffer)
    {
        switch (buffer[0])
        {
        case TDS_SQLBATCH:
        case TDS_PRELOGIN_OLD:
        case TDS_RPC:
        case TDS_RESPONSE:
        case TDS_ATTENTION_SIGNAL:
        case TDS_BULK_LOAD_DATA:
        case TDS_FEDAUTH:
        case TDS_Transaction_manager_request:
        case TDS_LOGIN7:
        case TDS_SSPI_LOGIN7:
        case TDS_PRELOGIN:
            return (TDS_MSG_TYPE)buffer[0];
            break;

        default:
            return TDS_Unkown;
        }
    }

    uint8_t GetTdsPacketID(unsigned char* buffer)
    {
        return buffer[6];
    }

    const wchar_t* GetTdsPacketTypeString(unsigned char* p)
    {
        if (p == NULL) return L"";

        if (*p == 0 || *p > 18) return L"";

        return TDS_MSG_TYPE_STRING[*p];
    }

    const wchar_t* GetTdsPacketTypeString(unsigned char c)
    {
        if (c == 0 || c > 18) return L"";

        return TDS_MSG_TYPE_STRING[c];
    }

    BOOL IsEndOfMsg(unsigned char* buffer)
    {
        BYTE status = buffer[1];
        return (status&TDS_STATUS_EOM);
    }
}