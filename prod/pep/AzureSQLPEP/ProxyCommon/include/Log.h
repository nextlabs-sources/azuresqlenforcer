#pragma once

#ifndef SQLTDS_LOG_H
#define SQLTDS_LOG_H

#include "celog.h"

typedef void(*pfLogW)(int, const char*, int, const wchar_t*);
typedef void(*pfLogA)(int, const char*, int, const char*);

class ProxyLog
{
public:
    static ProxyLog* GetInstance();
    static void Release();

    void Load();

    void WriteLog(int nLevel, const char* file, int nline, const wchar_t* fmt, ...);
    void WriteLog(int nLevel, const char* file, int nline, const char* fmt, ...);

private:
    ProxyLog();
    ~ProxyLog();

private:
    static ProxyLog* m_pThis;
    pfLogW           m_funcW;
    pfLogA           m_funcA;
};

#define LOGPRINT(x, s, ...) ProxyLog::GetInstance()->WriteLog(x, __FILE__, __LINE__, s, __VA_ARGS__)

#endif