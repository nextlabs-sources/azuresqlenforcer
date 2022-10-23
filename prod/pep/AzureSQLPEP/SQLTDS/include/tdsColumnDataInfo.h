#ifndef TDS_COLUMN_DATA_INFO
#define TDS_COLUMN_DATA_INFO

#include <windows.h>
#include "tdsCountVarValue.h"
#include "tdsTypeInfo.h"
#include "tdsCryptoMetaData.h"
#include "tdsTableName.h"
#include "MyBuffer.h"

class tdsColumnDataInfo
{

public:
	tdsColumnDataInfo(void);
	tdsColumnDataInfo(PBYTE pBuf);
	~tdsColumnDataInfo();

public:
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize() { return sizeof(UserType) +
							sizeof(Flags) +
							TypeInfo.CBSize() +
							(bHaveTableName ? tableName.CBSize() : 0 ) +
							(bHaveCryptMetaData ? CryptoMetaData.CBSize() : 0) +
							ColumnName.CBSize(); }

	TDS_LEN_TYPE GetLenType() { return TypeInfo.GetLenType(); }
	TDS_DATA_TYPE GetDataType() { return TypeInfo.GetTdsDataType(); }
	BOOL IsFixedLenType() { return TypeInfo.IsFixedLenType();}

	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);

private:


private:
	ULONG UserType;
	BYTE Flags[2]; //16 bit
	tdsTypeInfo TypeInfo;

	BOOL bHaveTableName;
	tdsTableName  tableName; //option

	BOOL bHaveCryptMetaData;
	tdsCryptoMetaData  CryptoMetaData;  //option

	tdsCountVarValue<BYTE,WCHAR>  ColumnName;

// by jie 2018.05.11
public:
	WCHAR* GetPColumnNameBegin() const { return ColumnName.GetPValue(); }
	BYTE GetCloumnNameCount() const { return ColumnName.GetCount(); }
};

#endif //end of TDS_COLUMN_DATA_INFO
