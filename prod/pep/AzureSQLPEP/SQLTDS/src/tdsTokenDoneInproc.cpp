#include "tdsTokenDoneInproc.h"

tdsTokenDoneInproc::tdsTokenDoneInproc(void)
{
	tdsTokenType = TDS_TOKEN_DONEINPROC;
}

tdsTokenDoneInproc::~tdsTokenDoneInproc(void)
{
}

void tdsTokenDoneInproc::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Status);
	rBuf.ReadAny(CurCmd);
	rBuf.ReadAny(DoneRowCount);
}

DWORD tdsTokenDoneInproc::CBSize()
{
	return tdsToken::CBSize() + sizeof(Status) + sizeof(CurCmd) + sizeof(DoneRowCount);
}

int tdsTokenDoneInproc::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Status);
	wBuf.WriteAny(CurCmd);
	wBuf.WriteAny(DoneRowCount);
	return 0;
}
