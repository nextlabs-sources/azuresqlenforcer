#include "tdsTokenLoginAck.h"

tdsTokenLoginAck::tdsTokenLoginAck(void) : pDATA(nullptr), Length(0)
{
	tdsTokenType = TDS_TOKEN_LOGINACK;
}

tdsTokenLoginAck::~tdsTokenLoginAck(void)
{
	delete[] pDATA;
}

void tdsTokenLoginAck::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Length);
	pDATA = new BYTE[Length];
	rBuf.ReadData(pDATA, Length);
}

DWORD tdsTokenLoginAck::CBSize()
{
	return tdsToken::CBSize() + sizeof(Length) + Length;
}

int tdsTokenLoginAck::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Length);
	wBuf.WriteData(pDATA, Length);
	return 0;
}
