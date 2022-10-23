#include <WinSock2.h>
#include "tdsLogin7.h"
#include "tdsSSPI.h"
#include "tdsHelper.h"
#include "CommonFunc.h"
#include "Log.h"

tdsLogin7::tdsLogin7(const std::list<PBYTE>& packs)
{
    Length = 0;
    TDSVersion = 0;
    PacketSize = 0;
    ClientProgVer = 0;
    ClientPID = 0;
    ConnectionID = 0;

    OptionFlags1 = 0;
    OptionFlags2 = 0;
    OptionFlags3 = 0;
    typeFlag = 0;

    ClientTimeZone = 0;
    ClientLCID = 0;

    memset(HostName, 0, sizeof(HostName));
    memset(UserName, 0, sizeof(UserName));
    memset(Password, 0, sizeof(Password));
    memset(AppName, 0, sizeof(AppName));
    memset(ServerName, 0, sizeof(ServerName));

    bExtensionUsed = FALSE;
    memset(Extension, 0, sizeof(Extension));
    cbExtension = 0;

    memset(CltIntName, 0, sizeof(CltIntName));
    memset(Language, 0, sizeof(Language));
    memset(DataBase, 0, sizeof(DataBase));
    memset(ClientID, 0, sizeof(ClientID));

    pSSPI = NULL;
    cbSSPI = 0;

    memset(AtchDBFile, 0, sizeof(AtchDBFile));
    bChangePassword = FALSE;
    memset(ChangePassword, 0, sizeof(ChangePassword));
    SSPILong = 0;

    FeatureDataLen = 0;
    memset(FeatureData, 0, sizeof(FeatureData));

    if (packs.size() == 0)
        return;

    __super::Parse(packs.front());
	Parse(packs);
}


tdsLogin7::~tdsLogin7(void)
{
    if (pSSPI) {
        delete[] pSSPI;
        pSSPI = NULL;
    }
}

std::wstring tdsLogin7::ClientID2String()
{
    WCHAR wstr[30] = { 0 };
    wsprintfW(wstr, L"%02X-%02X-%02X-%02X-%02X-%02X", ClientID[0], ClientID[1], ClientID[2], ClientID[3], ClientID[4], ClientID[5]);

    return std::wstring(wstr);
}

void tdsLogin7::Parse(const std::list<PBYTE>& packs)
{
    if (packs.size() == 1)
    {
        Parse(packs.front(), TDS::GetTdsPacketLength(packs.front()));
    }
    else
    {
        uint32_t total_len = 0;
        for (auto it : packs)
        {
            if (total_len == 0) {
                total_len = TDS::GetTdsPacketLength(it);
                continue;
            }

            total_len += TDS::GetTdsPacketLength(it) - TDS_HEADER_LEN;
        }

        uint32_t offset = 0;
        PBYTE buff = new BYTE[total_len];
        for (auto it : packs)
        {
            if (offset == 0)
            {
                memcpy(buff, it, TDS::GetTdsPacketLength(it));
                offset = TDS::GetTdsPacketLength(it);
                continue;
            }

            memcpy(buff + offset, it + TDS_HEADER_LEN, TDS::GetTdsPacketLength(it) - TDS_HEADER_LEN);
            offset += TDS::GetTdsPacketLength(it) - TDS_HEADER_LEN;
        }

        Parse(buff, total_len);

        delete[] buff;
        buff = nullptr;
    }
}

