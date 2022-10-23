#include "tdsTokenEnvChange.h"
#include "tdsTypeHelper.h"

void RoutingData::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(RoutingDataValueLength);
	rBuf.ReadAny(Protocol);
	rBuf.ReadAny(ProtocolProperty);
	AlternateServer.Parse(rBuf);
}

DWORD RoutingData::CBSize()
{
	return sizeof(RoutingDataValueLength) +
		sizeof(Protocol) +
		sizeof(ProtocolProperty) +
		AlternateServer.CBSize();
}

int RoutingData::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(RoutingDataValueLength);
	wBuf.WriteAny(Protocol);
	wBuf.WriteAny(ProtocolProperty);
	AlternateServer.Serialize(wBuf);
	return 0;
}

////////////////////////////////////////////////////

tdsEnvValueData::tdsEnvValueData(void)
{
}

tdsEnvValueData::~tdsEnvValueData(void)
{
}

void tdsEnvValueData::Parse(ReadBuffer& rBuf)
{
	rBuf.ReadAny(Type);
	TDS_ENVCHANGE_TOKEN_TYPE envChangeType = (TDS_ENVCHANGE_TOKEN_TYPE)Type;
	switch (envChangeType)
	{
	case DATABASE:
	case LANGUAGE:
	case CHARACTER_SET:
	case PACKET_SIZE:
		NewValue.B_VARCHAR.Parse(rBuf);
		OldValue.B_VARCHAR.Parse(rBuf);
		break;
	case UNICODE_DATA_SORTING_LOCAL_ID:
	case UNICODE_DATA_SORTING_COMPARISON_FLAGS:
		NewValue.B_VARCHAR.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case SQL_COLLATION:
		NewValue.B_VARBYTE.Parse(rBuf);
		OldValue.B_VARBYTE.Parse(rBuf);
		break;
	case BEGIN_TRANSACTION:
		NewValue.B_VARBYTE.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case COMMIT_TRANSACTION:
	case ROLLBACK_TRANSACTION:
	case ENLIST_DTC_TRANSACTION:
		rBuf.ReadAny(NewValue.ZERO1);
		OldValue.B_VARBYTE.Parse(rBuf);
		break;
	case DEFECT_TRANSACTION:
		NewValue.B_VARBYTE.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case REAL_TIME_LOG_SHIPPING:
		// there is some problem at tds.doc page 91
		NewValue.B_VARCHAR.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case PROMOTE_TRANSACTION:
		NewValue.L_VARBYTE.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case TRANSACTION_MANAGER_ADDRESS:
		NewValue.B_VARBYTE.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case TRANSACTION_ENDED:
		rBuf.ReadAny(NewValue.ZERO1);
		OldValue.B_VARBYTE.Parse(rBuf);
		break;
	case RESETCONNECTION_OR_RESETCONNECTIONSKIPTRAN_COMPLETION_ACKNOWLEDGEMENT:
		rBuf.ReadAny(NewValue.ZERO1);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case SEND_BACK_NAME_OF_USER_INSTANCE_STARTED_PER_LOGIN_REQUEST:
		NewValue.B_VARCHAR.Parse(rBuf);
		rBuf.ReadAny(OldValue.ZERO1);
		break;
	case SENDS_ROUTING_INFORMATION_TO_CLIENT:
		NewValue.ROUTING.Parse(rBuf);
		rBuf.ReadData(OldValue.ZERO2, 2);
		break;
	default:
		break;
	}
}

