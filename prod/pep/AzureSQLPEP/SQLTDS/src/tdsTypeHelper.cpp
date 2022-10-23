#include "tdsTypeHelper.h"
#include <stdio.h>

tdsTypeHelper::tdsTypeHelper(void)
{
}


tdsTypeHelper::~tdsTypeHelper(void)
{
}

BOOL tdsTypeHelper::HaveTableNameInMetadata(TDS_DATA_TYPE tdsType)
{
	//text, ntext, or image 
	return tdsType==TEXTTYPE || tdsType==NTEXTTYPE || tdsType==IMAGETYPE;
}

BOOL tdsTypeHelper::IsFixedLenType(TDS_DATA_TYPE type)
{
	 return  NULLTYPE==type ||
		INT1TYPE==type ||
		BITTYPE==type ||
		INT2TYPE==type ||
		INT4TYPE==type ||
		DATETIM4TYPE==type ||
		FLT4TYPE==type ||
		MONEYTYPE==type ||
		DATETIMETYPE==type ||
		FLT8TYPE==type ||
		MONEY4TYPE==type ||
		INT8TYPE==type;
}

BYTE tdsTypeHelper::FixedTypeLen(TDS_DATA_TYPE tdsType)
{
	switch (tdsType)
	{
	case NULLTYPE:
		return 0;
		break;
	case INT1TYPE:
	case BITTYPE:
		return 1;
		break;
	case INT2TYPE:
		return 2;
		break;
	case INT4TYPE:
	case DATETIM4TYPE:
	case FLT4TYPE:
	case MONEY4TYPE:
		return 4;
		break;
	case MONEYTYPE:
	case DATETIMETYPE:
	case FLT8TYPE:
	case INT8TYPE:
		return 8;
		break;
	default:
	{
        return -1;
	}
	break;
	}
}

TDS_LEN_TYPE tdsTypeHelper::GetLengthType(TDS_DATA_TYPE tdsType)
{
	switch (tdsType)
	{
	case GUIDTYPE:
	case INTNTYPE:
	case DECIMALTYPE:
	case NUMERICTYPE:
	case BITNTYPE:
	case DECIMALNTYPE:
	case NUMERICNTYPE:
	case FLTNTYPE:
	case MONEYNTYPE:
	case DATETIMNTYPE:
	case DATENTYPE:
	case TIMENTYPE:
	case DATETIME2NTYPE:
	case DATETIMEOFFSETNTYPE:
	case CHARTYPE:
	case VARCHARTYPE:
	case BINARYTYPE:
	case VARBINARYTYPE:
		return BYTE_LEN_TYPE;
		break;

	case BIGVARBINTYPE:
	case BIGVARCHRTYPE:
	case BIGBINARYTYPE:
	case BIGCHARTYPE:
	case NVARCHARTYPE:
	case NCHARTYPE:
		return USHORT_LEN_TYPE;
		break;

	case IMAGETYPE:
    case NTEXTTYPE:
	case SSVARIANTTYPE:
	case TEXTTYPE:
	case XMLTYPE:
		return LONG_LEN_TYPE;
		break;

	default:
		DebugBreak();
		break;
	}

	return UNKNOW_LEN_TYPE;
}

TDS_DATA_TYPE tdsTypeHelper::GetTdsDataType(BYTE bType)
{
	switch (bType)
	{
	case NULLTYPE:
	case INT1TYPE:
	case BITTYPE:
	case INT2TYPE:
	case INT4TYPE:
	case DATETIM4TYPE:
	case FLT4TYPE:
	case MONEYTYPE:
	case DATETIMETYPE:
	case FLT8TYPE:
	case MONEY4TYPE:
	case INT8TYPE:
	case GUIDTYPE:
	case INTNTYPE:
	case DECIMALTYPE:
	case NUMERICTYPE:
	case BITNTYPE:
	case DECIMALNTYPE:
	case NUMERICNTYPE:
	case FLTNTYPE:
	case MONEYNTYPE:
	case DATETIMNTYPE:
	case DATENTYPE:
	case TIMENTYPE:
	case DATETIME2NTYPE:
	case DATETIMEOFFSETNTYPE:
	case CHARTYPE:
	case VARCHARTYPE:
	case BINARYTYPE:
	case VARBINARYTYPE:
	case BIGVARBINTYPE:
	case BIGVARCHRTYPE:
	case BIGBINARYTYPE:
	case BIGCHARTYPE:
	case NVARCHARTYPE:
	case NCHARTYPE:
	case XMLTYPE:
	case UDTTYPE:
	case TEXTTYPE:
	case IMAGETYPE:
	case NTEXTTYPE:
	case SSVARIANTTYPE:
        return (TDS_DATA_TYPE)bType;
		break;

	default:
		return UNKNOW_TYPE;
		break;
	}

	//return UNKNOW_TYPE;
}

BOOL tdsTypeHelper::HaveCollation(TDS_DATA_TYPE tdsType)
{
	return tdsType==BIGCHARTYPE ||
		tdsType==BIGVARCHRTYPE ||
		tdsType==TEXTTYPE || 
		tdsType==NTEXTTYPE||
		tdsType==NCHARTYPE||
		tdsType==NVARCHARTYPE;
}

BOOL tdsTypeHelper::HavePrecision(TDS_DATA_TYPE tdsType)
{
	return tdsType==NUMERICTYPE ||
		tdsType==NUMERICNTYPE ||
		tdsType==DECIMALTYPE ||
		tdsType==DECIMALNTYPE;
}

BOOL tdsTypeHelper::HaveScale(TDS_DATA_TYPE tdsType)
{
	return HavePrecision(tdsType) ||
		   tdsType==TIMENTYPE ||
		   tdsType==DATETIME2NTYPE ||
		   tdsType==DATETIMEOFFSETNTYPE; 
}

BOOL tdsTypeHelper::IsUnicode16Type(TDS_DATA_TYPE tdsType)
{
	//from https://docs.microsoft.com/en-us/sql/relational-databases/collations/collation-and-unicode-support?view=sql-server-2017
	// If you store character data that reflects multiple languages, always use Unicode data types (nchar, nvarchar, and ntext) instead of the non-Unicode data types (char, varchar, and text).
	return tdsType==NCHARTYPE ||
		tdsType==NVARCHARTYPE ||
		tdsType==NTEXTTYPE;
}

BOOL tdsTypeHelper::HaveTextPoint(TDS_DATA_TYPE tdsType)
{
	return tdsType==TEXTTYPE ||
		  tdsType==NTEXTTYPE ||
		  tdsType==IMAGETYPE; 
}

BOOL tdsTypeHelper::IsMultiByte(TDS_DATA_TYPE tdsType)
{
	//from https://docs.microsoft.com/en-us/sql/relational-databases/collations/collation-and-unicode-support?view=sql-server-2017
	return CHARTYPE == tdsType ||
		VARCHARTYPE == tdsType ||
		BIGVARCHRTYPE == tdsType ||
		TEXTTYPE == tdsType ||
		BIGCHARTYPE == tdsType;
}