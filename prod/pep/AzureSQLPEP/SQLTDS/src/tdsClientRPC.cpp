#include "tdsClientRPC.h"
#include "TDSException.h"
#include "Log.h"

tdsClientRPC::tdsClientRPC(uint32_t sz)
    : m_cache(nullptr)
    , m_max_packet_size(sz)
    , m_cache_len(0)
    , m_procedure_name_len(0)
    , m_procedure_name(nullptr)
    , m_stored_procedure_id(0)
    , m_option_flags(0)
    , m_use_cache(false)
    , m_param_sqltext_index(-1)
    , m_packetIndex(0)
    , m_has_smp(false)
{
    
}

tdsClientRPC::~tdsClientRPC()
{
    for (size_t i = 0; i < m_params.size(); i++)
    {
        if (m_params[i]->data) {
            delete[] m_params[i]->data;
            m_params[i]->data = nullptr;
        }

        for (auto it : m_params[i]->PLP_data_list)
        {
            if (it.PLP_chunk_data)
                delete[] (it.PLP_chunk_data);
        }
        m_params[i]->PLP_data_list.clear();
        delete m_params[i];
    }
    m_params.clear();

    if (m_cache) {
        free(m_cache);
        m_cache = nullptr;
    }

    if (m_procedure_name) {
        delete[] m_procedure_name;
        m_procedure_name = nullptr;
    }
}

std::vector<RpcText> tdsClientRPC::GetRpcTextList()
{
    std::vector<RpcText> texts;

    for (size_t i = 0; i < m_params.size(); i++)
    {
        if (m_params[i]->collation.flags & RPC_PARAM_BINARY)
            continue;

        if (m_params[i]->param_type == NVARCHARTYPE
            || m_params[i]->param_type == NTEXTTYPE
            || m_params[i]->param_type == BIGVARCHRTYPE
            || m_params[i]->param_type == BIGCHARTYPE
            || m_params[i]->param_type == NCHARTYPE
            || m_params[i]->param_type == TEXTTYPE)
        {
            if (m_params[i]->use_PLP_format) {
                RpcText rt;
                rt.param_index = i;

                for (auto it : m_params[i]->PLP_data_list)
                {
                    if (it.PLP_chunk_data)
                        rt.text.append((WCHAR*)it.PLP_chunk_data, (WCHAR*)(it.PLP_chunk_data + it.PLP_chunk_length));
                }

                texts.push_back(rt);
            }
            else {
                if (m_params[i]->data) {
                    texts.push_back({ (WCHAR*)(m_params[i]->data) , (uint32_t)i });
                }
            }
        }
    }

    return texts;
}

void tdsClientRPC::SetNewSqlText(const wchar_t* str)
{
    m_sqltext = str ? str : L"";

    UpdateCacheSqlText();
}

void tdsClientRPC::SetNewSqlText(const wstring& sql)
{
    m_sqltext = sql;

    UpdateCacheSqlText();
}

