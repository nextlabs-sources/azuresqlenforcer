#include "tdsEK_INFO.h"

tdsEncryptionKeyValue::tdsEncryptionKeyValue(void)
{

}

tdsEncryptionKeyValue::tdsEncryptionKeyValue(PBYTE pBuf)
{
	Parse(pBuf);
}

tdsEncryptionKeyValue::~tdsEncryptionKeyValue()
{

}

void tdsEncryptionKeyValue::Parse(PBYTE pBuf)
{
	PBYTE p = pBuf;

	EncryptedKey.Parse(p);
	p += EncryptedKey.CBSize();

	KeyStoreName.Parse(p);
	p += KeyStoreName.CBSize();

    KeyPath.Parse(p);
	p += KeyPath.CBSize();

	AsymmetricAlgo.Parse(p);
	p += AsymmetricAlgo.CBSize();
}


tdsEK_INFO::tdsEK_INFO(void)
{
	Empty();
}

tdsEK_INFO::tdsEK_INFO(PBYTE pBuf)
{
	Parse(pBuf);
}


tdsEK_INFO::~tdsEK_INFO(void)
{
	if (Count)
	{
		delete[] pEncryptionKeyValue;
	}
	Empty();
}

void tdsEK_INFO::Parse(PBYTE pBuf)
{
	PBYTE p=pBuf;

	DatabaseID = *(ULONG*)p;
	p += sizeof(DatabaseID);

	CekId = *(ULONG*)p;
	p += sizeof(CekId);

	CekVersion = *(ULONG*)p;
	p += sizeof(CekVersion);

	CekMDVersion = *(ULONGLONG*)p;
	p += sizeof(CekMDVersion);

	Count = *p;

	pEncryptionKeyValue = 0;
	if (Count)
	{
		pEncryptionKeyValue = new tdsEncryptionKeyValue[Count];
		for (int i=0; i<Count; i++)
		{
			pEncryptionKeyValue[i].Parse(p);
			p += pEncryptionKeyValue[i].CBSize();
		}
	}
}

void tdsEK_INFO::Empty()
{
	DatabaseID = 0;
	CekId = 0;
	CekVersion = 0;
	CekMDVersion = 0;
	Count = 0;;
	pEncryptionKeyValue = NULL;
}