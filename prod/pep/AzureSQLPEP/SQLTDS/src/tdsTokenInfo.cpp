#include "tdsTokenInfo.h"

tdsTokenInfo::tdsTokenInfo(void) : pDATA(nullptr), Length(0)
{
	tdsTokenType = TDS_TOKEN_INFO;
}

tdsTokenInfo::~tdsTokenInfo(void)
{
	delete[] pDATA;
}

void tdsTokenInfo::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Length);
	pDATA = new BYTE[Length];
	rBuf.ReadData(pDATA, Length);
}

DWORD tdsTokenInfo::CBSize()
{
	return tdsToken::CBSize() + sizeof(Length) + Length;
}

int tdsTokenInfo::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Length);
	wBuf.WriteData(pDATA, Length);
	return 0;
}
