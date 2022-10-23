#include "SMP.h"

namespace SMP
{

uint32_t GetSMPPacketLen(const uint8_t* buff)
{
    if (buff && buff[0] == SMP_PACKET_FLAG)
    {
        return *(uint32_t*)(buff + 4);
    }

    return 0;
}

uint8_t GetSMPFlag(const uint8_t* buff)
{
    if (buff[0] != SMP_PACKET_FLAG)
        return 0;

    return *(buff + 1);
}

uint16_t GetSMPSid(const uint8_t* buff)
{
    if (buff[0] != SMP_PACKET_FLAG)
        return -1;

    return *(uint16_t*)(buff + 2);
}

uint32_t GetSMPSeqnum(const uint8_t* buff)
{
    if (buff[0] != SMP_PACKET_FLAG)
        return 0;

    return *(uint32_t*)(buff + 8);
}

uint32_t GetSMPWndw(const uint8_t* buff)
{
    if (buff[0] != SMP_PACKET_FLAG)
        return 0;

    return *(uint32_t*)(buff + 12);
}

void SerializeSMPHeader(uint8_t* buff, uint8_t flags, uint16_t sid, uint32_t length, uint32_t seqnum, uint32_t wndw)
{
    uint8_t* p = buff;
    *p = SMP_PACKET_FLAG; p++;
    *p = flags; p++;
    *(uint16_t*)p = sid; p += 2;
    *(uint32_t*)p = length; p += 4;
    *(uint32_t*)p = seqnum; p += 4;
    *(uint32_t*)p = wndw;
}

}

void SMPHeader::Parse(const uint8_t* buff)
{
    if (buff[0] != SMP_PACKET_FLAG)
        return;

    const uint8_t* p = buff;

    m_smid = *p; p++;
    m_flags = *p; p++;
    m_sid = *(uint16_t*)p; p += 2;
    m_length = *(uint32_t*)p; p += 4;
    m_seqnum = *(uint32_t*)p; p += 4;
    m_wndw = *(uint32_t*)p; p += 4;
}