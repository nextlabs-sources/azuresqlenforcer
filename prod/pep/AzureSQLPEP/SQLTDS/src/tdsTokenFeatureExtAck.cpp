#include "tdsTokenFeatureExtAck.h"

void tdsFeatureAckOpt::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(FeatureId);
	rBuf.ReadAny(FeatureAckDataLen);
	pDATA = new BYTE[FeatureAckDataLen];
	rBuf.ReadData(pDATA, FeatureAckDataLen);
}
DWORD tdsFeatureAckOpt::CBSize()
{
	return sizeof(FeatureId) + sizeof(FeatureAckDataLen) + FeatureAckDataLen;
}
int tdsFeatureAckOpt::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(FeatureId);
	wBuf.WriteAny(FeatureAckDataLen);
	wBuf.WriteData(pDATA, FeatureAckDataLen);
	return 0;
}



tdsTokenFeatureExtAck::tdsTokenFeatureExtAck(void)
{
	tdsTokenType = TDS_TOKEN_FEATUREEXTACK;
}

tdsTokenFeatureExtAck::~tdsTokenFeatureExtAck(void)
{
	for (auto it : FeatureAckOpts)
		delete it;
	FeatureAckOpts.clear();
}

void tdsTokenFeatureExtAck::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	BYTE TERMINATOR = 0;
	while (rBuf.PeekAny(TERMINATOR) && TERMINATOR != 0xff)
	{
		tdsFeatureAckOpt* opt = new tdsFeatureAckOpt;
		opt->Parse(rBuf);
		FeatureAckOpts.push_back(opt);
	}
	rBuf.ReadAny(TERMINATOR);
}

DWORD tdsTokenFeatureExtAck::CBSize()
{
	DWORD ret = tdsToken::CBSize();
	for (auto it : FeatureAckOpts)
		ret += it->CBSize();
	return ret + sizeof(BYTE);
}

int tdsTokenFeatureExtAck::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	for (auto it : FeatureAckOpts)
		it->Serialize(wBuf);
	BYTE TERMINATOR = 0xff;
	wBuf.WriteAny(TERMINATOR);
	return 0;
}
