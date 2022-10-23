#ifndef TDS_LOGIN7_H
#define TDS_LOGIN7_H

#include <string>
#include <list>
#include <vector>
#include "tdsPacket.h"

#define LOGIN_FLAG3_EXTENSION 0x10
#define LOGIN_FLAG3_CHANGEPASSWORD 0x01

enum TDSVer
{
    VER_7_4 = 0x74000004, // TDS 7.4
    VER_7_3 = 0x730B0003, // TDS 7.3B(includes null bit compression)
    VER_7_2 = 0x72090002, // TDS 7.2
};

class tdsLogin7 : public tdsPacket
{
public:
	tdsLogin7(const std::list<PBYTE>& packs);
	~tdsLogin7(void);

public:
	void Serialize(PBYTE pBuf, DWORD bufLen, DWORD* cbData);
    void Serialize(std::list<PBYTE>& packs);
	void SetServerName(LPWSTR wstrServerName);
    inline const WCHAR* GetServerName() const { return ServerName; }
    inline const WCHAR* GetAppName() const { return AppName; }
	inline const WCHAR* GetUserUniqueName() const { return UserName; }
    inline const WCHAR* GetLanguage() const { return Language; }
    inline const WCHAR* GetCltIntName() const { return CltIntName; }
	inline const WCHAR* GetDataBase() const { return DataBase; }
    inline const WCHAR* GetHostName() const { return HostName; }
    inline const WCHAR* GetAtachDBFile() const { return AtchDBFile; }
    inline USHORT GetcbSSPI() const { return cbSSPI; }
    inline DWORD GetSSPILong() const { return SSPILong; }
    std::wstring ClientID2String();

    bool IsKerberosAuth();
    void GetKerberosTicket(std::vector<BYTE>& data);
    void SetKerberosTicket(const std::vector<BYTE>& data);

private:
	void Parse(const std::list<PBYTE>& packs);
    void Parse(PBYTE data, uint32_t len);
	void SetStructSize(PBYTE Packet, DWORD dwStructSize);
    void FillContext(PBYTE buff, uint32_t buff_len);
    uint32_t EstimateSerializeSize();

private:
	//login7 packet data
	DWORD   Length;
	DWORD	TDSVersion;
	DWORD	PacketSize;
	DWORD	ClientProgVer;
	DWORD	ClientPID;
	DWORD	ConnectionID;

	BYTE OptionFlags1;
    BYTE OptionFlags2;
	BYTE typeFlag;
	BYTE OptionFlags3;

	LONG ClientTimeZone;
	DWORD ClientLCID;

	//offset data
	WCHAR HostName[129];
	WCHAR UserName[129];
	WCHAR Password[129];
	WCHAR AppName[129];
	WCHAR ServerName[129];

	BOOL  bExtensionUsed;
	BYTE  Extension[255];
	USHORT  cbExtension;

	WCHAR CltIntName[129];
	WCHAR Language[129];
	WCHAR DataBase[129];
	BYTE  ClientID[6];

	PBYTE pSSPI;
	USHORT cbSSPI;

	WCHAR AtchDBFile[261];

	BOOL  bChangePassword;
	WCHAR  ChangePassword[129];
    // if SSPI data length >= 65535, then SSPILong will be the real length.
	DWORD SSPILong;

	//feature
	//BYTE FeatureID;
	DWORD FeatureDataLen;
	BYTE FeatureData[1024];
};

#endif //TDS_LOGIN7_H

