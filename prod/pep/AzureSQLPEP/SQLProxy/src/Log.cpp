#define ASYN_LOG   0

#include "Log.h"
#include "celog_policy_file.hpp"
#include "celog_policy_stderr.hpp"
#include "celog_policy_windbg.hpp"
#include "CriticalSectionLock.h"
#include "CommonFunc.h"
#include <Shlobj.h>
#include <shlwapi.h>

CLog theLog;

HANDLE CLogContent::m_priHeap = NULL;

void CLogContent::Init()
{
	if (m_priHeap==NULL)
	{
		m_priHeap = HeapCreate(0, 0, 0);
	}
}

void* CLogContent::operator new(size_t size)
{
	if (m_priHeap!=NULL)
	{
		return HeapAlloc(m_priHeap, 0, size);
	}
	return NULL;
}

void CLogContent::operator delete(void* p)
{
	if (m_priHeap!=NULL)
	{
		HeapFree(m_priHeap, 0, p);
	}
}

bool CLog::Init(int nLogLevel, DWORD logPolicy)
{
	//CleanLogFile(7 * 24 * 3600);

	//init logcontent
	//CLogContent::Init();

    m_log_level = nLogLevel;
	//
	celog_Enable();
	CELogS::Instance()->SetLevel(nLogLevel);

	if (logPolicy&LOG_POLICY_StdErr){
		CELogS::Instance()->SetPolicy(new CELogPolicy_Stderr());
	}
	
	if (logPolicy&LOG_POLICY_WinDbg){
		CELogS::Instance()->SetPolicy(new CELogPolicy_WinDbg());
	}

	if (logPolicy&LOG_POLICY_File) {
		std::wstring strLogFile = GetLogFile();
		CELogPolicy_File* policy = new CELogPolicy_File(ProxyCommon::WStringToString(strLogFile).c_str());
		policy->SetMaxLogSize(10 * 1024 * 1024); /* 100MB */
		CELogS::Instance()->SetPolicy(policy);
	}

	//
#if ASYN_LOG
	InitializeCriticalSection(&m_csLogs);
	CloseHandle(CreateThread(NULL, 0, WriteLogThread, this, 0, NULL));
#endif
	
	return true;
}

void CLog::SetLevel(int nLevel)
{
    m_log_level = nLevel;
    CELogS::Instance()->SetLevel(nLevel);
}

int CLog::WriteLog(int lvl, const char* file, int line, const wchar_t* fmt, ...)
{
	int nLog=0;
    
    if (lvl > m_log_level)
        return 0;

#if ASYN_LOG
	CLogContent* pLogContent = new CLogContent();

	//format log content
	va_list args;
	va_start(args, fmt);
	nLog = _vsnwprintf(pLogContent->m_wszLog, _countof(pLogContent->m_wszLog)-1, fmt, args);
	va_end(args);

	//pLogContent->m_wszLog[nLog] = 0;	// vswprintf return -1 when _Format size exceed _BufferCount
	pLogContent->m_nlogLevel = lvl;

	{
		CriticalSectionLock lockLogs(&m_csLogs);
		m_lstLogs.push_back(pLogContent);
	}
#else
	va_list args;
	va_start(args, fmt);
	nLog = CELogS::Instance()->Log(lvl, file, line, fmt, args);
	va_end(args);
#endif

	return nLog;
}

int CLog::WriteLog(int lvl, const char* file, int line, const char* fmt, ...)
{
    int nLog = 0;
    if (lvl > m_log_level)
        return 0;

    va_list args;
    va_start(args, fmt);
    nLog = CELogS::Instance()->Log(lvl, file, line, fmt, args);
    va_end(args);

    return nLog;
}

std::wstring CLog::GetLogFile()
{
    std::wstring wFolder = ProxyCommon::GetInstallFolder();

	if (!wFolder.empty())
	{
		std::wstring strLogFile = wFolder + L"\\log\\" + ProxyCommon::GetWorkerNameW();

        if (!PathFileExistsW(strLogFile.c_str()))
            SHCreateDirectory(NULL, strLogFile.c_str());
		//CreateDirectoryW(strLogFile.c_str(), NULL);
		strLogFile += L"\\TdsProxy_";
		strLogFile += ProxyCommon::GetLocalTimeString();
		strLogFile += L".txt";
		return strLogFile;
	}

	return L"";
}

/*
std::wstring CLog::GetLogFileFolder()
{
	std::wstring strAppDataFolder = ProxyCommon::GetProgramDataFolder();
	::OutputDebugStringW(strAppDataFolder.c_str());
	if (!strAppDataFolder.empty())
	{
		std::wstring strLogFile = strAppDataFolder + L"\\log";

		CreateDirectoryW(strLogFile.c_str(), NULL);
		strLogFile += L"\\";
		return strLogFile;
	}
	return L"";
}

void CLog::CleanLogFile(DWORD intervalSec)
{
	std::wstring folder = GetLogFileFolder();
	std::wstring file = folder + L"\\*.txt";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(file.c_str(), &FindFileData);
	
	SYSTEMTIME end = { 0 };
	GetLocalTime(&end);

	if (INVALID_HANDLE_VALUE == hFind)
		return;
	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			SYSTEMTIME start = ProxyCommon::LocalTimeStringToSYSTEMTIME(FindFileData.cFileName);
			double inter = ProxyCommon::IntervalOfSYSTEMTIME(start, end);
			if (inter > intervalSec)
			{
				std::string cur = ProxyCommon::WStringToString(folder + FindFileData.cFileName);
				remove(cur.c_str());
			}
		}
	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);
}
*/

#if ASYN_LOG
DWORD WINAPI CLog::WriteLogThread(LPVOID pParam)
{
	CLog* thisLog = reinterpret_cast<CLog*>(pParam);

	while (true)
	{
		Sleep(10 * 1000);//the log don't need write real time, so we don't use WaitSingleEvent, just sleep

		//get logs
		std::list<CLogContent*> lstLogs;
		//copy list to limit the lock time
		{
			CriticalSectionLock lockLogs(&thisLog->m_csLogs);
			lstLogs.assign(thisLog->m_lstLogs.begin(), thisLog->m_lstLogs.end());
			thisLog->m_lstLogs.clear();
		}

		//write log
		for (std::list<CLogContent*>::iterator itLog = lstLogs.begin(); itLog != lstLogs.end(); itLog++)
		{
			CLogContent* pLog = *itLog;
			CELogS::Instance()->Log(pLog->m_nlogLevel, pLog->m_wszLog);
			delete pLog;
		}
	}


}
#endif 
