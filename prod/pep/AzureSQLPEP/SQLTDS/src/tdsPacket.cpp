#include "tdsPacket.h"

const wchar_t* const TDS_MSG_TYPE_STRING[19] = {
    L"TDS_Unkown",    // 0
    L"TDS_SQL_Batch",       // 1
    L"TDS_Pre_Login_old",   // 2
    L"TDS_RPC",             // 3
    L"TDS_server_Response", // 4
    L"",                    // 5
    L"TDS_Attention_signal",// 6
    L"TDS_Bulk_load_data",  // 7
    L"TDS_Federated_Authentication_Token", // 8
    L"", // 9
    L"", // 10
    L"", // 11
    L"", // 12
    L"", // 13
    L"TDS_Transaction_manager_request", // 14
    L"", // 15
    L"TDS_Login7", // 16
    L"TDS_SSPI_login7", // 17
    L"TDS_Pre_login"
};

tdsPacket::tdsPacket(PBYTE pData):hdr(pData)
{
}

tdsPacket::tdsPacket()
{

}

tdsPacket::~tdsPacket(void)
{
}

void tdsPacket::Parse(PBYTE pBuf)
{
	hdr.Parse(pBuf);
}
