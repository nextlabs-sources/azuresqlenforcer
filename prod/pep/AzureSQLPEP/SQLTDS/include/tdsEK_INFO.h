#ifndef TDS_EK_INFO_H
#define TDS_EK_INFO_H
#include <windows.h>
#include "tdsCountVarValue.h"


class tdsEncryptionKeyValue
{
public:
	tdsEncryptionKeyValue(PBYTE pBuf);
	tdsEncryptionKeyValue(void);
	~tdsEncryptionKeyValue();

public:
	void Parse(PBYTE pBuf);
	DWORD CBSize(){ return EncryptedKey.CBSize() +
							KeyStoreName.CBSize() +
							KeyPath.CBSize() +
							AsymmetricAlgo.CBSize(); }


private:
	tdsCountVarValue<USHORT,BYTE>  EncryptedKey;
	tdsCountVarValue<BYTE, CHAR>   KeyStoreName;
	tdsCountVarValue<USHORT, CHAR>  KeyPath;
	tdsCountVarValue<BYTE,CHAR>  AsymmetricAlgo;
};
	

class tdsEK_INFO
{
public:
	tdsEK_INFO(void);
	tdsEK_INFO(PBYTE pBuf);
	~tdsEK_INFO(void);

public:
	void Parse(PBYTE pBuf);

private:
	void Empty();

private:
	ULONG  DatabaseID;
	ULONG  CekId;
	ULONG  CekVersion;
	ULONGLONG CekMDVersion;
	BYTE   Count;
	tdsEncryptionKeyValue* pEncryptionKeyValue;
};

/*
	Count                 =   BYTE

	EncryptedKey          =   US_VARBYTES

	KeyStoreName          =   B_VARCHAR

	KeyPath               =   US_VARCHAR

	AsymmetricAlgo        =   B_VARCHAR

	EncryptionKeyValue    =   EncryptedKey
							  KeyStoreName
							  KeyPath
	                          AsymmetricAlgo

	DatabaseId            =   ULONG

	CekId                 =   ULONG

	CekVersion            =   ULONG

	CekMDVersion          =   ULONGLONG


	EK_INFO               =   DatabaseId
							  CekId
							  CekVersion
								CekMDVersion
							  Count
							*EncryptionKeyValue
*/

#endif 

