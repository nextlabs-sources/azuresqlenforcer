#ifndef TDS_FEDAUTH_H
#define TDS_FEDAUTH_H

#include "tdsPacket.h"
#include "tdsCountVarValue.h"

class tdsFedauth : public tdsPacket
{
public:
	tdsFedauth();
	~tdsFedauth();

public:
	void Parse(PBYTE pData);

public:
	DWORD DataLen;
	tdsCountVarValue<LONG, BYTE> FedAuthToken;
	BYTE Nonce[32];

};

#endif // !TDS_FEDAUTH_H