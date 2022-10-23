#include "MyBuffer.h"
#include "TDS.h"

tdsTokenStream::tdsTokenStream(void)
{
}


tdsTokenStream::~tdsTokenStream(void)
{
	Free();
}

tdsToken* tdsTokenStream::CreateTokenByTokenType(TDS_TOKEN_TYPE tokenType)
{
	switch (tokenType)
	{
	case TDS_TOKEN_COLMETADATA:
		return new tdsTokenColumnMetaData;
		break;

	case TDS_TOKEN_DONE:
		return new tdsTokenDone;
		break;

	case TDS_TOKEN_ROW:
		return new tdsTokenRow;
		break;

	case TDS_TOKEN_NBCROW:
		return new tdsTokenNbcRow;
		break;

	case TDS_TOKEN_ORDER:
		return new tdsTokenOrder;
		break;

	case TDS_TOKEN_ERROR:
		return new tdsTokenError;
		break;

	case TDS_TOKEN_INFO:
		return new tdsTokenInfo;
		break;

	case TDS_TOKEN_LOGINACK:
		return new tdsTokenLoginAck;
		break;

	case TDS_TOKEN_ENVCHANGE:
		return new tdsTokenEnvChange;
		break;

	case TDS_TOKEN_FEDAUTHINFO:
		return new tdsTokenFedauthInfo;
		break;

	case TDS_TOKEN_DONEINPROC:
		return new tdsTokenDoneInproc;
		break;

	case TDS_TOKEN_FEATUREEXTACK:
		return new tdsTokenFeatureExtAck;
		break;

	default:
		//DebugBreak();
		break;
	}

	return NULL;
}

TDS_TOKEN_TYPE tdsTokenStream::GetTdsTokenType(BYTE bType)
{
	switch (bType)
	{
	case TDS_TOKEN_ALTMETADATA:
	case TDS_TOKEN_ALTROW:
	case TDS_TOKEN_COLMETADATA:
	case TDS_TOKEN_COLINFO:
	case TDS_TOKEN_DONE:
	case TDS_TOKEN_DONEPROC:
	case TDS_TOKEN_DONEINPROC:
	case TDS_TOKEN_ENVCHANGE:
	case TDS_TOKEN_ERROR:
	case TDS_TOKEN_FEATUREEXTACK:
	case TDS_TOKEN_FEDAUTHINFO:
	case TDS_TOKEN_INFO:
	case TDS_TOKEN_LOGINACK:
	case TDS_TOKEN_NBCROW:
	case TDS_TOKEN_OFFSET:
	case TDS_TOKEN_ORDER:
	case TDS_TOKEN_RETURNSTATUS:
	case TDS_TOKEN_RETURNVALUE:
	case TDS_TOKEN_ROW:
	case TDS_TOKEN_SESSIONSTATE:
	case TDS_TOKEN_SSPI:
	case TDS_TOKEN_TABNAME:
		return (TDS_TOKEN_TYPE)bType;
		break;

	default:
		return TDS_TOKEN_UNKNOWN;
		break;
	}
}

int tdsTokenStream::Parse(PBYTE pBuf)
{
	int nParseRes = 1;

	PBYTE p = pBuf;

	//
	hdr.Parse(p);
	p += TDS_HEADER_LEN;
    DWORD dwAllTokenLen = hdr.GetPacketLength() - TDS_HEADER_LEN;

	DWORD dwParsedLen = 0;
	PBYTE pTokenData = p;
	tdsTokenColumnMetaData* pTokenMetaData = NULL;
	while (dwParsedLen<dwAllTokenLen)
	{
		BYTE tokenType = *pTokenData;
		TDS_TOKEN_TYPE tdsTokenType = GetTdsTokenType(tokenType);

		if (TDS_TOKEN_UNKNOWN!=tdsTokenType)
		{
			tdsToken* pToken = CreateTokenByTokenType(tdsTokenType);
			if (pToken)
			{
				if (pToken->GetTdsTokenType()==TDS_TOKEN_COLMETADATA)
				{
					pTokenMetaData = dynamic_cast<tdsTokenColumnMetaData*>(pToken);
				}
				else if (pToken->GetTdsTokenType()==TDS_TOKEN_ROW)
				{
					dynamic_cast<tdsTokenRow*>(pToken)->SetColumnMetaData(pTokenMetaData);
				}

				pToken->Parse(pTokenData);
				m_lstTokens.push_back(pToken);

				DWORD dwTokenSize = pToken->CBSize();
				dwParsedLen += dwTokenSize;
				pTokenData += dwTokenSize;
			}
			else
			{
				nParseRes = 0;
				break;
			}
		}
		else
		{
			nParseRes = 0;
			//DebugBreak();
			break;
		}
	}

	return nParseRes;
}


