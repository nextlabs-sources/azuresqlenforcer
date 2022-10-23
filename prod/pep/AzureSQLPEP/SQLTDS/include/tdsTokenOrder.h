#ifndef TDS_TOKEN_ORDER_H
#define TDS_TOKEN_ORDER_H

#include <list>
#include "tdstoken.h"

class tdsTokenOrder : public tdsToken
{
public:
	tdsTokenOrder(void);
	~tdsTokenOrder(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Length;
	std::list<USHORT> ColNum;
};

#endif

