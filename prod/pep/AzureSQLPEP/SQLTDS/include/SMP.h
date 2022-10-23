#pragma once

#ifndef __SMP_H__
#define __SMP_H__

#include <stdint.h>

#define SMP_PACKET_FLAG 0x53
#define SMP_HEADER_LEN  16

#define SMP_SYN     0x01
#define SMP_ACK     0x02
#define SMP_FIN     0x04
#define SMP_DATA    0x08

namespace SMP
{
    uint32_t GetSMPPacketLen(const uint8_t* buff);

    uint8_t GetSMPFlag(const uint8_t* buff);

    uint16_t GetSMPSid(const uint8_t* buff);

    uint32_t GetSMPSeqnum(const uint8_t* buff);

    uint32_t GetSMPWndw(const uint8_t* buff);

    void SerializeSMPHeader(uint8_t* buff, uint8_t flags, uint16_t sid, uint32_t length, uint32_t seqnum, uint32_t wndw);
}

class SMPHeader
{
public:
    SMPHeader()
        :m_smid(SMP_PACKET_FLAG)
        ,m_flags(0)
        ,m_sid(0)
        ,m_length(0)
        ,m_seqnum(0)
        ,m_wndw(0)
    {}
    ~SMPHeader() {}

    void Parse(const uint8_t* buff);

    uint8_t     m_smid;
    uint8_t     m_flags;
    uint16_t    m_sid;
    uint32_t    m_length;
    uint32_t    m_seqnum;
    uint32_t    m_wndw;
};

#endif