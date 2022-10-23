#include "tdsTokenDone.h"


tdsTokenDone::tdsTokenDone(void)
{
	tdsTokenType = TDS_TOKEN_DONE;
}


tdsTokenDone::~tdsTokenDone(void)
{
}

void tdsTokenDone::Parse(PBYTE pBuf)
{
	PBYTE p= pBuf;

	tdsToken::Parse(pBuf);
	p += tdsToken::CBSize();

	Status = *(USHORT*)p;
	p += sizeof(Status);

	CurCmd = *(USHORT*)p;
	p += sizeof(CurCmd);

	DoneRowCount = *(ULONGLONG*)p;
	p += sizeof(DoneRowCount);
}

void tdsTokenDone::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Status);
	rBuf.ReadAny(CurCmd);
	rBuf.ReadAny(DoneRowCount);
}

int  tdsTokenDone::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{	
	cbData = 0;
	PBYTE p = pBuf;
	DWORD a = 0;

	tdsToken::Serialize(p, dwBufLen, a);
	p += tdsToken::CBSize();
	dwBufLen -= tdsToken::CBSize();
	cbData += tdsToken::CBSize();

	*(USHORT*)p = Status;
	p += sizeof(Status);
	dwBufLen -= sizeof(Status);
	cbData += sizeof(Status);

	*(USHORT*)p = CurCmd;
	p += sizeof(CurCmd);
	dwBufLen -= sizeof(CurCmd);
	cbData += sizeof(CurCmd);

	*(ULONGLONG*)p = DoneRowCount;
	p += sizeof(DoneRowCount);
	dwBufLen -= sizeof(DoneRowCount);
	cbData += sizeof(DoneRowCount);

	return 0;
}

int tdsTokenDone::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Status);
	wBuf.WriteAny(CurCmd);
	wBuf.WriteAny(DoneRowCount);
	return 0;
}