void tdsLogin7::Parse(PBYTE pBuf, uint32_t len)
{
	PBYTE p = pBuf;

	//offset header
	p += TDS_HEADER_LEN;


	//copy the first part of data.
	//because we didn't modify this part of data. so we just copy it from buffer
	Length = *(DWORD*)p;
	p += sizeof(DWORD);

	TDSVersion = *(DWORD*)p;
	p += sizeof(DWORD);

	PacketSize =  *(DWORD*)p;
	p += sizeof(DWORD);

	ClientProgVer =  *(DWORD*)p;
	p += sizeof(DWORD);

	ClientPID =  *(DWORD*)p;
	p += sizeof(DWORD);

	ConnectionID =  *(DWORD*)p;
	p += sizeof(DWORD);

	OptionFlags1 = *p;
	p += sizeof(OptionFlags1);

	OptionFlags2 = *p;
	p += sizeof(OptionFlags2);

	typeFlag = *p;
	p += sizeof(typeFlag);

	OptionFlags3 = *p;
	bExtensionUsed = OptionFlags3 & LOGIN_FLAG3_EXTENSION;
	bChangePassword = OptionFlags3 & LOGIN_FLAG3_CHANGEPASSWORD;
	p += sizeof(OptionFlags3);

	ClientTimeZone = *(DWORD*)p;
	p += sizeof(ClientTimeZone);

	ClientLCID = *(DWORD*)p;
	p += sizeof(ClientLCID);

   //parse offset data
   USHORT Offset, Len;
   PBYTE pLogin7Data = NULL;

   //host name
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(HostName, pLogin7Data, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   HostName[Len] = 0;
  
   //user name
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(UserName, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   UserName[Len] = 0;

   //password 
   Offset =*(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
       memcpy(Password, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   Password[Len] = 0;

   //appname
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	    pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
		memcpy(AppName, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
		pLogin7Data += Len*sizeof(WCHAR);
   }  
   AppName[Len] = 0;

   //servername
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(ServerName, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   ServerName[Len] = 0;	

   //Extension
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   cbExtension = 0; 
   if (bExtensionUsed && Len)
   {
	   cbExtension = Len;
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(Extension, pBuf + TDS_HEADER_LEN + Offset, cbExtension);
	   pLogin7Data += cbExtension;
   }


   //cltIntName
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(CltIntName, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   CltIntName[Len] = 0;	

   //language
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	    memcpy(Language, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   Language[Len] = 0;	

   //database
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(DataBase, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   DataBase[Len] = 0;	

   //clientID
   memcpy(ClientID, p, 6);
   p += sizeof(ClientID);

   //SSPI
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   cbSSPI = Len;
   if (Len > 0)
   {
       pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
       pSSPI = new BYTE[Len];
       memcpy(pSSPI, pBuf + TDS_HEADER_LEN + Offset, cbSSPI);
       pLogin7Data += cbSSPI;
   }
   else
       cbSSPI = 0;
  

   //atch DBFile
   Offset = *(USHORT*)p; p+= sizeof(USHORT);
   Len = *(USHORT*)p;  p += sizeof(USHORT);
   if (Len)
   {
	   pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
	   memcpy(AtchDBFile, pBuf + TDS_HEADER_LEN + Offset, Len*sizeof(WCHAR));
	   pLogin7Data += Len*sizeof(WCHAR);
   }
   AtchDBFile[Len] = 0;	

   if (TDSVersion >= TDSVer::VER_7_2)
   {
       //change password
       Offset = *(USHORT*)p; p += sizeof(USHORT);
       Len = *(USHORT*)p;  p += sizeof(USHORT);
       if (bChangePassword && Len)
       {
           pLogin7Data = pBuf + TDS_HEADER_LEN + Offset;
           memcpy(ChangePassword, pBuf + TDS_HEADER_LEN + Offset, Len * sizeof(WCHAR));
           pLogin7Data += Len * sizeof(WCHAR);
           ChangePassword[Len] = 0;
       }
       else
       {
           ChangePassword[0] = 0;
       }

       //SSPILong 
       SSPILong = *(DWORD*)p;
       p += sizeof(SSPILong);
   }

   //caculate feature data
   FeatureDataLen = 0;
   if (bExtensionUsed)
   {
	   DWORD dwExtensionOffset = *(DWORD*)Extension;
	   if (dwExtensionOffset)
	   {
		   FeatureDataLen = (Length) - dwExtensionOffset;
		   memcpy(FeatureData, pBuf+TDS_HEADER_LEN+dwExtensionOffset, FeatureDataLen);
	   }
   }
}


//packet= header + fixdata + offsetdata + data + FeatureExt
void tdsLogin7::Serialize(PBYTE pBuf, DWORD bufLen, DWORD* cbData)
{
	PBYTE p = pBuf;
	*cbData = 0;

	//fill packet header
	hdr.Serialize(p);
	p += TDS_HEADER_LEN;
	*cbData += TDS_HEADER_LEN;


	//cacualte and fill fixdata
	*(DWORD*)p = Length;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(DWORD*)p = TDSVersion;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(DWORD*)p = PacketSize;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(DWORD*)p = ClientProgVer;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(DWORD*)p = ClientPID;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(DWORD*)p = ConnectionID;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	*(p++) = OptionFlags1;
	*(p++) = OptionFlags2;
	*(p++) = typeFlag;
	*(p++) = OptionFlags3;
	*cbData += 4;
	

	*(LONG*)p = ClientTimeZone;
	*cbData += sizeof(LONG);
	p += sizeof(LONG);
    
	*(DWORD*)p = ClientLCID;
	*cbData += sizeof(DWORD);
	p += sizeof(DWORD);

	//calculate and fill offsetdata and data
	const USHORT nOffsetDataLen = 59;
	USHORT* pOffsetData = (USHORT*)p;
	PBYTE pData = p + nOffsetDataLen;
	USHORT nDataOffset =  *cbData - TDS_HEADER_LEN + nOffsetDataLen;
	*cbData += nOffsetDataLen;

	//host name
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(HostName);
	if (pOffsetData[1])
	{
		memcpy(pData, HostName, pOffsetData[1]*sizeof(WCHAR) );
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //User name
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(UserName);
	if (pOffsetData[1])
	{
		memcpy(pData, UserName, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;


	//password
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(Password);
	if (pOffsetData[1])
	{
		memcpy(pData, Password, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
        *cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //app name
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(AppName);
	if (pOffsetData[1])
	{
		memcpy(pData, AppName, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

	//Server Name
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(ServerName);
	if (pOffsetData[1])
	{
		memcpy(pData, ServerName, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //Extension
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = cbExtension;
	DWORD* pExtensionOffset = NULL;
	if (bExtensionUsed && pOffsetData[1])
	{
		memcpy(pData, Extension, cbExtension);
		pExtensionOffset = (DWORD*)pData;

		pData += cbExtension;
		nDataOffset += cbExtension;
		*cbData += cbExtension;
	}
	pOffsetData += 2;

    //CltIntName
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(CltIntName);
	if (pOffsetData[1])
	{
		memcpy(pData, CltIntName, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //language
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(Language);
	if (pOffsetData[1])
	{
		memcpy(pData, Language, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //database
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(DataBase);
	if (pOffsetData[1])
	{
		memcpy(pData, DataBase, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    //ClientID
	memcpy(pOffsetData, ClientID, sizeof(ClientID));
	pOffsetData = (USHORT*)((PBYTE)pOffsetData + sizeof(ClientID));

	//SSPI
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = cbSSPI;
	if (pOffsetData[1])
	{
		memcpy(pData, pSSPI, cbSSPI);
		pData += cbSSPI;
		nDataOffset += cbSSPI;
		*cbData += cbSSPI;
	}
	pOffsetData += 2;

	//AtchDBFile
	pOffsetData[0] = nDataOffset;
	pOffsetData[1] = wcslen(AtchDBFile);
	if (pOffsetData[1])
	{
		memcpy(pData, AtchDBFile, pOffsetData[1]*sizeof(WCHAR));
		pData += pOffsetData[1]*sizeof(WCHAR);
		nDataOffset += pOffsetData[1]*sizeof(WCHAR);
		*cbData += pOffsetData[1]*sizeof(WCHAR);
	}
	pOffsetData += 2;

    if (TDSVersion >= TDSVer::VER_7_2)
    {
        //ChangePassword
        pOffsetData[0] = nDataOffset;
        pOffsetData[1] = wcslen(ChangePassword);
        if (bChangePassword && pOffsetData[1])
        {
            memcpy(pData, ChangePassword, pOffsetData[1] * sizeof(WCHAR));
            pData += pOffsetData[1] * sizeof(WCHAR);
            nDataOffset += pOffsetData[1] * sizeof(WCHAR);
            *cbData += pOffsetData[1] * sizeof(WCHAR);
        }
        pOffsetData += 2;

        //SSPI long
        *(DWORD*)pOffsetData = SSPILong;
        pOffsetData = (USHORT*)((BYTE*)pOffsetData + sizeof(SSPILong));
    }

	//Feature Ext
	if (bExtensionUsed)
	{
		if (FeatureDataLen)
		{
			memcpy(pData, FeatureData, FeatureDataLen);
			*cbData += FeatureDataLen;
			*pExtensionOffset = *cbData - 8 - FeatureDataLen;
		}
		else
		{
			*pExtensionOffset = 0;
		}
	}

   //modify finally data size
    TDS::SetTdsPacketLength(pBuf, *cbData);
	SetStructSize(pBuf, *cbData-TDS_HEADER_LEN);

}

void tdsLogin7::FillContext(PBYTE buff, uint32_t buff_len)
{
    if (buff == nullptr || buff_len == 0)
        return;

    PBYTE p = buff;

    //cacualte and fill fixdata
    *(DWORD*)p = Length;
    p += sizeof(DWORD);

    *(DWORD*)p = TDSVersion;
    p += sizeof(DWORD);

    *(DWORD*)p = PacketSize;
    p += sizeof(DWORD);

    *(DWORD*)p = ClientProgVer;
    p += sizeof(DWORD);

    *(DWORD*)p = ClientPID;
    p += sizeof(DWORD);

    *(DWORD*)p = ConnectionID;
    p += sizeof(DWORD);

    *(p++) = OptionFlags1;
    *(p++) = OptionFlags2;
    *(p++) = typeFlag;
    *(p++) = OptionFlags3;

    *(LONG*)p = ClientTimeZone;
    p += sizeof(LONG);

    *(DWORD*)p = ClientLCID;
    p += sizeof(DWORD);

    //calculate and fill offsetdata and data
    const USHORT nOffsetDataLen = 59;
    USHORT* pOffsetData = (USHORT*)p;
    PBYTE pData = p + nOffsetDataLen;
    USHORT nDataOffset = 36 + nOffsetDataLen;

    //host name
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(HostName);
    if (pOffsetData[1])
    {
        memcpy(pData, HostName, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //User name
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(UserName);
    if (pOffsetData[1])
    {
        memcpy(pData, UserName, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;


    //password
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(Password);
    if (pOffsetData[1])
    {
        memcpy(pData, Password, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //app name
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(AppName);
    if (pOffsetData[1])
    {
        memcpy(pData, AppName, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //Server Name
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(ServerName);
    if (pOffsetData[1])
    {
        memcpy(pData, ServerName, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //Extension
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = cbExtension;
    DWORD* pExtensionOffset = NULL;
    if (bExtensionUsed && pOffsetData[1])
    {
        memcpy(pData, Extension, cbExtension);
        pExtensionOffset = (DWORD*)pData;

        pData += cbExtension;
        nDataOffset += cbExtension;
    }
    pOffsetData += 2;

    //CltIntName
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(CltIntName);
    if (pOffsetData[1])
    {
        memcpy(pData, CltIntName, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //language
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(Language);
    if (pOffsetData[1])
    {
        memcpy(pData, Language, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //database
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(DataBase);
    if (pOffsetData[1])
    {
        memcpy(pData, DataBase, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    //ClientID
    memcpy(pOffsetData, ClientID, sizeof(ClientID));
    pOffsetData = (USHORT*)((PBYTE)pOffsetData + sizeof(ClientID));

    //SSPI
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = cbSSPI;
    if (pOffsetData[1])
    {
        memcpy(pData, pSSPI, cbSSPI);
        pData += cbSSPI;
        nDataOffset += cbSSPI;
    }
    pOffsetData += 2;

    //AtchDBFile
    pOffsetData[0] = nDataOffset;
    pOffsetData[1] = wcslen(AtchDBFile);
    if (pOffsetData[1])
    {
        memcpy(pData, AtchDBFile, pOffsetData[1] * sizeof(WCHAR));
        pData += pOffsetData[1] * sizeof(WCHAR);
        nDataOffset += pOffsetData[1] * sizeof(WCHAR);
    }
    pOffsetData += 2;

    if (TDSVersion >= TDSVer::VER_7_2)
    {
        //ChangePassword
        pOffsetData[0] = nDataOffset;
        pOffsetData[1] = wcslen(ChangePassword);
        if (bChangePassword && pOffsetData[1])
        {
            memcpy(pData, ChangePassword, pOffsetData[1] * sizeof(WCHAR));
            pData += pOffsetData[1] * sizeof(WCHAR);
            nDataOffset += pOffsetData[1] * sizeof(WCHAR);
        }
        pOffsetData += 2;

        //SSPI long
        *(DWORD*)pOffsetData = SSPILong;
        pOffsetData = (USHORT*)((BYTE*)pOffsetData + sizeof(SSPILong));
    }

    //Feature Ext
    if (bExtensionUsed)
    {
        if (FeatureDataLen)
        {
            memcpy(pData, FeatureData, FeatureDataLen);
            *pExtensionOffset = buff_len - FeatureDataLen;
        }
        else
        {
            *pExtensionOffset = 0;
        }
    }
}

void tdsLogin7::Serialize(std::list<PBYTE>& packs)
{
    uint32_t buff_len = EstimateSerializeSize();

    if (buff_len <= 4096) {
        PBYTE pack_buff = new BYTE[buff_len];
        memset(pack_buff, 0, buff_len);

        hdr.Serialize(pack_buff);
        pack_buff[1] = 1; // EOF
        FillContext(pack_buff + TDS_HEADER_LEN, buff_len - TDS_HEADER_LEN);

        //modify finally data size
        TDS::SetTdsPacketLength(pack_buff, buff_len);
        SetStructSize(pack_buff, buff_len - TDS_HEADER_LEN);

        packs.push_back(pack_buff);
    }
    else {
        uint32_t cache_len = buff_len - TDS_HEADER_LEN;
        PBYTE cache = new BYTE[cache_len];
        FillContext(cache, cache_len);

        uint32_t pack_id = 1;
        while (cache_len > 0)
        {
            if (cache_len + TDS_HEADER_LEN > 4096)
            {
                PBYTE pack_buff = new BYTE[4096];
                TDS::FillTDSHeader(pack_buff, TDS_LOGIN7, 4, 4096, 0, pack_id++, 0);
                memcpy(pack_buff + TDS_HEADER_LEN, cache + (buff_len - TDS_HEADER_LEN - cache_len), 4096 - TDS_HEADER_LEN);
                if (pack_id == 1)
                {
                    SetStructSize(pack_buff, buff_len - TDS_HEADER_LEN);
                }
                cache_len -= 4096 - TDS_HEADER_LEN;
                packs.push_back(pack_buff);
            }
            else
            {
                PBYTE pack_buff = new BYTE[cache_len + TDS_HEADER_LEN];
                TDS::FillTDSHeader(pack_buff, TDS_LOGIN7, 1, cache_len + TDS_HEADER_LEN, 0, pack_id++, 0);
                memcpy(pack_buff + TDS_HEADER_LEN, cache + (buff_len - TDS_HEADER_LEN - cache_len), cache_len);
                cache_len = 0;
                packs.push_back(pack_buff);
            }
        }

        delete[] cache;
    }
}

uint32_t tdsLogin7::EstimateSerializeSize()
{
    uint32_t buff_len = TDS_HEADER_LEN + 8 * sizeof(DWORD) + 4 + 59;

    if (wcslen(HostName) > 0)
        buff_len += wcslen(HostName) * sizeof(WCHAR);
    if (wcslen(UserName) > 0)
        buff_len += wcslen(UserName) * sizeof(WCHAR);
    if (wcslen(Password) > 0)
        buff_len += wcslen(Password) * sizeof(WCHAR);
    if (wcslen(AppName) > 0)
        buff_len += wcslen(AppName) * sizeof(WCHAR);
    if (wcslen(ServerName) > 0)
        buff_len += wcslen(ServerName) * sizeof(WCHAR);
    if (bExtensionUsed && cbExtension > 0)
        buff_len += cbExtension;
    if (wcslen(CltIntName) > 0)
        buff_len += wcslen(CltIntName) * sizeof(WCHAR);
    if (wcslen(Language) > 0)
        buff_len += wcslen(Language) * sizeof(WCHAR);
    if (wcslen(DataBase) > 0)
        buff_len += wcslen(DataBase) * sizeof(WCHAR);
    if (cbSSPI > 0)
        buff_len += cbSSPI;
    if (wcslen(AtchDBFile) > 0)
        buff_len += wcslen(AtchDBFile) * sizeof(WCHAR);
    if (TDSVersion >= TDSVer::VER_7_2 && bChangePassword && wcslen(ChangePassword) > 0)
        buff_len += wcslen(ChangePassword) * sizeof(WCHAR);
    if (bExtensionUsed && FeatureDataLen > 0)
        buff_len += FeatureDataLen;

    return buff_len;
}

void tdsLogin7::SetStructSize(PBYTE Packet, DWORD dwStructSize)
{
   *(DWORD*)(Packet + TDS_HEADER_LEN) = dwStructSize;
}

void tdsLogin7::SetServerName(LPWSTR wstrServerName)
{
	wcscpy(ServerName, wstrServerName);
}

bool tdsLogin7::IsKerberosAuth()
{
    if (cbSSPI > 0 && pSSPI != nullptr)
    {
        const uint8_t* p = ProxyCommon::BinarySearch(pSSPI, cbSSPI, NTLMSSP_identifier, sizeof(NTLMSSP_identifier));
        if (p == NULL)
            return true;
        else
            return false;
    }

    return false;
}

void tdsLogin7::GetKerberosTicket(std::vector<BYTE>& data)
{
    if (cbSSPI > 0 && pSSPI != nullptr)
    {
        data.resize(cbSSPI);
        memcpy(&data[0], pSSPI, cbSSPI);
    }
}

void tdsLogin7::SetKerberosTicket(const std::vector<BYTE>& data)
{
    if (cbSSPI > 0 && pSSPI != nullptr)
    {
        delete[] pSSPI;
        pSSPI = nullptr;
    }

    cbSSPI = data.size();
    pSSPI = new BYTE[cbSSPI];
    memcpy(pSSPI, &data[0], cbSSPI);
}