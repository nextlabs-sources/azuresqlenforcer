#include "tdsTokenOrder.h"


tdsTokenOrder::tdsTokenOrder(void)
{
	tdsTokenType = TDS_TOKEN_ORDER;
}

tdsTokenOrder::~tdsTokenOrder(void)
{
}

void tdsTokenOrder::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Length);
	int cur = 0;
	ColNum.clear();
	do
	{
		USHORT col = 0;
		cur += rBuf.ReadAny(col);
		ColNum.push_back(col);
	} while (cur < Length);
}

DWORD tdsTokenOrder::CBSize()
{
	return tdsToken::CBSize() + sizeof(Length) + ColNum.size() * sizeof(USHORT);
}

int tdsTokenOrder::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Length);
	for (auto it : ColNum)
		wBuf.WriteAny(it);
	return 0;
}
