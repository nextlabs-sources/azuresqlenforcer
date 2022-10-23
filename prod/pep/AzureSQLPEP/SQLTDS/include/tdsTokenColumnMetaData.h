#ifndef TDS_COLUMN_META_DATA_H
#define TDS_COLUMN_META_DATA_H
#include <windows.h>
#include "tdsCountVarValue.h"
#include "tdsEK_INFO.h"
#include "tdsTypeInfo.h"
#include "tdsCountVarValue.h"
#include "tdsCryptoMetaData.h"
#include "tdsColumnDataInfo.h"
#include "tdsToken.h"
#include <string>
#include "MyBuffer.h"
#include <list>

class tdsTokenColumnMetaData : public tdsToken
{
public:
	tdsTokenColumnMetaData(void);
	~tdsTokenColumnMetaData(void);

public:
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);

	USHORT GetColumnCount() { return Count;}
	tdsColumnDataInfo* GetColumnDataInfo(DWORD dwColIndex){return &pColumnData[dwColIndex]; }
	int ColumnNameToIndex(const std::wstring& target);	// GET FIRST
	bool ColumnNameToIndex(const std::wstring& target, std::list<int>& ret);

private:
	USHORT Count;
	tdsCountVarValue<USHORT,tdsEK_INFO> CekTable;
	tdsColumnDataInfo* pColumnData;
	const static USHORT NoMetaData;
};
#endif 

