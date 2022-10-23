#ifndef TDS_TOKEN_ROW_H
#define TDS_TOKEN_ROW_H
#include "tdsPacket.h"
#include "tdstoken.h"
#include "tdsCountVarValue.h"
#include <list>
#include "tdsColumnData.h"
#include "tdsTokenColumnMetaData.h"
#include <string>
#include <vector>

class tdsTokenRow : public tdsToken
{
public:
	tdsTokenRow(){
		m_pColumnMetaData = NULL;
       tdsTokenType = TDS_TOKEN_ROW;
	}
	virtual ~tdsTokenRow(void)
	{
		for (auto it : m_lstColumnData)
			delete it;
		m_lstColumnData.clear();
		m_vecColumnData.clear();
	};

public:
	virtual void SetColumnMetaData(tdsTokenColumnMetaData* colMetaData){m_pColumnMetaData=colMetaData;}
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize(){
		DWORD dwSize = tdsToken::CBSize();
		std::list<tdsColumnDataBase*>::iterator itColData = m_lstColumnData.begin();
		while (itColData != m_lstColumnData.end())
		{
			dwSize += (*itColData)->CBSize();
			itColData++;
		}
		return dwSize;
	}
	int Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);

protected:
	tdsColumnDataBase* CreateColumnData(tdsColumnDataInfo* pColumnInfo);
	tdsColumnDataBase* CreateVarLenColumnData(tdsColumnDataInfo* pColumnInfo);
	tdsColumnDataBase* CreateFixedLenColumnData(tdsColumnDataInfo* pColumnInfo);

protected:
	tdsTokenColumnMetaData* m_pColumnMetaData;
    std::list<tdsColumnDataBase*>  m_lstColumnData;

    std::vector<tdsColumnDataBase*> m_vecColumnData;		// accelerate search

public:
	bool SearchByColumnIndex(size_t index, tdsColumnDataInfo* &pColumnInfoRet, tdsColumnDataBase* &pTdsColumnDataBaseRet);
	virtual bool IsNull(size_t index)
	{
		return m_vecColumnData[index]->IsNull();
	}
	template<class LenType, class CharType>
	void FormatBuf(PBYTE pbuff, CharType mask, LenType cnt)
	{
		PBYTE p = pbuff;
		*(LenType*)pbuff = cnt;
		p += sizeof(LenType);
		PBYTE pEnd = p + cnt;
		for (; p != pEnd; p += sizeof(CharType))
		{
			*(CharType*)p = mask;
		}
	}

	template<class LenType>
	void CreateBuf(tdsColumnDataBase* pTdsColumnDataBase, bool isFixedLength, DWORD count, TDS_DATA_TYPE dataType, BYTE*& newValue)
	{
		LenType cnt = dynamic_cast<tdsColumnData<LenType, BYTE>*>(pTdsColumnDataBase)->getData().GetCount();
		if (!isFixedLength)
		{
			if (tdsTypeHelper::IsUnicode16Type(dataType))
			{
				cnt = (LenType)(count * sizeof(WCHAR));
			}
			else
			{
				cnt = (LenType)count;
			}
		}
		newValue = new BYTE[sizeof(LenType) + cnt];
		PBYTE p = newValue;
		if (tdsTypeHelper::IsUnicode16Type(dataType))
		{
			FormatBuf<LenType, WCHAR>(p, '*', cnt);
		}
		else if (tdsTypeHelper::IsMultiByte(dataType))
		{
			FormatBuf<LenType, CHAR>(p, '*', cnt);
		}
	}

public:
	bool maskStr(size_t index, bool isFixedLength = true, DWORD count = 0);
};

#endif

