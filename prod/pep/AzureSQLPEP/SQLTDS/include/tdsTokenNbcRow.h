#ifndef TDS_TOKEN_NBC_ROW_H
#define TDS_TOKEN_NBC_ROW_H
#include "tdsPacket.h"
#include "tdstoken.h"
#include "tdsCountVarValue.h"
#include <list>
#include "tdsColumnData.h"
#include "tdsTokenColumnMetaData.h"
#include <string>
#include <vector>
#include "tdsTokenRow.h"

class tdsTokenNbcRow : public tdsTokenRow
{
public:
	tdsTokenNbcRow(){
		m_NullBitmap = nullptr;
		m_NullBitmapByteCount = 0;
       tdsTokenType = TDS_TOKEN_NBCROW;
	}
	virtual ~tdsTokenNbcRow(void)
	{
        if (m_NullBitmap)
		    delete[] m_NullBitmap;
	};

public:
	void SetColumnMetaData(tdsTokenColumnMetaData* colMetaData)
	{
		m_pColumnMetaData=colMetaData;
		int NullBitmapBitCount = m_pColumnMetaData->GetColumnCount();
		int NullBitmapByteCount = (NullBitmapBitCount + 8 - 1) / 8;
		m_NullBitmap = new BYTE[NullBitmapByteCount];
		m_NullBitmapByteCount = NullBitmapByteCount;
	}

	void Parse(ReadBuffer& rBuf);
	DWORD CBSize(){
		DWORD dwSize = tdsToken::CBSize();
		std::list<tdsColumnDataBase*>::iterator itColData = m_lstColumnData.begin();
		while (itColData != m_lstColumnData.end())
		{
			if (nullptr != *itColData)
			{
				dwSize += (*itColData)->CBSize();
			}
			itColData++;
		}
		dwSize += sizeof(BYTE) * m_NullBitmapByteCount;
		return dwSize;
	}
	int Serialize(WriteBuffer& wBuf);
private:
	BYTE* m_NullBitmap;
	int m_NullBitmapByteCount;
public:
	virtual bool IsNull(size_t index)
	{
		int byteIndex = index / 8;
		int posAtByte = (index % 8);
		BYTE cur = m_NullBitmap[byteIndex];
		return cur & (1 << posAtByte);
	}
};

#endif

