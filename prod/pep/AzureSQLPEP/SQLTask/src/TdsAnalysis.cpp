#include "TdsAnalysis.h"
#include "CommonFunc.h"
#include "SQLEnforceHelper.h"
#include "tds.h"
#include "Log.h"

extern HMODULE g_hThisModule;

TdsAnalysis::TdsAnalysis()
    :m_maxPackSize(4096)
    ,m_params(nullptr)
{
}


TdsAnalysis::~TdsAnalysis()
{
}

#define DAE_USERPROPERTY_COUNT 7

void TdsAnalysis::DoSQLEnforce(const std::wstring& oriSqlText, std::wstring& newSqlText)
{
    std::string owner = m_params->domain_name.empty() ? (m_params->sql_user.c_str()) : (string(m_params->domain_name + "\\" + m_params->sql_user).c_str());
    DAESqlServer::DAEResultHandle result = nullptr;
    DAESqlServer::DAE_UserProperty* userprop = new DAESqlServer::DAE_UserProperty[DAE_USERPROPERTY_COUNT];
    userprop[0]._key = "USER_IP";
    userprop[0]._value = m_params->user_ip.c_str();
    userprop[1]._key = "SQL_USER";
    userprop[1]._value = m_params->sql_user.c_str();
    userprop[2]._key = "DOMAIN_NAME";
    userprop[2]._value = m_params->domain_name.c_str();
    userprop[3]._key = "COMPUTER_NAME";
    userprop[3]._value = m_params->computer_name.c_str();
    userprop[4]._key = "SERVICE_NAME";
    userprop[4]._value = m_params->database.c_str();
    userprop[5]._key = "OWNER";
    userprop[5]._value = owner.c_str();
    userprop[6]._key = "APP_NAME";
    userprop[6]._value = m_params->appname.c_str();

    // query default schema for window user
    // select * from sys.database_principals

    try
    {
        std::string sqltext = ProxyCommon::UnicodeToUTF8(oriSqlText);

        SQLEnforceMgr::GetInstance()->DAENewRes(&result);

        bool sdkres = SQLEnforceMgr::GetInstance()->EnforceSimpleV2(sqltext.c_str(), userprop, DAE_USERPROPERTY_COUNT, nullptr, 0, result);
        if (sdkres)
        {
            const char* newsql = nullptr;
            const char* default_db = nullptr;
            DAESqlServer::DAE_ResultCode rescode = DAESqlServer::DAE_ResultCode::DAE_FAILED;
            SQLEnforceMgr::GetInstance()->DAEGetRes(result, &rescode, &newsql, &default_db);
            // enforce and create new sql text
            if (rescode == DAESqlServer::DAE_ResultCode::DAE_ENFORCED && newsql)
            {
                newSqlText = ProxyCommon::UTF8ToUnicode(newsql);
            }

            if (default_db && _stricmp(default_db, m_params->database.c_str()) != 0)
                m_params->database = default_db;
        }
        else
        {
            LOGPRINT(CELOG_WARNING, L"sql enforce sdk return failed!");
        }
    }
    catch (std::exception e)
    {
        LOGPRINT(CELOG_ERR, "sql enforce sdk exception: %s", e.what());
    }

    // release resource
    if (result)
        SQLEnforceMgr::GetInstance()->DAEFreeRes(result);
    delete[] userprop;
}

void TdsAnalysis::ProcessClientMessage(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets)
{
    if (oldPackets.size() == 0 || m_params == nullptr)
        return;

    TDS_MSG_TYPE tdsType = TDS_Unkown;
    uint8_t* pack = oldPackets.front();
    if (pack[0] == SMP_PACKET_FLAG)
    {
        // nothing to do with SMP packet;
        if (SMP::GetSMPPacketLen(pack) <= 16)
            return;

        if (SMP::GetSMPPacketLen(pack) > 16)
            tdsType = TDS::GetTdsPacketType(pack + 16);
    }
    else 
        tdsType = TDS::GetTdsPacketType(pack);

    LOGPRINT(CELOG_INFO, L"Client message tds type: %s", TDS::GetTdsPacketTypeString(tdsType));

	switch (tdsType)
	{
	case TDS_SQLBATCH:
		ProcessClientBatch(oldPackets, newPackets);
		break;
	case TDS_RPC:
		ProcessClientRPC(oldPackets, newPackets);
		break;
	default:
		break;
	}
}

