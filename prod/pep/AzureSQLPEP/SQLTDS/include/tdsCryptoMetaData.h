#ifndef TDS_CRYPTO_META_DATA_H
#define TDS_CRYPTO_META_DATA_H

#include <windows.h>
#include "tdsTypeInfo.h"
#include "tdsCountVarValue.h"

class tdsCryptoMetaData
{
public:
	tdsCryptoMetaData(void);
	~tdsCryptoMetaData(void);

public:
	DWORD CBSize() { return sizeof(Ordinal) +
							sizeof(UserType) +
							BaseTypeInfo.CBSize() +
							sizeof(EncryptionAlgo) +
							AlgoName.CBSize() +
							sizeof(EncryptionAlgoType) +
							sizeof(NormVersion); }

private:
	USHORT Ordinal;
	ULONG  UserType;
	tdsTypeInfo BaseTypeInfo;
	BYTE 	EncryptionAlgo;
	tdsCountVarValue<BYTE,WCHAR> AlgoName;
	BYTE EncryptionAlgoType;
	BYTE NormVersion;

	/*
	CryptoMetaData  =   Ordinal              ; (CryptoMetaData introduced in TDS 7.4)
						UserType
						BaseTypeInfo
						EncryptionAlgo
						[AlgoName]
						EncryptionAlgoType
						NormVersion
	*/

};

#endif 

