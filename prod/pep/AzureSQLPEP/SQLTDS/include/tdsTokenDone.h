#ifndef TDS_TOKEN_DONE_H
#define TDS_TOKEN_DONE_H

#include "tdstoken.h"

class tdsTokenDone : public tdsToken
{
public:
	tdsTokenDone(void);
	~tdsTokenDone(void);

public:
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize(){return tdsToken::CBSize() +
		sizeof(Status) +
		sizeof(CurCmd) +
		sizeof(DoneRowCount); }

	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);
private:
	USHORT Status;
	USHORT CurCmd;
	ULONGLONG DoneRowCount;

	/*
	TokenType        =   BYTE
	Status           =   USHORT
	CurCmd           =   USHORT
	DoneRowCount     =   LONG / ULONGLONG;  (Changed to ULONGLONG in TDS 7.2)
   */

};

#endif

