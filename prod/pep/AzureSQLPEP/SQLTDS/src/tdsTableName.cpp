#include "tdsTableName.h"


tdsTableName::tdsTableName(void)
{
	numberPart = 0;
	PartName = NULL;
}


tdsTableName::~tdsTableName(void)
{
	if (PartName)
	{
	   delete[] PartName;
	   PartName = NULL;
	}

	numberPart = 0;
}

void tdsTableName::Parse(PBYTE pBuf)
{
	PBYTE p = pBuf;

	numberPart = *p;
	p += sizeof(numberPart);

	if (numberPart>0)
	{
		PartName = new tdsCountVarValue<USHORT,WCHAR>[numberPart];
		for (int i=0; i<numberPart; i++)
		{
			PartName[i].Parse(p);
			p += PartName[i].CBSize();
		}
	}
}

void tdsTableName::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(numberPart);
	if (numberPart > 0)
	{
		PartName = new tdsCountVarValue<USHORT, WCHAR>[numberPart];
		for (int i = 0; i < numberPart; ++i)
		{
			PartName[i].Parse(rBuf);
		}
	}
}

DWORD tdsTableName::CBSize()
{
	DWORD dwSize = sizeof(numberPart);

	if (numberPart && (PartName!=NULL))
	{
		for (int i=0; i<numberPart; i++)
		{
			dwSize += PartName[i].CBSize();
		}
	}

	return dwSize;
}

int  tdsTableName::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{
	PBYTE p = pBuf;
	*p = numberPart;
	p			+= sizeof(numberPart);
	dwBufLen	-= sizeof(numberPart);
	cbData		+= sizeof(numberPart);

	if (numberPart > 0)
	{
		DWORD a = 0;
		for (size_t i = 0; i < numberPart; ++i)
		{
			PartName[i].Serialize(p, dwBufLen, a);
			p += PartName[i].CBSize();
			dwBufLen -= PartName[i].CBSize();
			cbData += PartName[i].CBSize();
		}
	}
	return 0;
}

int tdsTableName::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(numberPart);
	if (numberPart > 0)
	{
		for (size_t i = 0; i < numberPart; ++i)
		{
			PartName[i].Serialize(wBuf);
		}
	}
	return 0;
}