DWORD tdsEnvValueData::CBSize()
{
	DWORD ret = sizeof(Type);
	TDS_ENVCHANGE_TOKEN_TYPE envChangeType = (TDS_ENVCHANGE_TOKEN_TYPE)Type;
	switch (envChangeType)
	{
	case DATABASE:
	case LANGUAGE:
	case CHARACTER_SET:
	case PACKET_SIZE:
		ret += NewValue.B_VARCHAR.CBSize();
		ret += OldValue.B_VARCHAR.CBSize();
		break;
	case UNICODE_DATA_SORTING_LOCAL_ID:
	case UNICODE_DATA_SORTING_COMPARISON_FLAGS:
		ret += NewValue.B_VARCHAR.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case SQL_COLLATION:
		ret += NewValue.B_VARBYTE.CBSize();
		ret += OldValue.B_VARBYTE.CBSize();
		break;
	case BEGIN_TRANSACTION:
		ret += NewValue.B_VARBYTE.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case COMMIT_TRANSACTION:
	case ROLLBACK_TRANSACTION:
	case ENLIST_DTC_TRANSACTION:
		ret += sizeof(NewValue.ZERO1);
		ret += OldValue.B_VARBYTE.CBSize();
		break;
	case DEFECT_TRANSACTION:
		ret += NewValue.B_VARBYTE.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case REAL_TIME_LOG_SHIPPING:
		// there is some problem at tds.doc page 91
		ret += NewValue.B_VARCHAR.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case PROMOTE_TRANSACTION:
		ret += NewValue.L_VARBYTE.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case TRANSACTION_MANAGER_ADDRESS:
		ret += NewValue.B_VARBYTE.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case TRANSACTION_ENDED:
		ret += sizeof(NewValue.ZERO1);
		ret += OldValue.B_VARBYTE.CBSize();
		break;
	case RESETCONNECTION_OR_RESETCONNECTIONSKIPTRAN_COMPLETION_ACKNOWLEDGEMENT:
		ret += sizeof(NewValue.ZERO1);
		ret += sizeof(OldValue.ZERO1);
		break;
	case SEND_BACK_NAME_OF_USER_INSTANCE_STARTED_PER_LOGIN_REQUEST:
		ret += NewValue.B_VARCHAR.CBSize();
		ret += sizeof(OldValue.ZERO1);
		break;
	case SENDS_ROUTING_INFORMATION_TO_CLIENT:
		ret += NewValue.ROUTING.CBSize();
		ret += 2;
		break;
	default:
		break;
	}
	return ret;
}

int tdsEnvValueData::Serialize(WriteBuffer& wBuf)
{
	wBuf.WriteAny(Type);
	TDS_ENVCHANGE_TOKEN_TYPE envChangeType = (TDS_ENVCHANGE_TOKEN_TYPE)Type;
	switch (envChangeType)
	{
	case DATABASE:
	case LANGUAGE:
	case CHARACTER_SET:
	case PACKET_SIZE:
		NewValue.B_VARCHAR.Serialize(wBuf);
		OldValue.B_VARCHAR.Serialize(wBuf);
		break;
	case UNICODE_DATA_SORTING_LOCAL_ID:
	case UNICODE_DATA_SORTING_COMPARISON_FLAGS:
		NewValue.B_VARCHAR.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case SQL_COLLATION:
		NewValue.B_VARBYTE.Serialize(wBuf);
		OldValue.B_VARBYTE.Serialize(wBuf);
		break;
	case BEGIN_TRANSACTION:
		NewValue.B_VARBYTE.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case COMMIT_TRANSACTION:
	case ROLLBACK_TRANSACTION:
	case ENLIST_DTC_TRANSACTION:
		wBuf.WriteAny(NewValue.ZERO1);
		OldValue.B_VARBYTE.Serialize(wBuf);
		break;
	case DEFECT_TRANSACTION:
		NewValue.B_VARBYTE.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case REAL_TIME_LOG_SHIPPING:
		// there is some problem at tds.doc page 91
		NewValue.B_VARCHAR.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case PROMOTE_TRANSACTION:
		NewValue.L_VARBYTE.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case TRANSACTION_MANAGER_ADDRESS:
		NewValue.B_VARBYTE.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case TRANSACTION_ENDED:
		wBuf.WriteAny(NewValue.ZERO1);
		OldValue.B_VARBYTE.Serialize(wBuf);
		break;
	case RESETCONNECTION_OR_RESETCONNECTIONSKIPTRAN_COMPLETION_ACKNOWLEDGEMENT:
		wBuf.WriteAny(NewValue.ZERO1);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case SEND_BACK_NAME_OF_USER_INSTANCE_STARTED_PER_LOGIN_REQUEST:
		NewValue.B_VARCHAR.Serialize(wBuf);
		wBuf.WriteAny(OldValue.ZERO1);
		break;
	case SENDS_ROUTING_INFORMATION_TO_CLIENT:
		NewValue.ROUTING.Serialize(wBuf);
		wBuf.WriteData(OldValue.ZERO2, 2);
		break;
	default:
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////

tdsTokenEnvChange::tdsTokenEnvChange(void)
{
	tdsTokenType = TDS_TOKEN_ENVCHANGE;
}

tdsTokenEnvChange::~tdsTokenEnvChange(void)
{
}

void tdsTokenEnvChange::Parse(ReadBuffer& rBuf)
{
	tdsToken::Parse(rBuf);
	rBuf.ReadAny(Length);
	EnvValueData.Parse(rBuf);
}

DWORD tdsTokenEnvChange::CBSize()
{
	return tdsToken::CBSize() +
		sizeof(Length) +
		EnvValueData.CBSize();
}

int tdsTokenEnvChange::Serialize(WriteBuffer& wBuf)
{
	tdsToken::Serialize(wBuf);
	wBuf.WriteAny(Length);
	EnvValueData.Serialize(wBuf);
	return 0;
}
