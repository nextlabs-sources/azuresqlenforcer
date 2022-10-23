#ifndef TDS_TOKEN_LOGIN_ACK_H
#define TDS_TOKEN_LOGIN_ACK_H

#include "tdstoken.h"

class tdsTokenLoginAck : public tdsToken
{
public:
	tdsTokenLoginAck(void);
	~tdsTokenLoginAck(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Length;
	PBYTE pDATA;
};

#endif