void TdsAnalysis::ProcessClientRPC(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets)
{
    LOGPRINT(CELOG_INFO, L"TdsTask::ProcessClientRPC Start");

    tdsClientRPC rpc(m_maxPackSize);
    rpc.SetSmpLastRequestSeqnum(m_params->smp_last_request_seqnum);
    rpc.SetSmpLastRequestWndw(m_params->smp_last_request_wndw);
    try
    {
        std::list<uint8_t*>::const_iterator itPack = oldPackets.begin();
        for (; itPack != oldPackets.end(); itPack++)
        {
            rpc.Parse(*itPack);
        }

        std::vector<RpcText> texts = rpc.GetRpcTextList();
        std::wstring sqltext;
        for each (const RpcText& t in texts)
        {
            if (SQLEnforceMgr::GetInstance()->DAEIsSqlText(t.text.c_str())) {
                sqltext = t.text;
                rpc.SetSqlTextIndex(t.param_index);

                LOGPRINT(CELOG_DEBUG, L"Remote Procedure Call:\n%s", t.text.c_str());
                break;
            }
        }
        if (sqltext.empty())
            return;

        /* do sql enforce here, and modify sql text. */
        /* if no need, just reture. and do nothing. */
        std::wstring newSqlText;
        DoSQLEnforce(sqltext, newSqlText);
        if (!newSqlText.empty())
        {
            rpc.SetNewSqlText(newSqlText);
            rpc.Serialize(newPackets);
        }
    }
    catch (TDSException& e)
    {
        LOGPRINT(CELOG_WARNING, "ProcessClientRPC exception: %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGPRINT(CELOG_ERR, "ProcessClientRPC exception: %s", e.what());
    }
    LOGPRINT(CELOG_INFO, L"TdsTask::ProcessClientRPC End");
}

void TdsAnalysis::ProcessClientBatch(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets)
{
    LOGPRINT(CELOG_INFO, L"TdsTask::ProcessClientBatch Start");

    try
    {
        tdsSqlBatch batch(m_maxPackSize);
        batch.SetSmpLastRequestSeqnum(m_params->smp_last_request_seqnum);
        batch.SetSmpLastRequestWndw(m_params->smp_last_request_wndw);
        list<PBYTE>::const_iterator itPack = oldPackets.begin();
        for (; itPack != oldPackets.end(); itPack++)
        {
            batch.Parse(*itPack);
        }
        LOGPRINT(CELOG_DEBUG, L"SQL Batch:\n%s", batch.GetSQLText().c_str());

        /* Parse SQL Text. Do enforce */
        std::wstring newSqlText;
        DoSQLEnforce(batch.GetSQLText(), newSqlText);
        if (!newSqlText.empty())
        {
            batch.SetSQLText(newSqlText);
            batch.Serialize(newPackets);
        }
    }
    catch (TDSException e)
    {
        LOGPRINT(CELOG_WARNING, "ProcessClientBatch exception: %s", e.what());
    }
    catch (std::exception e)
    {
        LOGPRINT(CELOG_ERR, "ProcessClientBatch exception: %s", e.what());
    }

    LOGPRINT(CELOG_INFO, L"TdsTask::ProcessClientBatch End");
}

void TdsAnalysis::ProcessServerMessage(const std::list<uint8_t*>& oldPackets, std::list<uint8_t*>& newPackets, uint8_t requestType)
{
    if (oldPackets.size() == 0 || m_params == nullptr)
        return;

    TDS_MSG_TYPE tdsType = TDS_Unkown;
    uint8_t* pack = oldPackets.front();
    if (pack[0] == SMP_PACKET_FLAG)
    {
        if (SMP::GetSMPPacketLen(pack) <= 16)
            return;

        if (SMP::GetSMPPacketLen(pack) > 16)
            tdsType = TDS::GetTdsPacketType(pack + 16);
    }
    else
	    tdsType = TDS::GetTdsPacketType(*oldPackets.begin());

	LOGPRINT(CELOG_INFO, L"Server message tds type: %s", TDS::GetTdsPacketTypeString(tdsType));
	LOGPRINT(CELOG_INFO, L"------> Binding client type: %s", TDS::GetTdsPacketTypeString(requestType));

	if (TDS_RESPONSE == tdsType)
	{
	}
}
