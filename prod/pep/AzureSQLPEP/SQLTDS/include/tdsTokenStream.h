#ifndef TDS_TOKEN_STREAM_H
#define TDS_TOKEN_STREAM_H

#include <windows.h>
#include <list>
#include "tdsTokenColumnMetaData.h"
#include "tdsPacket.h"
#include "tdsTokenRow.h"

class tdsTokenStream : public tdsPacket
{
public:
	tdsTokenStream(void);
	~tdsTokenStream(void);

public:
	int Parse(PBYTE pBuf);

protected:
	void Free();

private:
	tdsToken* CreateTokenByTokenType(TDS_TOKEN_TYPE tokenType);
	TDS_TOKEN_TYPE GetTdsTokenType(BYTE bType);

private:
	std::list<tdsToken*> m_lstTokens;

// by jie 2018.05.11
public:
	int Serialize(PBYTE pBuf, DWORD bufLen, DWORD& cbData);
	int Parse(std::list<PBYTE> listBuf);
	int Serialize(std::list<PBYTE> listBuf, std::list<DWORD> listBufLen);
	DWORD CBSize() const;
	bool test()
	{
		if (m_lstTokens.size() > 1 && m_lstTokens.front()->GetTdsTokenType() == TDS_TOKEN_COLMETADATA
			&& m_lstTokens.back()->GetTdsTokenType() == TDS_TOKEN_DONE)
			return true;
		return false;
	}

	bool MaskStr(const std::wstring& columnName, DWORD cnt = MAXDWORD);
	bool MaskStrAll(const std::wstring& columnName, DWORD cnt = MAXDWORD);

public:
	bool GetRoutingInfo(std::wstring& server, USHORT& port);
	bool GetNegotiatePacketSize(DWORD& sz);
    bool GetDefaultDatabase(std::wstring& database);
};

#endif 

