#include "tdsAllHeaders.h"
#include "Log.h"
#include <stdio.h>

tdsQNHeader::tdsQNHeader()
    :m_NotifyLength(0)
    ,m_NotifyStream(NULL)
    , m_SSBDeploymentLength(0)
    ,m_SSBDeploymentStream(NULL)
    ,m_NotifyTimeout(0)
{

}

tdsQNHeader::~tdsQNHeader()
{
    if (m_NotifyStream) {
        delete[] m_NotifyStream;
        m_NotifyStream = NULL;
    }

    if (m_SSBDeploymentStream) {
        delete[] m_SSBDeploymentStream;
        m_SSBDeploymentStream = NULL;
    }
}

void tdsQNHeader::Parse(uint8_t* pBuff)
{
    uint8_t* p = pBuff;
    
    m_length = *(uint32_t*)p;
    p += 4;
    m_type = *(uint16_t*)p;
    p += 2;
    m_NotifyLength = *(uint16_t*)p;
    p += 2;
    if (m_NotifyLength > 0) {
        m_NotifyStream = (wchar_t*)new uint8_t[m_NotifyLength + 2];
        memset(m_NotifyStream, 0, m_NotifyLength + 2);
        memcpy(m_NotifyStream, p, m_NotifyLength);
        p += m_NotifyLength;
    }
    m_SSBDeploymentLength = *(uint16_t*)p;
    p += 2;
    if (m_SSBDeploymentLength > 0) {
        m_SSBDeploymentStream = (wchar_t*)new uint8_t[m_SSBDeploymentLength + 2];
        memset(m_SSBDeploymentStream, 0, m_SSBDeploymentLength + 2);
        memcpy(m_SSBDeploymentStream, p, m_SSBDeploymentLength);
        p += m_SSBDeploymentLength;
    }
    m_NotifyTimeout = *(uint32_t*)p;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

tdsTDHeader::tdsTDHeader()
    :m_OutstandingRequestCount(0)
    ,m_TransactionDescriptor(0)
{

}

void tdsTDHeader::Parse(uint8_t* pBuff)
{
    uint8_t* p = pBuff;

    m_length = *(uint32_t*)p;
    p += 4;
    m_type = *(uint16_t*)p;
    p += 2;

    m_TransactionDescriptor = *(uint64_t*)p;
    p += 8;
    m_OutstandingRequestCount = *(uint32_t*)p;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
tdsTAHeader::tdsTAHeader()
{
    memset(&GUID_ActivityID[0], 0, sizeof(GUID_ActivityID));
    ActivitySequence = 0;
}

void tdsTAHeader::Parse(uint8_t* pBuff)
{
    uint8_t* p = pBuff;

    m_length = *(uint32_t*)p;
    p += 4;
    m_type = *(uint16_t*)p;
    p += 2;

    memcpy(&GUID_ActivityID[0], p, sizeof(GUID_ActivityID));
    p += sizeof(GUID_ActivityID);

    ActivitySequence = *(uint32_t*)p;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
tdsAllHeaders::tdsAllHeaders()
{
	dwTotalLength = 0;
}


tdsAllHeaders::~tdsAllHeaders(void)
{
    for (auto it : nodes)
    {
        delete it;
    }
    nodes.clear();
}

bool tdsAllHeaders::Parse(uint8_t* pData)
{
    if (pData == NULL)
        return false;

	uint8_t* p = pData;

	//total length
	dwTotalLength = *(uint32_t*)p;
	if (0==dwTotalLength)
	{
		return true;
	}
	p += sizeof(dwTotalLength);

	//
	while (p - pData < dwTotalLength)
	{
        tdsAllHeaderNode* header = nullptr;
		uint16_t htype = *(uint16_t*)(p+4);
		if (htype == emQNHeader)
		{
            header = new tdsQNHeader;
		}
		else if (htype == emTAHeader)
		{
            header = new tdsTAHeader;
		}
		else if (htype == emTDHeader)
		{
            header = new tdsTDHeader;
		}
		else
		{
			//error
            LOGPRINT(CELOG_WARNING, L"Parse all headers failed. Unknown header type!!!! ---- %d", htype);
            dwTotalLength = 0;
			break;
			//return FALSE;
		}

        if (header) {
            header->Parse(p);
            p += header->GetLength();

            nodes.push_back(header);
        }
	}
	
	return true;
}

void tdsAllHeaders::Serialize(uint8_t* pData)
{
    if (pData == NULL || dwTotalLength == 0)
        return;

    uint8_t* p = pData;
    *(uint32_t*)p = dwTotalLength;

    p += 4;

    for (auto header : nodes)
    {
        *(uint32_t*)p = header->GetLength();
        p += 4;
        *(uint16_t*)p = header->GetType();
        p += 2;

        if (header->GetType() == emQNHeader)
        {
            *(uint16_t*)p = ((tdsQNHeader*)header)->GetNotifyLength();
            p += 2;
            if (((tdsQNHeader*)header)->GetNotifyLength() > 0) {
                memcpy(p, ((tdsQNHeader*)header)->GetNotifyStream(), ((tdsQNHeader*)header)->GetNotifyLength());
                p += ((tdsQNHeader*)header)->GetNotifyLength();
            }

            *(uint16_t*)p = ((tdsQNHeader*)header)->GetSSBDeploymentLength();
            p += 2;
            if (((tdsQNHeader*)header)->GetSSBDeploymentLength() > 0) {
                memcpy(p, ((tdsQNHeader*)header)->GetSSBDeploymentStream(), ((tdsQNHeader*)header)->GetSSBDeploymentLength());
                p += ((tdsQNHeader*)header)->GetSSBDeploymentLength();
            }

            *(uint32_t*)p = ((tdsQNHeader*)header)->GetNotifyTimeout();
            p += 4;
        }
        else if (header->GetType() == emTAHeader)
        {
            memcpy(p, ((tdsTAHeader*)header)->GetActivityID(), 16);
            p += 16;
            *(uint32_t*)p = ((tdsTAHeader*)header)->GetActivitySequence();
            p += 4;
        }
        else if (header->GetType() == emTDHeader)
        {
            *(uint64_t*)p = ((tdsTDHeader*)header)->GetTransactionDescriptor();
            p += 8;
            *(uint32_t*)p = ((tdsTDHeader*)header)->GetOutstandingRequestCount();
            p += 4;
        }
    }
}