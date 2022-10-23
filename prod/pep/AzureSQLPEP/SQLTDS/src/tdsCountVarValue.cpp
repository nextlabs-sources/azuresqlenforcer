#if 0
#include "tdsCountVarValue.h"

template<typename CountType,typename ValueType>
tdsCountVarValue<CountType,ValueType>::tdsCountVarValue(PBYTE pBuf)
{
	Count = 0;
	pValue = NULL;
	Parse(pBuf);
}

template<typename CountType,typename ValueType>
tdsCountVarValue<CountType,ValueType>::tdsCountVarValue()
{
	Count = 0;
	pValue = NULL;
}

template<typename CountType,typename ValueType>
tdsCountVarValue<CountType,ValueType>::~tdsCountVarValue()
{
	Free();
}

template<typename CountType,typename ValueType>
void tdsCountVarValue<CountType,ValueType>::Parse(PBYTE pBuf)
{
	Count = (CountType*)pBuf;
	pBuf += sizeof(CountType);

	pValue = NULL;
	if (Count>0)
	{
		pValue =(ValueType*) new BYTE[Count];
		memcpy(pValue, pBuf, Count);
	}
}

template<typename CountType,typename ValueType>
void tdsCountVarValue<CountType,ValueType>::Free()
{
	if (Count && pValue!=NULL)
	{
		delete[] pValue;
		pValue = NULL;
		Count = 0;
	}
}
#endif 
