#ifndef TDS_TOKEN_INFO_H
#define TDS_TOKEN_INFO_H

#include "tdstoken.h"

class tdsTokenInfo : public tdsToken
{
public:
	tdsTokenInfo(void);
	~tdsTokenInfo(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Length;
	PBYTE pDATA;
};

#endif