void tdsClientRPC::UpdateCacheSqlText()
{
    if (m_param_sqltext_index != -1) {
        // make ignore case. new sql text is uppercase
        m_params[m_param_sqltext_index]->collation.flags |= RPC_PARAM_IGNORE_CASE;

        if (m_params[m_param_sqltext_index]->use_PLP_format) {
            m_params[m_param_sqltext_index]->PLP_vmax_length = 0xFFFFFFFFFFFFFFFE;

            for (auto it : m_params[m_param_sqltext_index]->PLP_data_list)
            {
                if (it.PLP_chunk_data)
                    delete[] (it.PLP_chunk_data);
            }
            m_params[m_param_sqltext_index]->PLP_data_list.clear();

            if (m_sqltext.length() * 2 > 0x7fffffff)
            {
                size_t have_write = 0;
                size_t need_write = m_sqltext.length() * 2;
                const uint8_t* text_data = (const uint8_t*)m_sqltext.c_str();

                while (need_write > 0)
                {
                    PLPDataContext pdc;
                    if (need_write > 0x00007d00)
                        pdc.PLP_chunk_length = 0x00007d00;
                    else
                        pdc.PLP_chunk_length = need_write;

                    pdc.PLP_chunk_data = new uint8_t[pdc.PLP_chunk_length];
                    memcpy(pdc.PLP_chunk_data, text_data + have_write, pdc.PLP_chunk_length);
                    have_write += pdc.PLP_chunk_length;

                    m_params[m_param_sqltext_index]->PLP_data_list.push_back(pdc);

                    need_write -= pdc.PLP_chunk_length;
                }
            }
            else
            {
                PLPDataContext pdc;
                pdc.PLP_chunk_length = m_sqltext.length() * 2;
                pdc.PLP_chunk_data = new uint8_t[pdc.PLP_chunk_length];
                memcpy(pdc.PLP_chunk_data, m_sqltext.c_str(), pdc.PLP_chunk_length);

                m_params[m_param_sqltext_index]->PLP_data_list.push_back(pdc);
            }
        }
        else {
            if (m_params[m_param_sqltext_index]->data)
                delete[] m_params[m_param_sqltext_index]->data;
            
            m_params[m_param_sqltext_index]->data = new uint8_t[(m_sqltext.length() + 1) * sizeof(wchar_t)];
            m_params[m_param_sqltext_index]->data_len = m_sqltext.length() * sizeof(wchar_t);
            m_params[m_param_sqltext_index]->max_len = m_sqltext.length() * sizeof(wchar_t);
            memset(m_params[m_param_sqltext_index]->data, 0, (m_sqltext.length() + 1) * sizeof(wchar_t));
            memcpy(m_params[m_param_sqltext_index]->data, m_sqltext.c_str(), m_params[m_param_sqltext_index]->data_len);
        }
    }
}

