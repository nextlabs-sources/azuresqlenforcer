#ifndef TDS_TOKEN_FEATURE_EXT_ACK_H
#define TDS_TOKEN_FEATURE_EXT_ACK_H

#include <list>
#include "tdstoken.h"

class tdsFeatureAckOpt
{
public:
	tdsFeatureAckOpt() {}
	~tdsFeatureAckOpt() { delete[] pDATA; }
public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	BYTE FeatureId;
	DWORD FeatureAckDataLen;
	PBYTE pDATA;
};

class tdsTokenFeatureExtAck : public tdsToken
{
public:
	tdsTokenFeatureExtAck(void);
	~tdsTokenFeatureExtAck(void);

public:
	void Parse(ReadBuffer& rBuf);
	DWORD CBSize();
	int Serialize(WriteBuffer& wBuf);
private:
	std::list<tdsFeatureAckOpt*> FeatureAckOpts;
};

#endif

