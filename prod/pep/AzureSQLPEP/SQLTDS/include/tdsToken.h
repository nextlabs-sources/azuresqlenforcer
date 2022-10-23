#ifndef TDS_TOKEN_H
#define TDS_TOKEN_H
#include <windows.h>
#include <assert.h>
#include "tdsPacket.h"
#include "MyBuffer.h"

class tdsToken
{
public:
	tdsToken(void){}
	virtual ~tdsToken(void){}

public:
	TDS_TOKEN_TYPE GetTdsTokenType() { return tdsTokenType;}

public:
	virtual void Parse(PBYTE pBuf)
	{
		assert(tdsTokenType==*pBuf);
	}
	virtual void Parse(ReadBuffer& rBuf)
	{
		BYTE tp;
		rBuf.ReadAny(tp);
		assert(tdsTokenType == tp);
	}
	virtual DWORD CBSize(){ return 1/*tdsTokenType*/;}
	virtual int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
	{
		cbData = 0;
		*pBuf = tdsTokenType;
		pBuf += sizeof(BYTE);
		dwBufLen -= sizeof(BYTE);
		cbData += sizeof(BYTE);
		return 0;
	}

	virtual int Serialize(WriteBuffer& wBuf)
	{
		wBuf.WriteAny((BYTE)tdsTokenType);
		return 0;
	}

protected:
	TDS_TOKEN_TYPE tdsTokenType;
};

#endif 