void tdsClientRPC::Parse(uint8_t* pBuff)
{
    tdsHeader header;
    uint8_t* pTdsBuff = nullptr;

    if (pBuff[0] == SMP_PACKET_FLAG) {
        m_has_smp = true;
        header.Parse(pBuff + SMP_HEADER_LEN);
        pTdsBuff = pBuff + SMP_HEADER_LEN;
    }
    else {
        header.Parse(pBuff);
        pTdsBuff = pBuff;
    }

    if (header.GetTdsPacketType() != TDS_RPC)
        return;

    if (header.GetPacketLength() > m_max_packet_size)
    {
        throw TDSException("RPC parse wrong packet length!");
    }

    if (header.GetPacketID() == 1 && m_packetIndex == 0)
    {
        if (pBuff[0] == SMP_PACKET_FLAG) {
            m_smp.Parse(pBuff);
        }

        m_pkHead.Parse(pTdsBuff);
        m_pkAllHead.Parse(pTdsBuff + TDS_HEADER_LEN);

        uint8_t* p = pTdsBuff + sizeof(tdsPacketHeader) + m_pkAllHead.GetTotalLength();
        m_procedure_name_len = *(uint16_t*)p; p += 2;
        if (m_procedure_name_len == 0xffff)
        {
            m_stored_procedure_id = *(uint16_t*)(p);
            p += 2;
            LOGPRINT(CELOG_DEBUG, L"RPC Stored procedure id: %d", m_stored_procedure_id);

            if (m_stored_procedure_id == 0 || m_stored_procedure_id > 15)
                ThrowTdsException("RPC parse, wrong m_stored_procedure_id [%d]", m_stored_procedure_id);
        }
        else if (m_procedure_name_len + TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() > m_pkHead.GetPacketLength())
        {
            ThrowTdsException("RPC parse, procedure name length too long, %d", m_procedure_name_len);
        }
        else 
        {
            if (m_procedure_name_len > m_pkHead.GetPacketLength() - m_pkAllHead.GetTotalLength() - 2) {
                LOGPRINT(CELOG_WARNING, L"RPC procedure name length too long, is invalid. len: %d", m_procedure_name_len);
                return;
            }
            // It is calling the real stored procedure. We do nothing.
            m_procedure_name = new wchar_t[m_procedure_name_len + 1];
            memset(m_procedure_name, 0, (m_procedure_name_len + 1) * sizeof(wchar_t));
            memcpy(m_procedure_name, p, m_procedure_name_len * sizeof(wchar_t));
            p = p + m_procedure_name_len * sizeof(wchar_t);
            LOGPRINT(CELOG_DEBUG, L"Call the real RPC Stored procedure: \n%s\n", m_procedure_name);

            if (m_pkHead.GetPacketLength() <= (TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 2 + m_procedure_name_len * sizeof(wchar_t) + 2))
                return;
        }
        
        m_option_flags = *(uint16_t*)(p);
    }

    // now RPC just for Sp_ExecuteSql Sp_PrepExec Sp_Prepare
    if (m_stored_procedure_id != Sp_ExecuteSql 
        && m_stored_procedure_id != Sp_PrepExec
        && m_stored_procedure_id != Sp_Prepare
        && !(m_procedure_name_len != 0xffff && m_procedure_name_len > 0))
    {
        return;
    }

    // the complete RPC packet has more than one packet
    if (header.GetPacketID() == 1 && m_packetIndex == 0 && !(header.GetStatus() & 0x01))
        m_use_cache = true;

    // the data is incomplete, cache it.
    if (m_use_cache)
    {
        if (header.GetPacketID() == 1 && m_packetIndex == 0)
        {
            uint32_t _offset = 0;
            if (m_procedure_name_len != 0xffff && m_procedure_name_len > 0)
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 2 + m_procedure_name_len * sizeof(wchar_t) + 2;
            else
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 6;

            m_cache_len = header.GetPacketLength() - _offset;
            m_cache = (uint8_t *)malloc(m_cache_len);
            if (m_cache == NULL)
                throw std::runtime_error("parse failed! memory alloc failed!");

            memcpy(m_cache, pTdsBuff + _offset, m_cache_len);
        }
        else
        {
            uint32_t len = header.GetPacketLength() - sizeof(tdsPacketHeader);
            void *p = realloc(m_cache, m_cache_len + len);
            if (p == NULL)
            {
                free(m_cache);
                m_cache = NULL;
                throw std::runtime_error("parse failed! memory realloc failed!");
            }

            m_cache = (uint8_t *)p;
            memcpy(m_cache + m_cache_len, pTdsBuff + sizeof(tdsPacketHeader), len);
            m_cache_len += len;
        }
    }

    m_packetIndex++;

    // get all parameters
    if (header.GetStatus() & 0x01)
    {
        uint32_t length = 0;
        uint8_t* p = nullptr;
        if (m_use_cache) {
            p = m_cache;
            length = m_cache_len;
        }
        else {
            uint32_t _offset = 0;
            if (m_procedure_name_len != 0xffff && m_procedure_name_len > 0)
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 2 + m_procedure_name_len * sizeof(wchar_t) + 2;
            else
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 6;

            p = pTdsBuff + _offset;
            length = header.GetPacketLength() - _offset;
        }

        ParseParameters(p, length);
    }
}

