#include <WinSock2.h>
#include "tdsPreLogin.h"
#include "tdsPacket.h"
#include "tdsHelper.h"
#include "Log.h"

VersionOption::VersionOption()
{
    memset(m_version, 0, sizeof(m_version));
}

VersionOption::~VersionOption()
{
    
}

void VersionOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;
    
    if (m_length == 6) {
        memcpy(m_version, begin_buff + m_offset, 6);
    }
}

//////////////////////////////////////////////////////////////////////////
//
void EncryptOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 1)
        m_encrypt = *(begin_buff + m_offset);
}


//////////////////////////////////////////////////////////////////////////
//
InstoptOption::InstoptOption()
    :m_inst(nullptr)
{

}

InstoptOption::~InstoptOption()
{
    if (m_inst) {
        delete[] m_inst;
        m_inst = nullptr;
    }
}

void InstoptOption::Parse(uint8_t* p, uint8_t* packet_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length > 0 && ((TDS_HEADER_LEN + m_offset + m_length) <= TDS::GetTdsPacketLength(packet_buff))) {
        m_inst = new uint8_t[m_length];
        if (m_inst) {
            memcpy(m_inst, packet_buff + TDS_HEADER_LEN + m_offset, m_length);
        }
    }
}


//////////////////////////////////////////////////////////////////////////
//
void ThreadidOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 4)
        m_threadID = ntohl(*(uint32_t*)(begin_buff + m_offset));
}

//////////////////////////////////////////////////////////////////////////
//
void MarsOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 1)
        m_mars = *(begin_buff + m_offset);
}


//////////////////////////////////////////////////////////////////////////
//
TraceidOption::TraceidOption()
{
    memset(m_traceID, 0, sizeof(m_traceID));
}

TraceidOption::~TraceidOption()
{

}

void TraceidOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 36) {
        memcpy(m_traceID, begin_buff + m_offset, 36);
    }
}

//////////////////////////////////////////////////////////////////////////
//
void FedAuthRequiredOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 1)
        m_fedAuth = *(begin_buff + m_offset);
}


//////////////////////////////////////////////////////////////////////////
//
void NonceoptOption::Parse(uint8_t* p, uint8_t* begin_buff)
{
    m_type = *p; p++;
    m_offset = ntohs(*(uint16_t*)p); p += 2;
    m_length = ntohs(*(uint16_t*)p); p += 2;

    if (m_length == 32)
        memcpy(m_nonce, begin_buff + m_offset, 32);
}

//////////////////////////////////////////////////////////////////////////
//
TdsPreLogin::TdsPreLogin()
{

}

TdsPreLogin::~TdsPreLogin()
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        delete m_options[i];
    }
    m_options.clear();
}

uint8_t TdsPreLogin::GetEncryptFlag()
{
    for (size_t i = 0; i < m_options.size(); i++)
    {
        if (m_options[i]->GetOptionType() == 1)
            return ((EncryptOption*)m_options[i])->GetEncryptFlag();
    }
    return 0;
}

void TdsPreLogin::Parse(uint8_t* pBuff)
{
    m_header.Parse(pBuff);

    uint8_t* p = pBuff + TDS_HEADER_LEN;

    bool bTerminator = false;
    while (p - pBuff < m_header.GetPacketLength())
    {
        switch (*p)
        {
        case 0:
        {
            VersionOption* ver = new VersionOption();
            ver->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(ver);
        }
        break;

        case 1:
        {
            EncryptOption* enc = new EncryptOption();
            enc->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(enc);
        }
        break;

        case 2:
        {
            InstoptOption* inst = new InstoptOption();
            inst->Parse(p, pBuff);
            m_options.push_back(inst);
        }
        break;
        
        case 3:
        {
            ThreadidOption* threadid = new ThreadidOption();
            threadid->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(threadid);
        }
        break;

        case 4:
        {
            MarsOption* mars = new MarsOption();
            mars->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(mars);
        }
        break;

        case 5:
        {
            TraceidOption* traceid = new TraceidOption();
            traceid->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(traceid);
        }
        break;

        case 6:
        {
            FedAuthRequiredOption* fed = new FedAuthRequiredOption();
            fed->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(fed);
        }
        break;

        case 7:
        {
            NonceoptOption* nonce = new NonceoptOption();
            nonce->Parse(p, pBuff + TDS_HEADER_LEN);
            m_options.push_back(nonce);
        }
        break;

        case 0xff:
        {
            TerminatorOption* term = new TerminatorOption();
            m_options.push_back(term);
            bTerminator = true;
        }
        break;

        default:
            LOGPRINT(CELOG_WARNING, "Unkown prelogin type: %d", *p);
            return;
        }

        if (bTerminator)
            break;

        p += 5;
    }
}