void tdsTokenStream::Free()
{
    for (auto it : m_lstTokens)
	{
        if (it) 
            delete it;
	}

	m_lstTokens.clear();
}

// by jie 2018.05.11
int tdsTokenStream::Serialize(PBYTE pBuf, DWORD bufLen, DWORD& cbData)
{
	cbData = 0;
	PBYTE p = pBuf;
	hdr.Serialize(p);
	p		+= TDS_HEADER_LEN;
	bufLen	-= TDS_HEADER_LEN;
	cbData	+= TDS_HEADER_LEN;
	DWORD a = 0;
	for (std::list<tdsToken*>::const_iterator itToken = m_lstTokens.begin(); itToken != m_lstTokens.end(); ++itToken)
	{
		(*itToken)->Serialize(p, bufLen, a);
		p		+= (*itToken)->CBSize();
		bufLen	-= (*itToken)->CBSize();
		cbData	+= (*itToken)->CBSize();
	}

    TDS::SetTdsPacketLength(pBuf, cbData);

	return 0;
}

int tdsTokenStream::Serialize(std::list<PBYTE> listBuf, std::list<DWORD> listBufLen)
{
	// Fill Heads
	WriteBuffer wBuf;
	auto itBuf = listBuf.begin();
	auto itLen = listBufLen.begin();
	BYTE id = 1;
	while (itBuf != listBuf.end() && itLen != listBufLen.end())
	{
		// Fill Each Head
		hdr.SetLength(*itLen);
		hdr.SetPacketID(id);
		if (*itBuf == listBuf.back())
		{
			// Mark End Of Message
			hdr.SetStatus(TDS_STATUS_EOM);
		}
		else
		{
			hdr.SetStatus(0);
		}
		hdr.Serialize(*itBuf);
		wBuf.attach((*itBuf) + TDS_HEADER_LEN, *itLen - TDS_HEADER_LEN);
		++itBuf;
		++itLen;
		++id;
	}
	
	// Fill Data
	for (std::list<tdsToken*>::const_iterator itToken = m_lstTokens.begin(); itToken != m_lstTokens.end(); ++itToken)
	{
		(*itToken)->Serialize(wBuf);
	}
	return 0;
}

int tdsTokenStream::Parse(std::list<PBYTE> listBuf)
{
	ReadBuffer rBuf;
	for (auto it = listBuf.begin(); it != listBuf.end(); ++it)
	{
		if (it == listBuf.begin())
		{
            PBYTE pack = *it;
            if (pack[0] == SMP_PACKET_FLAG)
            {
                if (SMP::GetSMPPacketLen(pack) > 16)
                    rBuf.attach(pack + 16, TDS::GetTdsPacketLength(pack + 16));
            }
            else
            {
                rBuf.attach(pack, TDS::GetTdsPacketLength(pack));
            }
		}
		else
		{
            PBYTE pack = *it;
            if (pack[0] == SMP_PACKET_FLAG)
            {
                if (SMP::GetSMPPacketLen(pack) > 16)
                    rBuf.attach(pack + 16 + TDS_HEADER_LEN, TDS::GetTdsPacketLength(pack + 16) - TDS_HEADER_LEN);
            }
            else
			    rBuf.attach(pack + TDS_HEADER_LEN, TDS::GetTdsPacketLength(pack) - TDS_HEADER_LEN);
		}
	}

	int nParseRes = 1;
	hdr.Parse(rBuf);
	tdsTokenColumnMetaData* pTokenMetaData = NULL;
	while (rBuf.Avail())
	{
		BYTE tokenType;
		rBuf.PeekAny(tokenType);
		TDS_TOKEN_TYPE tdsTokenType = GetTdsTokenType(tokenType);
		if (TDS_TOKEN_UNKNOWN != tdsTokenType)
		{
			tdsToken* pToken = CreateTokenByTokenType(tdsTokenType);
			if (pToken)
			{
				if (pToken->GetTdsTokenType() == TDS_TOKEN_COLMETADATA)
				{
					pTokenMetaData = dynamic_cast<tdsTokenColumnMetaData*>(pToken);
				}
				else if (pToken->GetTdsTokenType() == TDS_TOKEN_ROW)
				{
					dynamic_cast<tdsTokenRow*>(pToken)->SetColumnMetaData(pTokenMetaData);
				}
				else if (pToken->GetTdsTokenType() == TDS_TOKEN_NBCROW)
				{
					dynamic_cast<tdsTokenNbcRow*>(pToken)->SetColumnMetaData(pTokenMetaData);
				}
				pToken->Parse(rBuf);
				m_lstTokens.push_back(pToken);
			}
			else
			{
				nParseRes = 0;
				break;
			}
		}
		else
		{
			nParseRes = 0;
			break;
		}
	}
	return nParseRes;
}