void tdsClientRPC::ParseParameters(uint8_t* buff, uint32_t length)
{
    uint8_t* p = buff;
    while (p - buff < length)
    {
        //LOGPRINT(CELOG_DEBUG, "RPC parse buff: %p offset: %d", buff, (p - buff));
        ParameterDes_RPC* param = new ParameterDes_RPC;
        m_params.push_back(param);

        param->name_len = *p;
        p++;
        if (param->name_len != 0) {
            param->param_name.append((WCHAR*)p, param->name_len);
            p = p + param->name_len * sizeof(WCHAR);
        }
        param->status_flags = *p; p++;
        param->param_type = *p; p++;
        if (tdsTypeHelper::GetTdsDataType(param->param_type) == UNKNOW_TYPE)
        {
            ThrowTdsException("RPC parse [line:%d], unkown param type, %d", __LINE__, param->param_type);
        }

        TDS_LEN_TYPE len_type = tdsTypeHelper::GetLengthType((TDS_DATA_TYPE)(param->param_type));
        if ((TDS_DATA_TYPE)(param->param_type) == BIGVARBINTYPE && (*(uint32_t*)p == 0x7FFFFFFF))
        {
            param->max_len = 0x7FFFFFFF;
            len_type = LONG_LEN_TYPE;
            p += 4;
        }
        else
        {
            if (len_type == USHORT_LEN_TYPE) {
                param->max_len = *(uint16_t*)p;
                p += 2;
            }
            else if (len_type == LONG_LEN_TYPE) {
                param->max_len = *(uint32_t*)p;
                p += 4;
            }
            else if (len_type == BYTE_LEN_TYPE) {
                param->max_len = *p;
                p++;
            }
        }

        if ((TDS_DATA_TYPE)(param->param_type) == DATENTYPE)
        {
            if (param->max_len == 0)
                continue;

            if (param->max_len + p > buff + length)
                ThrowTdsException("RPC parse [line:%d], DATE data length too long, %d, p: %p, buff: %p", __LINE__, param->max_len, p, buff + length);

            param->data = new uint8_t[param->max_len];
            memcpy(param->data, p, param->max_len);
            p += param->max_len;
            continue;
        }

        if ((TDS_DATA_TYPE)(param->param_type) == DECIMALNTYPE)
        {
            param->decimal_precision = *p;
            p++;
            param->decimal_scale = *p;
            p++;
            param->data_len = *p;
            p++;

            if (param->data_len + p > buff + length)
            {
                ThrowTdsException("RPC parse [line:%d], DECIMALNTYPE data length too long, %d, p: %p, buff: %p", __LINE__, param->data_len, p, buff + length);
            }

            if (param->data_len > 0)
            {
                param->decimal_is_negative = *p;
                p++;

                param->data = new uint8_t[param->data_len - 1];
                memcpy(param->data, p, param->data_len - 1);
                p += param->data_len - 1;
            }
            continue;
        }
        
        if (tdsTypeHelper::HaveCollation((TDS_DATA_TYPE)(param->param_type))) {
            param->collation.flags = *(uint32_t*)p;
            p += 4;
            param->collation.sortid = *p;
            p++;
        }

        if (param->max_len == 0xffff && len_type == USHORT_LEN_TYPE)
        {
            // for set breakpoint at debug
            Sleep(0);
        }
        else
        {
            if (len_type == USHORT_LEN_TYPE) {
                param->data_len = *(uint16_t*)p;
                p += 2;
            }
            else if (len_type == LONG_LEN_TYPE) {
                param->data_len = *(uint32_t*)p;
                p += 4;
            }
            else if (len_type == BYTE_LEN_TYPE) {
                param->data_len = *p;
                p++;
            }

            if ((param->data_len == 0xff && len_type == BYTE_LEN_TYPE)
                || (param->data_len == 0xffff && len_type == USHORT_LEN_TYPE)
                || (param->data_len == 0xffffffff && len_type == LONG_LEN_TYPE)
                || param->max_len == 0 || param->data_len == 0)
            {
                // null value
            }
            else
            {
                if (param->data_len + p > buff + length)
                    ThrowTdsException("RPC parse [line:%d], param type: %d, data length too long: %d, p: %p, buff: %p", __LINE__, param->param_type, param->data_len, p, buff + length);
            }
        }

        if (param->max_len == 0xffff && len_type == USHORT_LEN_TYPE)
        {
            // PLP format
            param->use_PLP_format = true;
            param->PLP_vmax_length = *(uint64_t*)p;
            p += 8;
            if (param->PLP_vmax_length != 0xffffffffffffffff)
            {
                while (1)
                {
                    PLPDataContext pdc;
                    pdc.PLP_chunk_data = nullptr;
                    pdc.PLP_chunk_length = *(uint32_t*)p;
                    p += 4;
                    if (pdc.PLP_chunk_length + p > buff + length)
                        ThrowTdsException("RPC parse [line:%d], param type: %d, PLP data length too long: %lld, p: %p, buff: %p", __LINE__, param->param_type, pdc.PLP_chunk_length, p, buff + length);

                    if (pdc.PLP_chunk_length == 0)
                        break;

                    if (pdc.PLP_chunk_length > 0)
                    {
                        pdc.PLP_chunk_data = new uint8_t[pdc.PLP_chunk_length];
                        memcpy(pdc.PLP_chunk_data, p, pdc.PLP_chunk_length);
                        p += pdc.PLP_chunk_length;
                    }

                    param->PLP_data_list.push_back(pdc);
                }
            }
        }
        else if (param->max_len != 0 && param->data_len != 0 
            && !(param->max_len == 0xffff && len_type == USHORT_LEN_TYPE)
            && !(param->data_len == 0xffffffff && len_type == LONG_LEN_TYPE)
            && !(param->data_len == 0xffff && len_type == USHORT_LEN_TYPE)
            && !(param->data_len == 0xff && len_type == BYTE_LEN_TYPE)) {
            if (param->param_type == NVARCHARTYPE
                || param->param_type == NTEXTTYPE
                || param->param_type == BIGVARCHRTYPE
                || param->param_type == BIGCHARTYPE
                || param->param_type == NCHARTYPE
                || param->param_type == TEXTTYPE)
            {
                param->data = new uint8_t[param->data_len + 2];
                memset(param->data, 0, param->data_len + 2);
            }
            else
            {
                param->data = new uint8_t[param->data_len];
            }
            
            memcpy(param->data, p, param->data_len);
            p += param->data_len;
        }
    }
}

