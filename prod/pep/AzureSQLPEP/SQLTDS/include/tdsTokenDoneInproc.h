#ifndef TDS_TOKEN_DONE_INPROC_H
#define TDS_TOKEN_DONE_INPROC_H

#include "tdstoken.h"

class tdsTokenDoneInproc : public tdsToken
{
public:
	tdsTokenDoneInproc(void);
	~tdsTokenDoneInproc(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Status;
	USHORT CurCmd;
	ULONGLONG DoneRowCount;
};

#endif

