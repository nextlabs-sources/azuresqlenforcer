#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "Log.h"


ProxyLog* ProxyLog::m_pThis = nullptr;

ProxyLog* ProxyLog::GetInstance()
{
    if (m_pThis == nullptr)
    {
        m_pThis = new ProxyLog();
    }

    return m_pThis;
}

void ProxyLog::Release()
{
    if (m_pThis)
    {
        delete m_pThis;
        m_pThis = nullptr;
    }
}

ProxyLog::ProxyLog()
    :m_funcW(nullptr)
    ,m_funcA(nullptr)
{

}

ProxyLog::~ProxyLog()
{

}

void ProxyLog::Load()
{
    HMODULE hMod = GetModuleHandleW(L"SQLProxy.dll");
    if (hMod)
    {
        m_funcW = (pfLogW)GetProcAddress(hMod, "ProxyLogW");
        m_funcA = (pfLogA)GetProcAddress(hMod, "ProxyLogA");
    }
}

void ProxyLog::WriteLog(int nLevel, const char* file, int nline, const wchar_t* fmt, ...)
{
    if (m_funcW == NULL)
        Load();

    if (m_funcW == NULL)
        return;

    wchar_t* buff = new wchar_t[2048];
    memset(buff, 0, 4096);

    va_list args;
    va_start(args, fmt);
    _vsnwprintf(buff, 2047, fmt, args);
    va_end(args);

    m_funcW(nLevel, file, nline, buff);

    delete[] buff;
}

void ProxyLog::WriteLog(int nLevel, const char* file, int nline, const char* fmt, ...)
{
    if (m_funcA == NULL)
        Load();

    if (m_funcA == NULL)
        return;

    char* buff = new char[2048];
    memset(buff, 0, 2048);

    va_list args;
    va_start(args, fmt);
    _vsnprintf(buff, 2047, fmt, args);
    va_end(args);

    m_funcA(nLevel, file, nline, buff);

    delete[] buff;
}