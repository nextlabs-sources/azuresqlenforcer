#ifndef TDS_COUNT_VAR_VALUE
#define TDS_COUNT_VAR_VALUE
#include <windows.h>
#include "MyBuffer.h"

template<typename CountType,typename ValueType>
class tdsCountVarValue
{
public:
	tdsCountVarValue(PBYTE pBuf)
	{
		Count = 0;
		pValue = NULL;
		Parse(pBuf);
	}
	tdsCountVarValue()
	{
		Count = 0;
		pValue = NULL;
	}
    tdsCountVarValue(const tdsCountVarValue& _other)
    {
        this->pValue = NULL;
        this->Count = _other.Count;
        if (this->Count > 0)
        {
            DWORD cbData = Count * sizeof(ValueType);
            this->pValue = (ValueType*)new BYTE[cbData];
            memcpy(this->pValue, _other.pValue, cbData);
        }
    }
	~tdsCountVarValue()
	{
		Free();
	}

public:
	void Parse(PBYTE pBuf)
	{
        if (pValue)
            Free();

		Count = *(CountType*)pBuf;
		pBuf += sizeof(CountType);

		if (Count>0)
		{
			DWORD cbData = Count * sizeof(ValueType);
			pValue =(ValueType*) new BYTE[cbData];
			memcpy(pValue, pBuf, cbData);
		}
	}
	void Parse(ReadBuffer& rBuf)
	{
        if (pValue)
            Free();

		rBuf.ReadAny(Count);

		if (Count > 0)
		{
			DWORD cbData = Count * sizeof(ValueType);
			pValue = (ValueType*) new BYTE[cbData];
			rBuf.ReadData(pValue, cbData);
		}
	}
	DWORD CBSize(){ return sizeof(CountType) + Count*sizeof(ValueType); }

private:
	void Free(){
		if (Count && pValue!=NULL)
		{
            PBYTE tmp = (PBYTE)pValue;
			delete[] tmp;
			pValue = NULL;
			Count = 0;
		}
	}


private:
	CountType  Count;
	ValueType* pValue;

// by Jie 2018.05.11
public:
	CountType GetCount() const { return Count; }
	ValueType* GetPValue() const { return pValue; }
	void SetData(PBYTE pBuf)
	{
		Free();
		Parse(pBuf);
	}
	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
	{
		cbData = 0;
		PBYTE p = pBuf;

		*(CountType*)p = Count;
		p += sizeof(CountType);
		dwBufLen -= sizeof(CountType);
		cbData += sizeof(CountType);

		if (Count > 0)
		{
			DWORD a = Count * sizeof(ValueType);
			memcpy(p, pValue, a);
			p += a;
			dwBufLen -= a;
			cbData += a;
		}

		return 0;
	}
	int Serialize(WriteBuffer& wBuf)
	{
		wBuf.WriteAny(Count);
		if (Count > 0)
		{
			DWORD a = Count * sizeof(ValueType);
			wBuf.WriteData(pValue, a);
		}
		return 0;
	}
};
#endif 

