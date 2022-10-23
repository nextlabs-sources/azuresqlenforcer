#include "tdsTokenColumnMetaData.h"

const USHORT tdsTokenColumnMetaData::NoMetaData = 0xffff;


tdsTokenColumnMetaData::tdsTokenColumnMetaData(void)
{
	tdsTokenType = TDS_TOKEN_COLMETADATA;
}


tdsTokenColumnMetaData::~tdsTokenColumnMetaData(void)
{
   delete[] pColumnData;
   pColumnData = 0;
}


void tdsTokenColumnMetaData::Parse(PBYTE pBuf)
{
   PBYTE p = pBuf;

   tdsToken::Parse(pBuf);
   p += tdsToken::CBSize();

   Count = *(USHORT*)p;
   p += sizeof(Count);

   CekTable.Parse(p);
   p +=  CekTable.CBSize();

   if (Count!=0 && Count!=0xffff)
   {
	   pColumnData = new tdsColumnDataInfo[Count];

	   for (int i=0; i<Count; i++)
	   {
		   pColumnData[i].Parse(p);
		   p += pColumnData[i].CBSize();
	   }
   }
   else
   {
	   //no metadata
   }
}

void tdsTokenColumnMetaData::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Count);
	CekTable.Parse(rBuf);
	if (Count != 0 && Count != 0xffff)
	{
		pColumnData = new tdsColumnDataInfo[Count];
		for (int i = 0; i < Count; ++i)
		{
			pColumnData[i].Parse(rBuf);
		}
	}
}

DWORD tdsTokenColumnMetaData::CBSize()
{	
	DWORD dwSize = tdsToken::CBSize() + sizeof(Count) + CekTable.CBSize();
	if (Count==0 || Count==0xffff)
	{
		dwSize += sizeof(NoMetaData);
	}
	else
	{
		for (int i=0; i<Count; i++)
		{
			dwSize += pColumnData[i].CBSize();
		}
	}

	return dwSize;
}



// by jie 2018.5.11
int  tdsTokenColumnMetaData::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{
	cbData = 0;
	DWORD a;
	PBYTE p = pBuf;
	tdsToken::Serialize(pBuf, dwBufLen, a);
	p			+= tdsToken::CBSize();
	dwBufLen	-= tdsToken::CBSize();
	cbData		+= tdsToken::CBSize();

	*(USHORT*)p = Count;
	p			+= sizeof(Count);
	dwBufLen	-= sizeof(Count);
	cbData		+= sizeof(Count);

	CekTable.Serialize(p, dwBufLen, a);
	p			+= CekTable.CBSize();
	dwBufLen	-= CekTable.CBSize();
	cbData		+= CekTable.CBSize();
	
	if (Count != 0 && Count != 0xffff)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			pColumnData[i].Serialize(p, dwBufLen, a);
			p			+= pColumnData[i].CBSize();
			dwBufLen	-= pColumnData[i].CBSize();
			cbData		+= pColumnData[i].CBSize();
		}
	}
	
	return 0;
}

int tdsTokenColumnMetaData::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Count);
	CekTable.Serialize(wBuf);
	if (Count != 0 && Count != 0xffff)
	{
		for (size_t i = 0; i < Count; ++i)
		{
			pColumnData[i].Serialize(wBuf);
		}
	}
	return 0;
}

int tdsTokenColumnMetaData::ColumnNameToIndex(const std::wstring& target)
{
	DWORD i = 0;
	bool flag = false;
	for (i = 0; i < GetColumnCount(); ++i)
	{
		if (!wcsnicmp(target.c_str(), GetColumnDataInfo(i)->GetPColumnNameBegin(), GetColumnDataInfo(i)->GetCloumnNameCount()))
		{
			flag = true;
			break;
		}	
	}

	if (flag)	// target columnName not found
		return i;
	return -1;
}

bool tdsTokenColumnMetaData::ColumnNameToIndex(const std::wstring& target, std::list<int>& ret)
{
	ret.clear();
	for (DWORD i = 0; i < GetColumnCount(); ++i)
	{
		int maxCnt = max(target.length(), GetColumnDataInfo(i)->GetCloumnNameCount());
		if (!wcsnicmp(target.c_str(), GetColumnDataInfo(i)->GetPColumnNameBegin(), maxCnt))
		{
			ret.push_back(i);
		}
	}
	return ret.size() > 0;
}
