#include "tdsFedauth.h"



tdsFedauth::tdsFedauth()
{
}


tdsFedauth::~tdsFedauth()
{
}

void tdsFedauth::Parse(PBYTE pBuf)
{
	PBYTE p = pBuf;
	hdr.Parse(pBuf);
	p += TDS_HEADER_LEN;

	DataLen = *(DWORD*)p;
	p += sizeof(DWORD);

	FedAuthToken.Parse(p);
	p += FedAuthToken.CBSize();

}
