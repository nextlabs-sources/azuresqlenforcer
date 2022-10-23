#include <windows.h>
#include <string>
#include "ProxyDataAnalysis.h"
#include "CommonFunc.h"
#include "Log.h"

extern HMODULE g_hThisModule;

ProxyDataAnalysis::ProxyDataAnalysis()
    :m_processClient(nullptr)
    ,m_processServer(nullptr)
    ,m_taskInit(nullptr)
    ,m_isInit(false)
{

}

ProxyDataAnalysis::~ProxyDataAnalysis()
{

}

bool ProxyDataAnalysis::Init()
{
    wchar_t wcsModule[MAX_PATH] = { 0 };
    GetModuleFileNameW(g_hThisModule, wcsModule, MAX_PATH);

    std::wstring wsSQLTask = ProxyCommon::GetFileFolder(wcsModule) + L"\\SQLTask.dll";
    HMODULE hMod = LoadLibraryW(wsSQLTask.c_str());
    if (hMod == NULL) {
        PROXYLOG(CELOG_ERR, L"LoadLibrary SQLTask.dll failed! err: %d", GetLastError());
        return false;
    }

    m_processClient = (pf_ProcessClientPacket)GetProcAddress(hMod, "ProcessClientPacket");
    m_processServer = (pf_ProcessServerPacket)GetProcAddress(hMod, "ProcessServerPacket");
    m_taskInit      = (pf_init)GetProcAddress(hMod, "SQLTaskInit");
    if (m_processClient == NULL || m_processServer == NULL /*|| m_taskInit == NULL*/) {
        PROXYLOG(CELOG_ERR, L"Load SQLTask.dll func address failed!");
        return false;
    }

    //return true;
    m_isInit = m_taskInit();
    return m_isInit;
}

void ProxyDataAnalysis::ProcessClientPacket(
                                const std::list<uint8_t *>& oldPackets, 
                                std::list<uint8_t *>& newPackets, 
                                uint32_t maxPackSize, 
                                EnforceParams& enforceParams)
{
    if (m_processClient)
        m_processClient(oldPackets, newPackets, maxPackSize, enforceParams);
}

void ProxyDataAnalysis::ProcessServerPacket(
                                const std::list<uint8_t *>& oldPackets,
                                std::list<uint8_t *>& newPackets, 
                                uint8_t requestType, 
                                uint32_t maxPackSize, 
                                EnforceParams& enforceParams)
{
    if (m_processServer)
        m_processServer(oldPackets, newPackets, requestType, maxPackSize, enforceParams);
}