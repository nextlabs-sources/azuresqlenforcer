#include "tdsTokenRow.h"


void tdsTokenRow::Parse(PBYTE pBuf)
{
	PBYTE p = pBuf;

	tdsToken::Parse(p);
	p += tdsToken::CBSize();

	DWORD dwColCount = m_pColumnMetaData->GetColumnCount();
	
	m_vecColumnData.reserve(dwColCount);

	for (DWORD dwCol=0; dwCol<dwColCount; dwCol++)
	{
	     tdsColumnDataInfo* pColDataInfo = m_pColumnMetaData->GetColumnDataInfo(dwCol);

		 tdsColumnDataBase* pColData = CreateColumnData(pColDataInfo);
		 pColData->Parse(p);
		 m_lstColumnData.push_back(pColData);

		 p += pColData->CBSize();

		 //
		 /*std::string cur(pColDataInfo->GetPColumnNameBegin(),
			 pColDataInfo->GetPColumnNameBegin() + pColDataInfo->GetCloumnNameCount());
		 m_mapColumnData[cur] = SearchItem(pColData, pColDataInfo);*/
		 m_vecColumnData.push_back(pColData);
	}
}

void tdsTokenRow::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	DWORD dwColCount = m_pColumnMetaData->GetColumnCount();
	m_vecColumnData.reserve(dwColCount);
	for (DWORD dwCol = 0; dwCol < dwColCount; ++dwCol)
	{
		tdsColumnDataInfo* pColDataInfo = m_pColumnMetaData->GetColumnDataInfo(dwCol);
		tdsColumnDataBase* pColData = CreateColumnData(pColDataInfo);
		pColData->Parse(rBuf);
		m_lstColumnData.push_back(pColData);
		m_vecColumnData.push_back(pColData);
	}
}

tdsColumnDataBase* tdsTokenRow::CreateFixedLenColumnData(tdsColumnDataInfo* pColumnInfo)
{
	return new tdsColumnData<BYTE, BYTE>(pColumnInfo);
}

tdsColumnDataBase* tdsTokenRow::CreateVarLenColumnData(tdsColumnDataInfo* pColumnInfo)
{
	TDS_LEN_TYPE lenType = pColumnInfo->GetLenType();
	TDS_DATA_TYPE dataType = pColumnInfo->GetDataType();

	if (BYTE_LEN_TYPE==lenType)
	{
	    return new tdsColumnData<BYTE, BYTE>(pColumnInfo);
	}
	else if (USHORT_LEN_TYPE==lenType)
	{
		return new tdsColumnData<USHORT, BYTE>(pColumnInfo);
	}
	else if (LONG_LEN_TYPE==lenType)
	{
		return new tdsColumnData<ULONG, BYTE>(pColumnInfo);
	}
	else
	{
		assert(FALSE);
	}
	return NULL;
}


tdsColumnDataBase* tdsTokenRow::CreateColumnData(tdsColumnDataInfo* pColumnInfo)
{
	if (pColumnInfo->IsFixedLenType())
	{
		return CreateFixedLenColumnData(pColumnInfo);
	}
	else
	{
		return CreateVarLenColumnData(pColumnInfo);
	}
}

int tdsTokenRow::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{
	cbData = 0;
	PBYTE p = pBuf;

	DWORD a = 0;
	tdsToken::Serialize(p, dwBufLen, a);
	p += tdsToken::CBSize();
	dwBufLen -= tdsToken::CBSize();
	cbData += tdsToken::CBSize();

	for (std::list<tdsColumnDataBase*>::const_iterator it = m_lstColumnData.begin(); it != m_lstColumnData.end(); ++it)
	{
		(*it)->Serialize(p, dwBufLen, a);
		p += (*it)->CBSize();
		dwBufLen -= (*it)->CBSize();
		cbData += (*it)->CBSize();
	}

	return 0;
}

int tdsTokenRow::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	for (std::list<tdsColumnDataBase*>::const_iterator it = m_lstColumnData.begin(); it != m_lstColumnData.end(); ++it)
	{
		(*it)->Serialize(wBuf);
	}
	return 0;
}

bool tdsTokenRow::SearchByColumnIndex(size_t index, tdsColumnDataInfo* &pColumnInfoRet, tdsColumnDataBase* &pTdsColumnDataBaseRet)
{
	if (0 <= index && index < m_vecColumnData.size())
	{
		if (IsNull(index))
			return false;
		pTdsColumnDataBaseRet = m_vecColumnData[index];
		pColumnInfoRet = m_pColumnMetaData->GetColumnDataInfo(index);
		return true;
	}

	return false;
}

bool tdsTokenRow::maskStr(size_t index, bool isFixedLength/* = true*/, DWORD count/* = 0*/)
{
	tdsColumnDataInfo* pColumnInfo = nullptr;
	tdsColumnDataBase* pTdsColumnDataBase = nullptr;
	if (!SearchByColumnIndex(index, pColumnInfo, pTdsColumnDataBase))
		return false;

	BYTE* newValue = nullptr;
	
	if (pColumnInfo->IsFixedLenType())
	{
		//tdsColumnData<BYTE, BYTE>
		return false;
	}
	else
	{
		TDS_LEN_TYPE lenType = pColumnInfo->GetLenType();
		TDS_DATA_TYPE dataType = pColumnInfo->GetDataType();

		if (!tdsTypeHelper::IsUnicode16Type(dataType) &&
			!tdsTypeHelper::IsMultiByte(dataType))
			return false;

		if (BYTE_LEN_TYPE == lenType)
		{
			//tdsColumnData<BYTE, BYTE>
			CreateBuf<BYTE>(pTdsColumnDataBase, isFixedLength, count, dataType, newValue);
		}
		else if (USHORT_LEN_TYPE == lenType)
		{
			//tdsColumnData<USHORT, BYTE>
			CreateBuf<USHORT>(pTdsColumnDataBase, isFixedLength, count, dataType, newValue);
		}
		else if (LONG_LEN_TYPE == lenType)
		{
			//tdsColumnData<ULONG, BYTE>
			CreateBuf<ULONG>(pTdsColumnDataBase, isFixedLength, count, dataType, newValue);
		}
		else
		{
			assert(FALSE);
		}
		pTdsColumnDataBase->SetData(newValue);
		delete[] newValue;
	}
	return true;
}