uint32_t tdsClientRPC::EstimateNewSize()
{
    uint32_t data_total_len = 0;
    for (size_t i = 0; i < m_params.size(); i++)
    {
        data_total_len++;
        if (m_params[i]->name_len != 0)
            data_total_len += (m_params[i]->name_len * sizeof(wchar_t));
        data_total_len += 2;

        TDS_LEN_TYPE len_type = tdsTypeHelper::GetLengthType((TDS_DATA_TYPE)(m_params[i]->param_type));
        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == BIGVARBINTYPE && m_params[i]->max_len == 0x7FFFFFFF)
        {
            data_total_len += 4;
            len_type = LONG_LEN_TYPE;
        }
        else
        {
            if (len_type == BYTE_LEN_TYPE)          data_total_len++;
            else if (len_type == USHORT_LEN_TYPE)   data_total_len += 2;
            else if (len_type == LONG_LEN_TYPE)     data_total_len += 4;
        }

        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == DATENTYPE)
        {
            if (m_params[i]->max_len == 0)
                continue;

            if (m_params[i]->max_len > 0) {
                data_total_len += m_params[i]->max_len;
                continue;
            }
        }

        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == DECIMALNTYPE)
        {
            data_total_len += 3;
            data_total_len += m_params[i]->data_len;
            continue;
        }

        if (tdsTypeHelper::HaveCollation((TDS_DATA_TYPE)(m_params[i]->param_type))) {
            data_total_len += 5;
        }

        if (!(m_params[i]->max_len == 0xffff && len_type == USHORT_LEN_TYPE)) {
            if (len_type == BYTE_LEN_TYPE)          data_total_len++;
            else if (len_type == USHORT_LEN_TYPE)   data_total_len += 2;
            else if (len_type == LONG_LEN_TYPE)     data_total_len += 4;

            if (m_params[i]->data_len != 0
                && !(m_params[i]->data_len == 0xffffffff && len_type == LONG_LEN_TYPE)
                && !(m_params[i]->data_len == 0xffff && len_type == USHORT_LEN_TYPE)
                && !(m_params[i]->data_len == 0xff && len_type == BYTE_LEN_TYPE))
            {
                data_total_len += m_params[i]->data_len;
            }
        }
        else if (m_params[i]->use_PLP_format)
        {
            data_total_len += 8;
            if (m_params[i]->PLP_vmax_length != 0xffffffffffffffff)
            {
                for (auto it : m_params[i]->PLP_data_list)
                {
                    if (it.PLP_chunk_data && it.PLP_chunk_length > 0) {
                        data_total_len += 4;
                        data_total_len += it.PLP_chunk_length;
                    }
                }
                data_total_len += 4;
            }
        }
    }

    return data_total_len;
}

