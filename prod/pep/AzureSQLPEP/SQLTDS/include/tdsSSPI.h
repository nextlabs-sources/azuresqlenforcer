#pragma once

#ifndef __TDS_SSPI_LOGIN_H__
#define __TDS_SSPI_LOGIN_H__

#include <stdint.h>
#include "tdsPacket.h"

extern const uint8_t NTLMSSP_identifier[8];

class tdsSSPI
{
public:
    tdsSSPI();
    ~tdsSSPI();

    void Parse(uint8_t* pBuff);
    inline const wchar_t* GetDomain() const { return m_domain_str ? m_domain_str : L""; }
    inline const wchar_t* GetUsername() const { return m_username_str ? m_username_str : L""; }
    inline const wchar_t* GetHostname() const { return m_hostname_str ? m_hostname_str : L""; }
    inline const uint8_t* GetSessionKey() const { return m_sessionkey_buff; }
    inline const uint16_t GetSessionKeyLen() const { return m_sessionkey_len; }

private:
    tdsHeader   m_Header;

    uint32_t    m_msgType;
    uint16_t    m_lanMgrResp_len;
    uint16_t    m_lanMgrResp_maxlen;
    uint32_t    m_lanMgrResp_offset;
    uint16_t    m_NTMLResp_len;
    uint16_t    m_NTMLResp_maxlen;
    uint32_t    m_NTMLResp_offset;
    uint16_t    m_domain_len;
    uint16_t    m_domain_maxlen;
    uint32_t    m_domain_offset;
    wchar_t*    m_domain_str;
    uint16_t    m_username_len;
    uint16_t    m_username_maxlen;
    uint32_t    m_username_offset;
    wchar_t*    m_username_str;
    uint16_t    m_hostname_len;
    uint16_t    m_hostname_maxlen;
    uint32_t    m_hostname_offset;
    wchar_t*    m_hostname_str;
    uint16_t    m_sessionkey_len;
    uint16_t    m_sessionkey_maxlen;
    uint32_t    m_sessionkey_offset;
    uint8_t*    m_sessionkey_buff;
};

#endif