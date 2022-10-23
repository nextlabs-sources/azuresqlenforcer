#ifndef TDS_TOKEN_ERROR_H
#define TDS_TOKEN_ERROR_H

#include "tdstoken.h"

class tdsTokenError : public tdsToken
{
public:
	tdsTokenError(void);
	~tdsTokenError(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Length;
	PBYTE pDATA;
};

#endif