void tdsClientRPC::Serialize(std::list<uint8_t*>& packets)
{
    uint32_t data_new_len = EstimateNewSize();
    if (data_new_len == 0)
        return;

    if (data_new_len != m_cache_len) {
        if (m_cache)
            free(m_cache);
        m_cache = (uint8_t*)malloc(data_new_len);
        if (m_cache == NULL) {
            throw std::runtime_error("Client RPC Serialize failed! Cache malloc failed!");
        }
        m_cache_len = data_new_len;
    }

    SerializeCache(data_new_len);

    bool packet_start = true;
    uint32_t smp_seqnum = m_smp_last_seqnum + 1;
    uint8_t packet_id = 1;
    uint32_t need_len = m_cache_len;
    while (need_len > 0)
    {
        if (packet_start)
        {
            packet_start = false;
            uint32_t _offset = 0;
            if (m_procedure_name_len != 0xffff && m_procedure_name_len > 0)
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 2 + m_procedure_name_len * sizeof(wchar_t) + 2;
            else
                _offset = TDS_HEADER_LEN + m_pkAllHead.GetTotalLength() + 6;

            uint32_t packet_len = _offset + need_len;
            if (packet_len > m_max_packet_size) {
                uint32_t maloc_len = m_max_packet_size;
                if (m_has_smp)
                    maloc_len += SMP_HEADER_LEN;

                uint8_t* buff = new uint8_t[maloc_len];
                uint8_t* tds_buff = nullptr;
                if (m_has_smp) {
                    SMP::SerializeSMPHeader(buff, m_smp.m_flags, m_smp.m_sid, maloc_len, smp_seqnum++, m_smp_last_wndw);
                    tds_buff = buff + SMP_HEADER_LEN;
                }
                else
                    tds_buff = buff;

                m_pkHead.Serialize(tds_buff);
                tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
                pHead->PacketLength = htons(m_max_packet_size);
                pHead->PacketID = packet_id++;
                if (pHead->PacketStatus & 0x01)
                    pHead->PacketStatus = 0x04;

                m_pkAllHead.Serialize(tds_buff + TDS_HEADER_LEN);

                uint8_t* pTmp = tds_buff + TDS_HEADER_LEN + m_pkAllHead.GetTotalLength();
                *(uint16_t*)pTmp = m_procedure_name_len;
                if (m_procedure_name_len != 0xffff && m_procedure_name_len > 0) {
                    memcpy(pTmp + 2, m_procedure_name, m_procedure_name_len * sizeof(wchar_t));
                    *(uint16_t*)(pTmp + 2 + m_procedure_name_len * sizeof(wchar_t)) = m_option_flags;
                }
                else {
                    *(uint16_t*)(pTmp + 2) = m_stored_procedure_id;
                    *(uint16_t*)(pTmp + 4) = m_option_flags;
                }

                memcpy(tds_buff + _offset, 
                    m_cache, 
                    m_max_packet_size - _offset);

                need_len -= (m_max_packet_size - _offset);

                packets.push_back(buff);
            }
            else {
                uint32_t maloc_len = packet_len;
                if (m_has_smp)
                    maloc_len += SMP_HEADER_LEN;

                uint8_t* buff = new uint8_t[maloc_len];
                uint8_t* tds_buff = buff;
                if (m_has_smp) {
                    SMP::SerializeSMPHeader(buff, m_smp.m_flags, m_smp.m_sid, maloc_len, smp_seqnum++, m_smp_last_wndw);
                    tds_buff = buff + SMP_HEADER_LEN;
                }

                m_pkHead.Serialize(tds_buff);
                tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
                pHead->PacketLength = htons(packet_len);
                pHead->PacketID = packet_id++;
                pHead->PacketStatus = 0x01;

                m_pkAllHead.Serialize(tds_buff + TDS_HEADER_LEN);

                uint8_t* pTmp = tds_buff + TDS_HEADER_LEN + m_pkAllHead.GetTotalLength();
                *(uint16_t*)pTmp = m_procedure_name_len;
                if (m_procedure_name_len != 0xffff && m_procedure_name_len > 0) {
                    memcpy(pTmp + 2, m_procedure_name, m_procedure_name_len * sizeof(wchar_t));
                    *(uint16_t*)(pTmp + 2 + m_procedure_name_len * sizeof(wchar_t)) = m_option_flags;
                }
                else {
                    *(uint16_t*)(pTmp + 2) = m_stored_procedure_id;
                    *(uint16_t*)(pTmp + 4) = m_option_flags;
                }

                memcpy(tds_buff + _offset, m_cache, need_len);
                need_len = 0;

                packets.push_back(buff);
            }
        }
        else
        {
            uint32_t packet_len = sizeof(tdsPacketHeader) + need_len;
            uint8_t* buff = nullptr;
            uint32_t maloc_len = 0;
            uint32_t tds_len = 0;
            if (packet_len > m_max_packet_size) {
                tds_len = m_max_packet_size;
                maloc_len = m_max_packet_size;
            }
            else {
                tds_len = packet_len;
                maloc_len = packet_len;
            }

            if (m_has_smp)
                maloc_len += SMP_HEADER_LEN;

            buff = new uint8_t[maloc_len];
            uint8_t* tds_buff = buff;
            if (m_has_smp) {
                SMP::SerializeSMPHeader(buff, m_smp.m_flags, m_smp.m_sid, maloc_len, smp_seqnum++, m_smp_last_wndw);
                tds_buff = buff + SMP_HEADER_LEN;
            }

            m_pkHead.Serialize(tds_buff);
            tdsPacketHeader* pHead = (tdsPacketHeader*)tds_buff;
            pHead->PacketLength = htons(tds_len);
            pHead->PacketID = packet_id++;
            if (packet_len > m_max_packet_size) {
                if (pHead->PacketStatus & 0x01) 
                    pHead->PacketStatus = 0x04;
            }
            else {
                pHead->PacketStatus = 0x01;
            }

            memcpy(tds_buff + TDS_HEADER_LEN, m_cache + (m_cache_len - need_len), tds_len - TDS_HEADER_LEN);
            need_len -= (tds_len - TDS_HEADER_LEN);

            packets.push_back(buff);
        }
    }
}

