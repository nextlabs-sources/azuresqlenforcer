#include "tdsTokenError.h"

tdsTokenError::tdsTokenError(void) : pDATA(nullptr), Length(0)
{
	tdsTokenType = TDS_TOKEN_ERROR;
}

tdsTokenError::~tdsTokenError(void)
{
    if (pDATA)
	    delete[] pDATA;
}

void tdsTokenError::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Length);
	pDATA = new BYTE[Length];
	rBuf.ReadData(pDATA, Length);
}

DWORD tdsTokenError::CBSize()
{
	return tdsToken::CBSize() + sizeof(Length) + Length;
}

int tdsTokenError::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Length);
	wBuf.WriteData(pDATA, Length);
	return 0;
}
