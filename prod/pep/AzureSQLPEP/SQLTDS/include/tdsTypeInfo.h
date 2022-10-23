#ifndef TDS_TYPE_INFO_H
#define TDS_TYPE_INFO_H
#include <windows.h>
#include "tdsTypeHelper.h"
#include "MyBuffer.h"

class  BIT8
{
	BYTE b[8];
};

typedef BIT8 PRECISION;
typedef BIT8 SCALE;

class COLLATION
{
public:
	COLLATION(PBYTE pBuf)
	{
		Parse(pBuf);
	}
	COLLATION(void){}
	~COLLATION(){}

public:
	DWORD GetLCID() { return (*(DWORD*)LcidFlagVer)&0x000fffff;} 
	BOOL bIgnoreCase(){return ((*(DWORD*)LcidFlagVer)&0x00100000)!=0;}
	
	DWORD CBSize() { return sizeof(LcidFlagVer) + sizeof(SortId); }
	void Parse(PBYTE pBuf)
	{
		PBYTE p = pBuf;

		memcpy(LcidFlagVer, p, sizeof(LcidFlagVer));
		p += sizeof(LcidFlagVer);

		SortId = *p;
	}
	void Parse(ReadBuffer& rBuf)
	{
		rBuf.ReadData(LcidFlagVer, sizeof(LcidFlagVer));
		rBuf.ReadAny(SortId);
	}

	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
	{
		cbData = 0;
		PBYTE p = pBuf;
		memcpy(p, LcidFlagVer, sizeof(LcidFlagVer));
		p += sizeof(LcidFlagVer);
		dwBufLen -= sizeof(LcidFlagVer);
		cbData += sizeof(LcidFlagVer);

		*p = SortId;
		p += sizeof(SortId);
		dwBufLen -= sizeof(SortId);
		cbData += sizeof(SortId);

		return 0;
	}

	int Serialize(WriteBuffer& wBuf)
	{
		wBuf.WriteData(LcidFlagVer, sizeof(LcidFlagVer));
		wBuf.WriteAny(SortId);
		return 0;
	}


private:
	BYTE LcidFlagVer[4];
    BYTE SortId;
	/*
	LCID             =   20BIT

	fIgnoreCase      =   BIT
	fIgnoreAccent    =   BIT
	fIgnoreWidth     =   BIT
	fIgnoreKana      =   BIT
	fBinary          =   BIT
	fBinary2         =   BIT
	ColFlags         =   fIgnoreCase fIgnoreAccent fIgnoreKana
	                     fIgnoreWidth fBinary fBinary2 FRESERVEDBIT
	                     FRESERVEDBIT
	Version          =   4BIT
	SortId           =   BYTE

	COLLATION        =   LCID ColFlags Version SortId
   */
};

class tdsTypeInfo
{
public:
	tdsTypeInfo(void);
	tdsTypeInfo(PBYTE pBuf);
	~tdsTypeInfo(void);


public:
	TDS_DATA_TYPE GetTdsDataType() { return tdsType; }
	void Parse(PBYTE pBuf);
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize()
	{
		return sizeof(type) +  
			   (bFixedLenType ? 0 : lenType==BYTE_LEN_TYPE ? 1 : (lenType==USHORT_LEN_TYPE ? 2 : 4) ) +
			   (bHaveCollation ? Collation.CBSize() : 0) +
			   (bHavePrecision ? sizeof(Precision) : 0) +
			   (bHaveScale     ? sizeof(Scale)     : 0);
			  
	}

	TDS_LEN_TYPE GetLenType(){ return lenType; }
	BOOL IsFixedLenType() { return bFixedLenType; }

	int  Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData);
	int Serialize(WriteBuffer& wBuf);

private:
	


private:
	BYTE type; //fix-len or var-len type
	TDS_DATA_TYPE  tdsType;//convert from type 
	BOOL bFixedLenType;

	union 
	{
		BYTE   BYTELEN;
		USHORT USHORTCHARBINLEN;
		LONG  LONGLEN;
	}TYPE_VARLEN;
	TDS_LEN_TYPE lenType;

	BOOL bHaveCollation;
	COLLATION  Collation;

	BOOL bHavePrecision;
	PRECISION  Precision;

	BOOL bHaveScale;
	SCALE      Scale;


	/*	USHORTMAXLEN     =   %xFFFF

	TYPE_INFO        =   FIXEDLENTYPE 
	/
	(VARLENTYPE TYPE_VARLEN [COLLATION])
	/
	(VARLENTYPE TYPE_VARLEN [PRECISION SCALE])
	/
	(VARLENTYPE SCALE) ; (introduced in TDS 7.3)
	/
	VARLENTYPE         ; (introduced in TDS 7.3)
	/
	(PARTLENTYPE 
	[USHORTMAXLEN] 
	[COLLATION] 
	[XML_INFO] 
	[UDT_INFO])
   */
};

#endif //TDS_TYPE_INFO_H

