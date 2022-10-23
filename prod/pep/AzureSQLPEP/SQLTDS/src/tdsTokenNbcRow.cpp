#include "tdsTokenNbcRow.h"

void tdsTokenNbcRow::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadData(m_NullBitmap, m_NullBitmapByteCount);
	DWORD dwColCount = m_pColumnMetaData->GetColumnCount();
	m_vecColumnData.reserve(dwColCount);
	for (DWORD dwCol = 0; dwCol < dwColCount; ++dwCol)
	{
		if (IsNull(dwCol))
		{
			m_lstColumnData.push_back(nullptr);
			m_vecColumnData.push_back(nullptr);
		}
		else
		{
			tdsColumnDataInfo* pColDataInfo = m_pColumnMetaData->GetColumnDataInfo(dwCol);
			tdsColumnDataBase* pColData = CreateColumnData(pColDataInfo);
			pColData->Parse(rBuf);
			m_lstColumnData.push_back(pColData);
			m_vecColumnData.push_back(pColData);
		}
	}
}

int tdsTokenNbcRow::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteData(m_NullBitmap, m_NullBitmapByteCount);
	for (std::list<tdsColumnDataBase*>::const_iterator it = m_lstColumnData.begin(); it != m_lstColumnData.end(); ++it)
	{
		if (nullptr != *it)
		{
			(*it)->Serialize(wBuf);
		}
	}
	return 0;
}
