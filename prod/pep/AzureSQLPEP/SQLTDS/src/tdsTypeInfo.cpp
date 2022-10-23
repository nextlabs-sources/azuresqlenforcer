#include "tdsTypeInfo.h"
#include "tdsTypeHelper.h"
#include <windows.h>
#include <assert.h>


tdsTypeInfo::tdsTypeInfo(void)
{
	bHaveCollation = FALSE;
	bHavePrecision = FALSE;
	bHaveScale = FALSE;
	bFixedLenType= TRUE;
}

tdsTypeInfo::tdsTypeInfo(PBYTE pBuf)
{
	bHaveCollation = FALSE;
	bHavePrecision = FALSE;
	bHaveScale = FALSE;
	bFixedLenType= TRUE;
	Parse(pBuf);
}

tdsTypeInfo::~tdsTypeInfo(void)
{
}


void tdsTypeInfo::Parse(PBYTE pBuf)
{
	PBYTE p = pBuf;

	type = *p;
	p += sizeof(type);

	tdsType = tdsTypeHelper::GetTdsDataType(type);
	if (UNKNOW_TYPE==tdsType)
	{
		assert(FALSE);
		return;
	}

	bFixedLenType = tdsTypeHelper::IsFixedLenType(tdsType);
	if (!bFixedLenType)
	{
		lenType = tdsTypeHelper::GetLengthType(tdsType);
		switch (lenType)
		{
		case BYTE_LEN_TYPE:
			TYPE_VARLEN.BYTELEN = *p;
			p += sizeof(TYPE_VARLEN.BYTELEN);
			break;

		case USHORT_LEN_TYPE:
			TYPE_VARLEN.USHORTCHARBINLEN = *(USHORT*)p;
			p += sizeof(TYPE_VARLEN.USHORTCHARBINLEN);
			break;

		case LONG_LEN_TYPE:
			TYPE_VARLEN.LONGLEN = *(LONG*)p;
			p += sizeof(TYPE_VARLEN.LONGLEN);
			break;

		default:
			DebugBreak();
			break;
		}
	}

	bHaveCollation = tdsTypeHelper::HaveCollation(tdsType);
	if (bHaveCollation)
	{
		Collation.Parse(p);
		p += Collation.CBSize();
	}

	bHavePrecision = tdsTypeHelper::HavePrecision(tdsType);
	if (bHavePrecision)
	{
	    memcpy(&Precision, p, sizeof(Precision));
		p += sizeof(Precision);
	}

	bHaveScale = tdsTypeHelper::HaveScale(tdsType);
	if (bHaveScale)
	{
		memcpy(&Scale, p, sizeof(Scale));
		p += sizeof(Scale);
	}
}

void tdsTypeInfo::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(type);
	tdsType = tdsTypeHelper::GetTdsDataType(type);
	if (UNKNOW_TYPE == tdsType)
	{
		assert(FALSE);
		return;
	}
	bFixedLenType = tdsTypeHelper::IsFixedLenType(tdsType);
	if (!bFixedLenType)
	{
		lenType = tdsTypeHelper::GetLengthType(tdsType);
		switch (lenType)
		{
		case BYTE_LEN_TYPE:
			rBuf.ReadAny(TYPE_VARLEN.BYTELEN);
			break;
		case USHORT_LEN_TYPE:
			rBuf.ReadAny(TYPE_VARLEN.USHORTCHARBINLEN);
			break;
		case LONG_LEN_TYPE:
			rBuf.ReadAny(TYPE_VARLEN.LONGLEN);
			break;
		default:
			break;
		}
	}
	bHaveCollation = tdsTypeHelper::HaveCollation(tdsType);
	if (bHaveCollation)
	{
		Collation.Parse(rBuf);
	}
	bHavePrecision = tdsTypeHelper::HavePrecision(tdsType);
	if (bHavePrecision)
	{
		rBuf.ReadAny(Precision);
	}
	bHaveScale = tdsTypeHelper::HaveScale(tdsType);
	if (bHaveScale)
	{
		rBuf.ReadData(&Scale, sizeof(Scale));
	}
}


int  tdsTypeInfo::Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
{
	cbData = 0;
	PBYTE p = pBuf;

	*p = type;
	p			+= sizeof(type);
	dwBufLen	-= sizeof(type);
	cbData		+= sizeof(type);

	if (!bFixedLenType)
	{
		switch (lenType)
		{
		case UNKNOW_LEN_TYPE:
			DebugBreak();
			break;
		case BYTE_LEN_TYPE:
			*p = TYPE_VARLEN.BYTELEN;
			p			+= sizeof(TYPE_VARLEN.BYTELEN);
			dwBufLen	-= sizeof(TYPE_VARLEN.BYTELEN);
			cbData		+= sizeof(TYPE_VARLEN.BYTELEN);
			break;
		case USHORT_LEN_TYPE:
			*(USHORT*)p = TYPE_VARLEN.USHORTCHARBINLEN;
			p			+= sizeof(TYPE_VARLEN.USHORTCHARBINLEN);
			dwBufLen	-= sizeof(TYPE_VARLEN.USHORTCHARBINLEN);
			cbData		+= sizeof(TYPE_VARLEN.USHORTCHARBINLEN);
			break;
		case LONG_LEN_TYPE:
			*(LONG*)p = TYPE_VARLEN.LONGLEN;
			p			+= sizeof(TYPE_VARLEN.LONGLEN);
			dwBufLen	-= sizeof(TYPE_VARLEN.LONGLEN);
			cbData		+= sizeof(TYPE_VARLEN.LONGLEN);
			break;
		default:
			DebugBreak();
			break;
		}
	}

	DWORD a = 0;
	if (bHaveCollation)
	{
		Collation.Serialize(p, dwBufLen, a);
		p			+= Collation.CBSize();
		dwBufLen	-= Collation.CBSize();
		cbData		+= Collation.CBSize();
	}

	if (bHavePrecision)
	{
		memcpy(p, &Precision, sizeof(Precision));
		p			+= sizeof(Precision);
		dwBufLen	-= sizeof(Precision);
		cbData		+= sizeof(Precision);
	}

	if (bHaveScale)
	{
		memcpy(p, &Scale, sizeof(Scale));
		p			+= sizeof(Scale);
		dwBufLen	-= sizeof(Scale);
		cbData		+= sizeof(Scale);
	}

	return 0;
}

int tdsTypeInfo::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(type);
	if (!bFixedLenType)
	{
		switch (lenType)
		{
		case UNKNOW_LEN_TYPE:
			DebugBreak();
			break;
		case BYTE_LEN_TYPE:
			wBuf.WriteAny(TYPE_VARLEN.BYTELEN);
			break;
		case USHORT_LEN_TYPE:
			wBuf.WriteAny(TYPE_VARLEN.USHORTCHARBINLEN);
			break;
		case LONG_LEN_TYPE:
			wBuf.WriteAny(TYPE_VARLEN.LONGLEN);
			break;
		default:
			DebugBreak();
			break;
		}
	}

	if (bHaveCollation)
	{
		Collation.Serialize(wBuf);
	}

	if (bHavePrecision)
	{
		wBuf.WriteData(&Precision, sizeof(Precision));
	}

	if (bHaveScale)
	{
		wBuf.WriteData(&Scale, sizeof(Scale));
	}
	return 0;
}