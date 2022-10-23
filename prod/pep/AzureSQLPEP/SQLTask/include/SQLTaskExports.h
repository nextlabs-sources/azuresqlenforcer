#pragma once

#ifndef __SQLTASK_EXPORTS_H__
#define __SQLTASK_EXPORTS_H__

#include <stdint.h>
#include <list>
#include <string>

#ifdef SQLTASK_EXPORT
#define SQLTASKAPI __declspec(dllexport)
#else
#define SQLTASKAPI __declspec(dllimport)
#endif

struct EnforceParams
{
    std::string user_ip;
    std::string sql_user;
    std::string domain_name;
    std::string computer_name;
    std::string database;
    std::string appname;
    uint32_t smp_last_request_seqnum;
    uint32_t smp_last_request_wndw;
};

typedef bool(*pf_init)();

typedef void(*pf_ProcessClientPacket)(
                            const std::list<uint8_t*>& oldPackets,
                            std::list<uint8_t*>& newPackets,
                            uint32_t maxPackSize,
                            EnforceParams& enforceParams);

typedef void (*pf_ProcessServerPacket)(
                            const std::list<uint8_t*>& oldPackets,
                            std::list<uint8_t*>& newPackets,
                            uint8_t requestType,
                            uint32_t maxPackSize,
                            EnforceParams& enforceParams);

extern "C" {
    SQLTASKAPI bool SQLTaskInit();

    SQLTASKAPI void ProcessClientPacket(
                            const std::list<uint8_t*>& oldPackets, 
                            std::list<uint8_t*>& newPackets, 
                            uint32_t maxPackSize,
                            EnforceParams& enforceParams
                            );
    
    SQLTASKAPI void ProcessServerPacket(
                            const std::list<uint8_t*>& oldPackets, 
                            std::list<uint8_t*>& newPackets, 
                            uint8_t requestType, 
                            uint32_t maxPackSize,
                            EnforceParams& enforceParams
                            );
}

#endif