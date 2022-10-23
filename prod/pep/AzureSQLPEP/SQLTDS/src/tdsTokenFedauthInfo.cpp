#include "tdsTokenFedauthInfo.h"

tdsTokenFedauthInfo::tdsTokenFedauthInfo(void) : pDATA(nullptr), TokenLength(0)
{
	tdsTokenType = TDS_TOKEN_FEDAUTHINFO;
}

tdsTokenFedauthInfo::~tdsTokenFedauthInfo(void)
{
	delete[] pDATA;
}

void tdsTokenFedauthInfo::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(TokenLength);
	pDATA = new BYTE[TokenLength];
	rBuf.ReadData(pDATA, TokenLength);
}

DWORD tdsTokenFedauthInfo::CBSize()
{
	return tdsToken::CBSize() + sizeof(TokenLength) + TokenLength;
}

int tdsTokenFedauthInfo::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(TokenLength);
	wBuf.WriteData(pDATA, TokenLength);
	return 0;
}
