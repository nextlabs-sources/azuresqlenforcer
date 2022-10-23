#pragma once

#include <fstream>
#include <comdef.h>

#include <activeds.h>
#include <sddl.h>

#include <shlwapi.h>
#pragma comment( lib , "activeds.lib" )
#pragma comment( lib , "adsiid.lib" )
#pragma comment(lib,"shlwapi.lib")

#include <windows.h>
#include <ole2.h>
#include <math.h>
#include <wchar.h>
#include <objbase.h>
#include <activeds.h>
#include <atlbase.h>
#include <string>

using namespace ATL;
#include <map>
#include <set>
#include <ctime>

class CWinAD
{
public:
	CWinAD(const std::string & conf_attrs);
public:
	~CWinAD(void);
private:
	// typedef struct _User_Info
	// {
	// 	std::wstring strSID;
	// 	std::wstring strEmail;
	// }User_Info;

	std::wstring m_strFileterKeyWords;
	//std::wstring m_strSidName;
	//std::wstring m_strMailName;
	//User_Info m_theUserInfo;

private:
	static long long ToInteger(LARGE_INTEGER const& integer)
	{
#ifdef INT64_MAX // Does the compiler natively support 64-bit integers?
		return integer.QuadPart;
#else
		return (static_cast<long long>(integer.HighPart) << 32) | integer.LowPart;
#endif
	}

	static std::tm ConvertIntoTm(const SYSTEMTIME st)
	{
		std::tm tm{};

		tm.tm_sec = st.wSecond;
		tm.tm_min = st.wMinute;
		tm.tm_hour = st.wHour;
		tm.tm_mday = st.wDay;
		tm.tm_mon = st.wMonth + 1;
		tm.tm_year = st.wYear - 1900;
		tm.tm_wday = st.wDayOfWeek;

		return tm;
	}

	static std::time_t GetTimeStamp(std::tm tm)
	{
		std::time_t time_stamp = mktime(&tm);
		return time_stamp;
	}

public:
    std::set<std::wstring> _attr_names;
	std::map<std::wstring, std::wstring> _attr;

private:

	HRESULT CWinAD::FindUsers(CComPtr<IDirectorySearch> pContainerToSearch,  // IDirectorySearch pointer to the container to search.
		LPOLESTR szFilter, // Filter for finding specific users.
		// NULL returns all user objects.
		LPOLESTR *pszPropertiesToReturn, // Properties to return for user objects found.
		// NULL returns all set properties.
		BOOL bIsVerbose   // TRUE indicates that all properties for the found objects are displayed.
		// FALSE indicates only the RDN.
		);
private:
	//std::wstring m_strManagerKey;
public:
	bool ReadAttribute(ADS_SEARCH_COLUMN & col);
public:
	bool SearchUserInfo(const wchar_t* wstrkeyWord=NULL);
};
