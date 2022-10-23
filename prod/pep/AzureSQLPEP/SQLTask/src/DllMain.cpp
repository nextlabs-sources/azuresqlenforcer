#include <windows.h>

HINSTANCE g_module = NULL;

BOOL DllMain(HINSTANCE _DllHandle, unsigned long _Reason, void* _Reserved)
{
    switch (_Reason)
    {
    case DLL_PROCESS_ATTACH:
        g_module = _DllHandle;
        break;

    case DLL_PROCESS_DETACH:
        break;

    default:
        break;
    }
    return TRUE;
}