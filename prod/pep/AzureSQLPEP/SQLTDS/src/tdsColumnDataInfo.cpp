#include "tdsColumnDataInfo.h"
#include "tdsTypeHelper.h"


tdsColumnDataInfo::tdsColumnDataInfo(void)
{
	bHaveTableName = FALSE;
	bHaveCryptMetaData = FALSE;
}

tdsColumnDataInfo::tdsColumnDataInfo(PBYTE pBuf)
{
	bHaveTableName = FALSE;
	bHaveCryptMetaData = FALSE;
	Parse(pBuf);
}


tdsColumnDataInfo::~tdsColumnDataInfo(void)
{
}

void tdsColumnDataInfo::Parse(PBYTE pBuf)
{
	PBYTE p= pBuf;

	UserType = *(ULONG*)p;
	p += sizeof(UserType);

	memcpy(Flags, p, sizeof(Flags));
	p+=sizeof(Flags);

    TypeInfo.Parse(p);
	p += TypeInfo.CBSize();

   bHaveTableName = tdsTypeHelper::HaveTableNameInMetadata(TypeInfo.GetTdsDataType());
   if (bHaveTableName)
   {
	   tableName.Parse(p);
	   p += tableName.CBSize();
   }


   ColumnName.Parse(p);


}

void tdsColumnDataInfo::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(UserType);
	rBuf.ReadData(Flags, sizeof(Flags));
	TypeInfo.Parse(rBuf);
	bHaveTableName = tdsTypeHelper::HaveTableNameInMetadata(TypeInfo.GetTdsDataType());
	if (bHaveTableName)
	{
		tableName.Parse(rBuf);
	}
	ColumnName.Parse(rBuf);
}


int  tdsColumnDataInfo::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{
	cbData = 0;
	PBYTE p = pBuf;

	*(ULONG*)p = UserType;
	p			+= sizeof(UserType);
	dwBufLen	-= sizeof(UserType);
	cbData		+= sizeof(UserType);

	memcpy(p, Flags, sizeof(Flags));
	p			+= sizeof(Flags);
	dwBufLen	-= sizeof(Flags);
	cbData		+= sizeof(Flags);

	DWORD a = 0;
	TypeInfo.Serialize(p, dwBufLen, a);
	p			+= TypeInfo.CBSize();
	dwBufLen	-= TypeInfo.CBSize();
	cbData		+= TypeInfo.CBSize();

	if (bHaveTableName)
	{
		tableName.Serialize(p, dwBufLen, a);
		p += tableName.CBSize();
		dwBufLen -= tableName.CBSize();
		cbData += tableName.CBSize();
	}

	ColumnName.Serialize(p, dwBufLen, a);
	p			+= ColumnName.CBSize();
	dwBufLen	-= ColumnName.CBSize();
	cbData		+= ColumnName.CBSize();

	return 0;

}

int tdsColumnDataInfo::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(UserType);
	wBuf.WriteData(Flags, sizeof(Flags));
	TypeInfo.Serialize(wBuf);
	if (bHaveTableName)
	{
		tableName.Serialize(wBuf);
	}
	ColumnName.Serialize(wBuf);
	return 0;
}
