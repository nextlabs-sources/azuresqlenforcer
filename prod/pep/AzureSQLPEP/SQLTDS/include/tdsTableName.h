#ifndef __TDS_TABLE_NAME_H__
#define __TDS_TABLE_NAME_H__

#include <windows.h>
#include "tdsCountVarValue.h"
#include "MyBuffer.h"

class tdsTableName
{
public:
	tdsTableName(void);
	~tdsTableName();

public:
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();

	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);
private:
	BYTE numberPart;
	tdsCountVarValue<USHORT,WCHAR>* PartName;
};

#endif