void tdsClientRPC::SerializeCache(uint32_t len)
{
    uint32_t use_len = 0;
    uint8_t* p = m_cache;
    for (size_t i = 0; i < m_params.size() && use_len <= len; i++)
    {
        *p = m_params[i]->name_len; p++;
        use_len++;
        if (m_params[i]->name_len > 0) {
            memcpy(p, m_params[i]->param_name.c_str(), m_params[i]->name_len * sizeof(wchar_t));
            p += (m_params[i]->name_len * sizeof(wchar_t));
            use_len += (m_params[i]->name_len * sizeof(wchar_t));
        }

        *p = m_params[i]->status_flags; p++; use_len++;
        *p = m_params[i]->param_type; p++; use_len++;

        TDS_LEN_TYPE len_type = tdsTypeHelper::GetLengthType((TDS_DATA_TYPE)(m_params[i]->param_type));
        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == BIGVARBINTYPE && m_params[i]->max_len == 0x7FFFFFFF)
        {
            len_type = LONG_LEN_TYPE;
            *(uint32_t*)p = 0x7FFFFFFF;
            p += 4;
            use_len += 4;
        }
        else
        {
            if (len_type == BYTE_LEN_TYPE) {
                *p = (uint8_t)(m_params[i]->max_len);
                p++;
                use_len++;
            }
            else if (len_type == USHORT_LEN_TYPE) {
                *(uint16_t*)p = (uint16_t)(m_params[i]->max_len);
                p += 2;
                use_len += 2;
            }
            else if (len_type == LONG_LEN_TYPE) {
                *(uint32_t*)p = m_params[i]->max_len;
                p += 4;
                use_len += 4;
            }
        }

        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == DATENTYPE)
        {
            if (m_params[i]->max_len == 0)
                continue;

            if (m_params[i]->max_len > 0)
            {
                memcpy(p, m_params[i]->data, m_params[i]->max_len);
                p += m_params[i]->max_len;
                use_len += m_params[i]->max_len;
                continue;
            }
        }

        if ((TDS_DATA_TYPE)(m_params[i]->param_type) == DECIMALNTYPE)
        {
            *p = m_params[i]->decimal_precision; p++; use_len++;
            *p = m_params[i]->decimal_scale; p++; use_len++;
            *p = (uint8_t)(m_params[i]->data_len); p++; use_len++;
            if (m_params[i]->data_len > 0)
            {
                *p = m_params[i]->decimal_is_negative; p++; use_len++;
                memcpy(p, m_params[i]->data, m_params[i]->data_len - 1);
                p += m_params[i]->data_len - 1;
                use_len += m_params[i]->data_len - 1;
            }
            continue;
        }

        if (tdsTypeHelper::HaveCollation((TDS_DATA_TYPE)(m_params[i]->param_type))) {
            *(uint32_t*)p = m_params[i]->collation.flags;
            p += 4; use_len += 4;
            *p = m_params[i]->collation.sortid;
            p++; use_len++;
        }

        if (!(m_params[i]->max_len == 0xffff && len_type == USHORT_LEN_TYPE))
        {
            if (len_type == BYTE_LEN_TYPE) {
                *p = (uint8_t)(m_params[i]->data_len);
                p++; 
                use_len++;
            }
            else if (len_type == USHORT_LEN_TYPE) {
                *(uint16_t*)p = (uint16_t)(m_params[i]->data_len);
                p += 2;
                use_len += 2;
            }
            else if (len_type == LONG_LEN_TYPE) {
                *(uint32_t*)p = m_params[i]->data_len;
                p += 4;
                use_len += 4;
            }

            if (m_params[i]->max_len != 0 && m_params[i]->data_len != 0 
                && !(m_params[i]->data_len == 0xffffffff && len_type == LONG_LEN_TYPE)
                && !(m_params[i]->data_len == 0xffff && len_type == USHORT_LEN_TYPE)
                && !(m_params[i]->data_len == 0xff && len_type == BYTE_LEN_TYPE))
            {
                memcpy(p, m_params[i]->data, m_params[i]->data_len);
                p += m_params[i]->data_len;
                use_len += m_params[i]->data_len;
            }
        }
        else if (m_params[i]->use_PLP_format)
        {
            *(uint64_t*)p = m_params[i]->PLP_vmax_length;
            p += 8;
            use_len += 8;
            if (m_params[i]->PLP_vmax_length != 0xffffffffffffffff)
            {
                for (auto it : m_params[i]->PLP_data_list)
                {
                    if (it.PLP_chunk_data && it.PLP_chunk_length > 0)
                    {
                        *(uint32_t*)p = it.PLP_chunk_length;
                        p += 4;
                        use_len += 4;
                        memcpy(p, it.PLP_chunk_data, it.PLP_chunk_length);
                        p += it.PLP_chunk_length;
                        use_len += it.PLP_chunk_length;
                    }
                }

                // PLP terminate
                *(uint32_t*)p = 0;
                p += 4;
                use_len += 4;
            }
        }
        else
        {
            assert(false);
        }
    }

    if (use_len != len)
    {
        LOGPRINT(CELOG_EMERG, "SerializeCache use_len != malloc_len, use_len [%d]  malloc_len[%d]", use_len, len);
    }
}