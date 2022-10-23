#ifndef TDS_TOKEN_FEDAUTH_INFO_H
#define TDS_TOKEN_FEDAUTH_INFO_H

#include "tdstoken.h"

class tdsTokenFedauthInfo : public tdsToken
{
public:
	tdsTokenFedauthInfo(void);
	~tdsTokenFedauthInfo(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	DWORD TokenLength;
	PBYTE pDATA;
};

#endif

