#pragma once

#ifndef __TDS_PRELOGIN_H__
#define __TDS_PRELOGIN_H__

#include <stdint.h>
#include <vector>
#include "tdsHeader.h"

class IOption
{
public:
    IOption()
        :m_type(-1)
        ,m_offset(0)
        ,m_length(0)
    {}

    virtual ~IOption() {}

    uint8_t GetOptionType() const { return m_type; }
    uint16_t GetOptionOffset() const { return m_offset; }
    uint16_t GetOptionLength() const { return m_length; }

protected:
    uint8_t  m_type;
    uint16_t m_offset;
    uint16_t m_length;
};

// 0x00
class VersionOption : public IOption
{
public:
    VersionOption();
    virtual ~VersionOption();

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t m_version[6];
};

// 0x01
class EncryptOption : public IOption
{
public:
    EncryptOption()
        :m_encrypt(0)
    {}

    virtual ~EncryptOption() {}

    void Parse(uint8_t* p, uint8_t* begin_buff);

    uint8_t GetEncryptFlag() const { return m_encrypt; }

private:
    uint8_t m_encrypt;
};

// 0x02
class InstoptOption : public IOption
{
public: 
    InstoptOption();
    virtual ~InstoptOption();

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t* m_inst;
};

// 0x03
class ThreadidOption : public IOption
{
public:
    ThreadidOption() :m_threadID(0) {}
    virtual ~ThreadidOption() {}

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint32_t m_threadID;
};

// 0x04
class MarsOption : public IOption
{
public:
    MarsOption() :m_mars(0) {}
    virtual ~MarsOption() {}

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t m_mars;
};

// 0x05
class TraceidOption : public IOption
{
public:
    TraceidOption();
    virtual ~TraceidOption();

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t m_traceID[36];
};

// 0x06
class FedAuthRequiredOption : public IOption
{
public:
    FedAuthRequiredOption() :m_fedAuth(0) {}
    virtual ~FedAuthRequiredOption() {}

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t m_fedAuth;
};

// 0x07
class NonceoptOption : public IOption
{
public:
    NonceoptOption() { memset(m_nonce, 0, sizeof(m_nonce)); }
    virtual ~NonceoptOption() {}

    void Parse(uint8_t* p, uint8_t* begin_buff);

private:
    uint8_t m_nonce[32];
};

class TerminatorOption : public IOption
{
public:
    TerminatorOption() { m_type = 0xff; }
    virtual ~TerminatorOption() {}
};

class TdsPreLogin
{
public:
    TdsPreLogin();
    ~TdsPreLogin();

    void Parse(uint8_t* pBuff);

    uint8_t GetEncryptFlag();

private:
    tdsHeader             m_header;
    std::vector<IOption*> m_options;
};

#endif