#ifndef TDS_TOKEN_ENV_CHANGE_H
#define TDS_TOKEN_ENV_CHANGE_H

#include "tdsToken.h"
#include "tdsCountVarValue.h"

struct RoutingData
{
    RoutingData()
    {
        RoutingDataValueLength = 0;
        Protocol = 0;
        ProtocolProperty = 0;
    }

	USHORT							RoutingDataValueLength;
	BYTE							Protocol;
	USHORT							ProtocolProperty;
	tdsCountVarValue<USHORT, WCHAR> AlternateServer;

	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
};

struct EnvValue
{
    tdsCountVarValue<BYTE, WCHAR>	B_VARCHAR;
    tdsCountVarValue<BYTE, BYTE>	B_VARBYTE;
    tdsCountVarValue<LONG, BYTE>	L_VARBYTE;
    RoutingData						ROUTING;

    BYTE							ZERO1;
    BYTE							ZERO2[2];
};

class tdsEnvValueData
{
public:
	tdsEnvValueData();
	~tdsEnvValueData();
public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
public:
	BYTE Type;
	EnvValue NewValue;
	EnvValue OldValue;
};

class tdsTokenEnvChange : public tdsToken
{
public:
	tdsTokenEnvChange(void);
	virtual ~tdsTokenEnvChange(void);
public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
	const tdsEnvValueData& GetTdsEnvValueData() const { return EnvValueData; }
private:
	USHORT 	Length;
	tdsEnvValueData EnvValueData;
};

#endif

