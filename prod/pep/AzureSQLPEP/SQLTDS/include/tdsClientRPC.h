#pragma once

#ifndef TDS_CLIENT_RPC_H
#define TDS_CLIENT_RPC_H

#include <list>
#include "tdsPacket.h"
#include "tdsAllHeaders.h"
#include "tdsTypeHelper.h"
#include "SMP.h"

enum SpID_RPC
{
    Sp_Cursor = 1,
    Sp_CursorOpen = 2,
    Sp_CursorPrepare = 3,
    Sp_CursorExecute = 4,
    Sp_CursorPrepExec = 5,
    Sp_CursorUnprepare = 6,
    Sp_CursorFetch = 7,
    Sp_CursorOption = 8,
    Sp_CursorClose = 9,
    Sp_ExecuteSql = 10,
    Sp_Prepare = 11,
    Sp_Execute = 12,
    Sp_PrepExec = 13,
    Sp_PrepExecRpc = 14,
    Sp_Unprepare = 15,
};

#define RPC_PARAM_BINARY        0x01000000      // check this param is binary or not
#define RPC_PARAM_IGNORE_CASE   0x00100000      // check this param is ignore case or not

struct PLPDataContext
{
    uint32_t PLP_chunk_length;
    uint8_t* PLP_chunk_data;
};

struct ParameterDes_RPC
{
    uint8_t name_len;
    wstring param_name;
    uint8_t status_flags;
    uint8_t param_type;
    uint32_t max_len;

    struct
    {
        uint32_t flags;
        uint8_t  sortid;
    } collation;

    uint8_t  decimal_precision;
    uint8_t  decimal_scale;
    uint8_t  decimal_is_negative;
    uint32_t data_len;
    uint8_t* data;

    bool use_PLP_format;
    uint64_t PLP_vmax_length;
    std::list<PLPDataContext> PLP_data_list;

    ParameterDes_RPC()
    {
        name_len = 0;
        status_flags = 0;
        param_type = 0;
        max_len = 0;
        collation.flags = 0;
        collation.sortid = 0;
        decimal_precision = 0;
        decimal_scale = 0;
        decimal_is_negative = 0;
        data_len = 0;
        data = nullptr;
        use_PLP_format = false;
        PLP_vmax_length = 0;
    }
};

struct RpcText
{
    std::wstring text;
    uint32_t param_index;
};

class tdsClientRPC
{
public:
    tdsClientRPC(uint32_t sz);
    ~tdsClientRPC();
    
    void Parse(uint8_t* pBuff);
    //const wstring& GetSqlText();
    std::vector<RpcText> GetRpcTextList();
    void SetSqlTextIndex(uint32_t i) { m_param_sqltext_index = i; }
    void SetNewSqlText(const wstring& sql);
    void SetNewSqlText(const wchar_t* str);
    void Serialize(std::list<uint8_t*>& packets);
    void SetSmpLastRequestSeqnum(uint32_t seqnum) { m_smp_last_seqnum = seqnum; }
    void SetSmpLastRequestWndw(uint32_t wndw) { m_smp_last_wndw = wndw; }

private:
    void ParseParameters(uint8_t* buff, uint32_t length);
    uint32_t EstimateNewSize();
    void SerializeCache(uint32_t len);
    void UpdateCacheSqlText();

private:
    uint32_t    m_packetIndex;
    bool        m_has_smp;
    uint32_t    m_smp_last_seqnum;
    uint32_t    m_smp_last_wndw;
    SMPHeader   m_smp;
    tdsHeader   m_pkHead;
    tdsAllHeaders m_pkAllHead;

    uint32_t    m_max_packet_size;
    wstring     m_sqltext;
    
    uint16_t    m_procedure_name_len;
    wchar_t*    m_procedure_name;
    uint16_t    m_stored_procedure_id;
    uint16_t    m_option_flags;
    bool        m_use_cache;
    uint32_t    m_cache_len;
    uint8_t*    m_cache;
    uint32_t    m_param_sqltext_index;
    vector<ParameterDes_RPC*>    m_params;
};

#endif // TDS_CLIENT_RPC_H