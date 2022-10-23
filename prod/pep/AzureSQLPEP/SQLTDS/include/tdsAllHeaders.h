#ifndef TDS_ALL_HEADERS
#define TDS_ALL_HEADERS

#include <stdint.h>
#include <vector>

enum tdsAllHeaderType
{
    emUnknown = 0,
    emQNHeader = 1, // Query Notifications Header
    emTDHeader = 2, // Transaction Descriptor Header
    emTAHeader = 3, // Trace Activity Header
};

class tdsAllHeaderNode
{
public:
    tdsAllHeaderNode()
        :m_length(0)
        ,m_type(0)
    {}
    virtual ~tdsAllHeaderNode() {}

    virtual uint32_t GetLength() { return m_length; }
    virtual void SetLength(uint32_t l) { m_length = l; }
    virtual uint16_t GetType() { return m_type; }
    virtual void SetType(uint16_t t) { m_type = t; }

    virtual void Parse(uint8_t* pBuff) = 0;

protected:
    uint32_t    m_length;
    uint16_t    m_type;
};

// Query Notifications Header
class tdsQNHeader : public tdsAllHeaderNode
{
public:
    tdsQNHeader();
    virtual ~tdsQNHeader();

    virtual void Parse(uint8_t* pBuff);

    uint16_t GetNotifyLength() { return m_NotifyLength; }
    const wchar_t* GetNotifyStream() { return m_NotifyStream; }
    uint16_t GetSSBDeploymentLength() { return m_SSBDeploymentLength; }
    const wchar_t* GetSSBDeploymentStream() { return m_SSBDeploymentStream; }
    uint32_t GetNotifyTimeout() { return m_NotifyTimeout; }

protected:
    uint16_t    m_NotifyLength;
    wchar_t*    m_NotifyStream;
    uint16_t    m_SSBDeploymentLength;
    wchar_t*    m_SSBDeploymentStream;
    uint32_t    m_NotifyTimeout;        //millisecond
};

// Transaction Descriptor Header
class tdsTDHeader : public tdsAllHeaderNode
{
public:
    tdsTDHeader();
    virtual ~tdsTDHeader() {}

    virtual void Parse(uint8_t* pBuff);

    uint32_t GetOutstandingRequestCount() { return m_OutstandingRequestCount; }
    uint64_t GetTransactionDescriptor() { return m_TransactionDescriptor; }

protected:
    uint32_t    m_OutstandingRequestCount;
    uint64_t    m_TransactionDescriptor;
};

// Trace Activity Header
class tdsTAHeader : public tdsAllHeaderNode
{
public:
    tdsTAHeader();
    virtual ~tdsTAHeader() {}

    virtual void Parse(uint8_t* pBuff);

    const uint8_t* GetActivityID() { return &GUID_ActivityID[0]; }
    uint32_t GetActivitySequence() { return ActivitySequence; }

protected:
    uint8_t     GUID_ActivityID[16];
    uint32_t    ActivitySequence;
};

class tdsAllHeaders
{
public:
	tdsAllHeaders();
	~tdsAllHeaders(void);

public:
	uint32_t GetTotalLength() { return dwTotalLength; }
	bool Parse(uint8_t* pData);
    void Serialize(uint8_t* pData);

protected:
	uint32_t dwTotalLength;  //0 means have no headers
    std::vector<tdsAllHeaderNode*> nodes;
};

#endif


