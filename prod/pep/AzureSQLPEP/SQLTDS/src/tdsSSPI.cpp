#include "tdsSSPI.h"
#include "CommonFunc.h"
#include "Log.h"

const uint8_t NTLMSSP_identifier[8] = { 0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00 };

tdsSSPI::tdsSSPI()
    :m_msgType(0)
    ,m_NTMLResp_len(0)
    ,m_NTMLResp_maxlen(0)
    ,m_NTMLResp_offset(0)
    ,m_lanMgrResp_len(0)
    ,m_lanMgrResp_maxlen(0)
    ,m_lanMgrResp_offset(0)
    ,m_domain_len(0)
    ,m_domain_maxlen(0)
    ,m_domain_offset(0)
    ,m_domain_str(nullptr)
    ,m_username_len(0)
    ,m_username_maxlen(0)
    ,m_username_offset(0)
    ,m_username_str(nullptr)
    ,m_hostname_len(0)
    ,m_hostname_maxlen(0)
    ,m_hostname_offset(0)
    ,m_hostname_str(nullptr)
    ,m_sessionkey_len(0)
    ,m_sessionkey_maxlen(0)
    ,m_sessionkey_offset(0)
    ,m_sessionkey_buff(nullptr)
{

}

tdsSSPI::~tdsSSPI()
{
    if (m_domain_str) {
        delete[] m_domain_str;
        m_domain_str = nullptr;
    }
    if (m_username_str) {
        delete[] m_username_str;
        m_username_str = nullptr;
    }
    if (m_hostname_str) {
        delete[] m_hostname_str;
        m_hostname_str = nullptr;
    }
    if (m_sessionkey_buff) {
        delete[] m_sessionkey_buff;
        m_sessionkey_buff = nullptr;
    }
}

void tdsSSPI::Parse(uint8_t* pBuff)
{
    m_Header.Parse(pBuff);

    const uint8_t* pStart = nullptr;
    const uint8_t* p = pBuff + TDS_HEADER_LEN;
    if (memcmp(p, NTLMSSP_identifier, 8) == 0) {
        p += 8;
        pStart = pBuff + TDS_HEADER_LEN;
    }
    else {
        const uint8_t* pSearch = ProxyCommon::BinarySearch(pBuff + TDS_HEADER_LEN, m_Header.GetPacketLength(), NTLMSSP_identifier, sizeof(NTLMSSP_identifier));
        if (pSearch == nullptr) {
            return;
        }

        p = pSearch + 8;
        pStart = pSearch;
    }

    // type
    m_msgType = *(uint32_t*)p;
    p += 4;

    // lan mgr response
    m_lanMgrResp_len = *(uint16_t*)p;
    p += 2;
    m_lanMgrResp_maxlen = *(uint16_t*)p;
    p += 2;
    m_lanMgrResp_offset = *(uint32_t*)p;
    p += 4;
    
    // NTML respose
    m_NTMLResp_len = *(uint16_t*)p;
    p += 2;
    m_NTMLResp_maxlen = *(uint16_t*)p;
    p += 2;
    m_NTMLResp_offset = *(uint32_t*)p;
    p += 4;

    // domain
    m_domain_len = *(uint16_t*)p;
    p += 2;
    m_domain_maxlen = *(uint16_t*)p;
    p += 2;
    m_domain_offset = *(uint32_t*)p;
    p += 4;
    if (m_domain_len != 0 && m_domain_offset != 0) {
        m_domain_str = (wchar_t*)new uint8_t[m_domain_len + 2];
        memset(m_domain_str, 0, m_domain_len + 2);
        memcpy(m_domain_str, pStart + m_domain_offset, m_domain_len);
    }

    // user name
    m_username_len = *(uint16_t*)p;
    p += 2;
    m_username_maxlen = *(uint16_t*)p;
    p += 2;
    m_username_offset = *(uint32_t*)p;
    p += 4;
    if (m_username_len != 0 && m_username_offset != 0) {
        m_username_str = (wchar_t*)new uint8_t[m_username_len + 2];
        memset(m_username_str, 0, m_username_len + 2);
        memcpy(m_username_str, pStart + m_username_offset, m_username_len);
    }

    // host name
    m_hostname_len = *(uint16_t*)p;
    p += 2;
    m_hostname_maxlen = *(uint16_t*)p;
    p += 2;
    m_hostname_offset = *(uint32_t*)p;
    p += 4;
    if (m_hostname_len != 0 && m_hostname_offset != 0) {
        m_hostname_str = (wchar_t*)new uint8_t[m_hostname_len + 2];
        memset(m_hostname_str, 0, m_hostname_len + 2);
        memcpy(m_hostname_str, pStart + m_hostname_offset, m_hostname_len);
    }

    // session key
    m_sessionkey_len = *(uint16_t*)p;
    p += 2;
    m_sessionkey_maxlen = *(uint16_t*)p;
    p += 2;
    m_sessionkey_offset = *(uint32_t*)p;
    p += 4;
    if (m_sessionkey_len != 0 && m_sessionkey_offset != 0 && m_sessionkey_offset + m_sessionkey_len + TDS_HEADER_LEN <= m_Header.GetPacketLength()) {
        m_sessionkey_buff = new uint8_t[m_sessionkey_len];
        memcpy(m_sessionkey_buff, pStart + m_sessionkey_offset, m_sessionkey_len);
    }
}