DWORD tdsTokenStream::CBSize() const
{
	DWORD cbData = 0;
	cbData += TDS_HEADER_LEN;
	for (std::list<tdsToken*>::const_iterator itToken = m_lstTokens.begin(); itToken != m_lstTokens.end(); ++itToken)
	{
		cbData += (*itToken)->CBSize();
	}
	return cbData;
}

bool tdsTokenStream::MaskStr(const std::wstring& columnName, DWORD cnt/* = MAXDWORD*/)
{
	bool isFixedLength = false;
	if (cnt == MAXDWORD || cnt <= 0)
	{
		isFixedLength = true;
	}
	else
	{
		isFixedLength = false;
	}

	int index = -1;
	for (auto it = m_lstTokens.begin(); it != m_lstTokens.end(); ++it)
	{
		if (TDS_TOKEN_ROW == (*it)->GetTdsTokenType())
		{
			if (-1 == index)
				continue;
			if (!dynamic_cast<tdsTokenRow*>(*it)->maskStr(index, isFixedLength, cnt))
				return false;
		}
		else if (TDS_TOKEN_COLMETADATA == (*it)->GetTdsTokenType())
		{
			index = dynamic_cast<tdsTokenColumnMetaData*>(*it)->ColumnNameToIndex(columnName);
		}
	}

	return true;
}

bool tdsTokenStream::MaskStrAll(const std::wstring& columnName, DWORD cnt/* = MAXDWORD*/)
{
	bool isFixedLength = false;
	if (cnt == MAXDWORD || cnt <= 0)
	{
		isFixedLength = true;
	}
	else
	{
		isFixedLength = false;
	}

	std::list<int> indexs;
	for (auto it = m_lstTokens.begin(); it != m_lstTokens.end(); ++it)
	{
		if (TDS_TOKEN_ROW == (*it)->GetTdsTokenType())
		{
			if (0 == indexs.size())
				continue;
			for (auto itIdx = indexs.begin(); itIdx != indexs.end(); ++itIdx)
			{
				dynamic_cast<tdsTokenRow*>(*it)->maskStr(*itIdx, isFixedLength, cnt);
			}
		}
		else if (TDS_TOKEN_NBCROW == (*it)->GetTdsTokenType())
		{
			if (0 == indexs.size())
				continue;
			for (auto itIdx = indexs.begin(); itIdx != indexs.end(); ++itIdx)
			{
				dynamic_cast<tdsTokenRow*>(*it)->maskStr(*itIdx, isFixedLength, cnt);
			}
		}
		else if (TDS_TOKEN_COLMETADATA == (*it)->GetTdsTokenType())
		{
			dynamic_cast<tdsTokenColumnMetaData*>(*it)->ColumnNameToIndex(columnName, indexs);
		}
	}
	return true;
}

bool tdsTokenStream::GetRoutingInfo(std::wstring& server, USHORT& port)
{
	for (auto it : m_lstTokens)
	{
		if (it->GetTdsTokenType() == TDS_TOKEN_ENVCHANGE &&
			dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().Type == SENDS_ROUTING_INFORMATION_TO_CLIENT)
		{
			auto& routing = dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().NewValue.ROUTING;
			server = std::wstring(routing.AlternateServer.GetPValue(), routing.AlternateServer.GetCount());
			port = routing.ProtocolProperty;
			return true;
		}
	}
	return false;
}

bool tdsTokenStream::GetNegotiatePacketSize(DWORD& sz)
{
	for (auto it : m_lstTokens)
	{
		if (it->GetTdsTokenType() == TDS_TOKEN_ENVCHANGE &&
			dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().Type == PACKET_SIZE)
		{
			auto& newvalue = dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().NewValue.B_VARCHAR;
			std::wstring n(newvalue.GetPValue(), newvalue.GetCount());
			sz = _wtoi(n.c_str());
			return true;
		}
	}
	return false;
}

bool tdsTokenStream::GetDefaultDatabase(std::wstring& database)
{
    for (auto it : m_lstTokens)
    {
        if (it->GetTdsTokenType() == TDS_TOKEN_ENVCHANGE &&
            dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().Type == DATABASE)
        {
            auto& newvalue = dynamic_cast<tdsTokenEnvChange*>(it)->GetTdsEnvValueData().NewValue.B_VARCHAR;
            database = std::wstring(newvalue.GetPValue(), newvalue.GetCount());
            return true;
        }
    }
    return false;
}