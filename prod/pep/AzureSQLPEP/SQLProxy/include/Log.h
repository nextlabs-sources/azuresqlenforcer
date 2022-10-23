#ifndef AZURE_SQL_LOG_H
#define AZURE_SQL_LOG_H

#include "celog.h"
#include <list>

enum LOG_POLICY
{
	LOG_POLICY_WinDbg=0x1,
	LOG_POLICY_StdErr=0x2,
	LOG_POLICY_File=0x4,
};

class CLogContent
{
public:
	CLogContent() : m_wszLog{ 0 } {}
	static void Init();

public:
	void *operator new(size_t size);
	void operator delete(void*);


public:
	wchar_t m_wszLog[CELOG_MAX_MESSAGE_SIZE_CHARS];
	int m_nlogLevel;

private:
	// private heap used for Alloc memory for object of this class.
	static HANDLE m_priHeap;
};

class CLog
{
public:
	bool Init(int nLogLevel, DWORD logPolicy);
    void SetLevel(int nLevel);
	int WriteLog(int lvl, const char* file, int line, const wchar_t* fmt, ...);
    int WriteLog(int lvl, const char* file, int line, const char* fmt, ...);

protected:
	std::wstring GetLogFile();
	//std::wstring GetLogFileFolder();
	//void CleanLogFile(DWORD intervalSec);

#if ASYN_LOG
private:
	static DWORD WINAPI WriteLogThread(LPVOID pParam);

private:
	CRITICAL_SECTION  m_csLogs;
	std::list<CLogContent*> m_lstLogs;
#endif

    int m_log_level;
};


extern CLog theLog;

#define PROXYLOG(l, s, ...) theLog.WriteLog(l, __FILE__, __LINE__, s, __VA_ARGS__)

#endif 