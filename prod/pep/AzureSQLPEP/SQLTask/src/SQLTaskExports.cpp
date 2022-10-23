#include "SQLTaskExports.h"
#include "TdsAnalysis.h"
#include "SQLEnforceHelper.h"
#include "Log.h"
#include "CommonFunc.h"

bool SQLTaskInit()
{
    try
    {
        return SQLEnforceMgr::GetInstance()->DAEInit(ProxyCommon::GetWorkerNameA());
    }
    catch (std::exception e)
    {
        LOGPRINT(CELOG_EMERG, "SQLTaskInit exception message: %s", e.what());
        return false;
    }
}

void ProcessClientPacket(
                    const std::list<uint8_t*>& oldPackets,
                    std::list<uint8_t*>& newPackets,
                    uint32_t maxPackSize,
                    EnforceParams& enforceParams
)
{
    TdsAnalysis ta;
    ta.SetMaxPacketSize(maxPackSize);
    ta.SetEnforceParams(&enforceParams);

    ta.ProcessClientMessage(oldPackets, newPackets);
}

void ProcessServerPacket(
                    const std::list<uint8_t*>& oldPackets,
                    std::list<uint8_t*>& newPackets,
                    uint8_t requestType,
                    uint32_t maxPackSize,
                    EnforceParams& enforceParams
)
{
    TdsAnalysis ta;
    ta.SetMaxPacketSize(maxPackSize);
    ta.SetEnforceParams(&enforceParams);

    ta.ProcessServerMessage(oldPackets, newPackets, requestType);
}