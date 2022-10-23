#ifndef TDS_TYPE_HELPER_H
#define TDS_TYPE_HELPER_H
#include <windows.h>

enum TDS_DATA_TYPE
{
	UNKNOW_TYPE = 0x00,
	//fixed-length data types
	/*
	NULLTYPE         =   %x1F  ; Null 
	INT1TYPE         =   %x30  ; TinyInt 
	BITTYPE          =   %x32  ; Bit 
	INT2TYPE         =   %x34  ; SmallInt 
	INT4TYPE         =   %x38  ; Int 
	DATETIM4TYPE     =   %x3A  ; SmallDateTime 
	FLT4TYPE         =   %x3B  ; Real 
	MONEYTYPE        =   %x3C  ; Money 
	DATETIMETYPE     =   %x3D  ; DateTime 
	FLT8TYPE         =   %x3E  ; Float 
	MONEY4TYPE       =   %x7A  ; SmallMoney 
	INT8TYPE         =   %x7F  ; BigInt 
	*/
	NULLTYPE=0x1F,
	INT1TYPE=0x30,
	BITTYPE =0x32,
	INT2TYPE = 0x34,
	INT4TYPE = 0x38,
	DATETIM4TYPE = 0x3A,
	FLT4TYPE  =0x3B,
	MONEYTYPE = 0x3C,
	DATETIMETYPE =0x3D,
	FLT8TYPE  = 0x3E,
	MONEY4TYPE = 0x7A,
	INT8TYPE   = 0x7F,

	//2.2.5.4.2	Variable-Length Data Types
	/*
	GUIDTYPE            =   %x24  ; UniqueIdentifier
	INTNTYPE            =   %x26  ; (see below)
	DECIMALTYPE         =   %x37  ; Decimal (legacy support)
	NUMERICTYPE         =   %x3F  ; Numeric (legacy support)
	BITNTYPE            =   %x68  ; (see below)
	DECIMALNTYPE        =   %x6A  ; Decimal
	NUMERICNTYPE        =   %x6C  ; Numeric
	FLTNTYPE            =   %x6D  ; (see below)
	MONEYNTYPE          =   %x6E  ; (see below)
	DATETIMNTYPE        =   %x6F  ; (see below)
	DATENTYPE           =   %x28  ; (introduced in TDS 7.3)
	TIMENTYPE           =   %x29  ; (introduced in TDS 7.3)
	DATETIME2NTYPE      =   %x2A  ; (introduced in TDS 7.3)
	DATETIMEOFFSETNTYPE =   %x2B  ; (introduced in TDS 7.3)
	CHARTYPE            =   %x2F  ; Char (legacy support)
	VARCHARTYPE         =   %x27  ; VarChar (legacy support)
	BINARYTYPE          =   %x2D  ; Binary (legacy support)
	VARBINARYTYPE       =   %x25  ; VarBinary (legacy support)

	BIGVARBINTYPE       =   %xA5  ; VarBinary
	BIGVARCHRTYPE       =   %xA7  ; VarChar
	BIGBINARYTYPE       =   %xAD  ; Binary
	BIGCHARTYPE         =   %xAF  ; Char
	NVARCHARTYPE        =   %xE7  ; NVarChar
	NCHARTYPE           =   %xEF  ; NChar
	XMLTYPE             =   %xF1  ; XML (introduced in TDS 7.2)
	UDTTYPE             =   %xF0  ; CLR UDT (introduced in TDS 7.2)

	TEXTTYPE            =   %x23  ; Text
	IMAGETYPE           =   %x22  ; Image
	NTEXTTYPE           =   %x63  ; NText
	SSVARIANTTYPE       =   %x62  ; Sql_Variant (introduced in TDS 7.2)
   */
   GUIDTYPE            =   0x24, // ; UniqueIdentifier
   INTNTYPE            =   0x26, //  ; (see below)
   DECIMALTYPE         =   0x37, //  ; Decimal (legacy support)
   NUMERICTYPE         =   0x3F, //  ; Numeric (legacy support)
   BITNTYPE            =   0x68, //  ; (see below)
   DECIMALNTYPE        =   0x6A, //  ; Decimal
   NUMERICNTYPE        =   0x6C, //  ; Numeric
   FLTNTYPE            =   0x6D, //  ; (see below)
   MONEYNTYPE          =   0x6E, //  ; (see below)
   DATETIMNTYPE        =   0x6F, //  ; (see below)
   DATENTYPE           =   0x28, //  ; (introduced in TDS 7.3)
   TIMENTYPE           =   0x29, //  ; (introduced in TDS 7.3)
   DATETIME2NTYPE      =   0x2A, //  ; (introduced in TDS 7.3)
   DATETIMEOFFSETNTYPE =   0x2B, //  ; (introduced in TDS 7.3)
   CHARTYPE            =   0x2F, //  ; Char (legacy support)
   VARCHARTYPE         =   0x27, //  ; VarChar (legacy support)
   BINARYTYPE          =   0x2D, //  ; Binary (legacy support)
   VARBINARYTYPE       =   0x25, //  ; VarBinary (legacy support)

   BIGVARBINTYPE       =   0xA5, //  ; VarBinary
   BIGVARCHRTYPE       =   0xA7, //  ; VarChar
   BIGBINARYTYPE       =   0xAD, //  ; Binary
   BIGCHARTYPE         =   0xAF, //  ; Char
   NVARCHARTYPE        =   0xE7, //  ; NVarChar
   NCHARTYPE           =   0xEF, //  ; NChar
   XMLTYPE             =   0xF1, //  ; XML (introduced in TDS 7.2)
   UDTTYPE             =   0xF0, //  ; CLR UDT (introduced in TDS 7.2)

   TEXTTYPE            =   0x23, //  ; Text
   IMAGETYPE           =   0x22, //  ; Image
   NTEXTTYPE           =   0x63, //  ; NText
   SSVARIANTTYPE       =   0x62, //  ; Sql_Variant (introduced in TDS 7.2)
};

enum TDS_LEN_TYPE
{
	UNKNOW_LEN_TYPE=0x0,
	BYTE_LEN_TYPE=0x1,
	USHORT_LEN_TYPE=0x2,
	LONG_LEN_TYPE=0x4    
};

enum TDS_NULL_TYPE
{
	GEN_NULL = 1,
	CHARBIN_NULL_2 = 2,
	CHARBIN_NULL_4 = 4,
	PLP_NULL = 8
};

enum TDS_ENVCHANGE_TOKEN_TYPE
{
	DATABASE = 1,
	LANGUAGE = 2,
	CHARACTER_SET = 3,
	PACKET_SIZE = 4,
	UNICODE_DATA_SORTING_LOCAL_ID = 5,
	UNICODE_DATA_SORTING_COMPARISON_FLAGS = 6,
	SQL_COLLATION = 7,
	BEGIN_TRANSACTION = 8,
	COMMIT_TRANSACTION = 9,
	ROLLBACK_TRANSACTION = 10,
	ENLIST_DTC_TRANSACTION = 11,
	DEFECT_TRANSACTION = 12,
	REAL_TIME_LOG_SHIPPING = 13,
	PROMOTE_TRANSACTION = 15,
	TRANSACTION_MANAGER_ADDRESS = 16,
	TRANSACTION_ENDED = 17,
	RESETCONNECTION_OR_RESETCONNECTIONSKIPTRAN_COMPLETION_ACKNOWLEDGEMENT = 18,
	SEND_BACK_NAME_OF_USER_INSTANCE_STARTED_PER_LOGIN_REQUEST = 19,
	SENDS_ROUTING_INFORMATION_TO_CLIENT = 20,
};

class tdsTypeHelper
{
public:
	tdsTypeHelper(void);
	~tdsTypeHelper(void);

public:
	static BOOL HaveTableNameInMetadata(TDS_DATA_TYPE tdsType);
	static BOOL IsFixedLenType(TDS_DATA_TYPE tdsType);
	static BYTE FixedTypeLen(TDS_DATA_TYPE tdsType);
	static TDS_LEN_TYPE GetLengthType(TDS_DATA_TYPE tdsType);
	static BOOL HaveCollation(TDS_DATA_TYPE tdsType);
	static BOOL HavePrecision(TDS_DATA_TYPE tdsType);
	static BOOL HaveScale(TDS_DATA_TYPE tdsType);
	static BOOL IsUnicode16Type(TDS_DATA_TYPE tdsType);
	static BOOL HaveTextPoint(TDS_DATA_TYPE tdsType);
	static TDS_DATA_TYPE GetTdsDataType(BYTE bType);
	static BOOL IsMultiByte(TDS_DATA_TYPE);
};

#endif 

