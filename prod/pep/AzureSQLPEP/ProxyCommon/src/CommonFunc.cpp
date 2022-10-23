#include "CommonFunc.h"
#include <windows.h>
#include <shellapi.h>
#include <Shlobj.h>
#include <vector>

namespace ProxyCommon
{

std::string WStringToString(const std::wstring& wstr)
{
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
	if (len <= 0)
		return result;

	char* buffer = new char[len + 1];
	if (buffer == nullptr)
		return result;
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, nullptr, nullptr);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;

	return result;
}

std::string UnicodeToUTF8(const std::wstring& wstr)
{
    std::string result;
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
    if (len <= 0)
        return result;

    char* buffer = new char[len + 1];
    if (buffer == nullptr)
        return result;
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer, len, nullptr, nullptr);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;

    return result;
}

std::wstring StringToWString(const std::string& str)
{
	std::wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), nullptr, 0);
	if (len <= 0)
		return result;

	wchar_t* buffer = new wchar_t[len + 1];
	if (buffer == nullptr)
		return result;
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;

	return result;
}

std::wstring UTF8ToUnicode(const std::string& str)
{
    std::wstring result;
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0);
    if (len <= 0)
        return result;

    wchar_t* buffer = new wchar_t[len + 1];
    if (buffer == nullptr)
        return result;
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;

    return result;
}

std::wstring GetFileFolder(const std::wstring& filePath)
{
	std::wstring strPath = filePath;
	std::wstring::size_type position = strPath.find_last_of(L'\\');
	if (position != std::wstring::npos)
	{
		strPath.erase(position);
	}
	return strPath;
}

std::wstring GetLocalTimeString()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);

    wchar_t szTime[256] = {0};
	wsprintfW(szTime, L"%d%02d%02d-%02d%02d%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	return szTime;
}

SYSTEMTIME LocalTimeStringToSYSTEMTIME(std::wstring str)
{
	SYSTEMTIME st = { 0 };
	int a = 0;
	swscanf(str.c_str(), L"%04d%02d%02d-%02d%02d%02d.txt", &st.wYear, &st.wMonth, &st.wDay, &st.wHour, &st.wMinute, &st.wSecond);
	return st;
}

double IntervalOfSYSTEMTIME(SYSTEMTIME start, SYSTEMTIME end)
{
	ULARGE_INTEGER fTime1;/*FILETIME*/
	ULARGE_INTEGER fTime2;/*FILETIME*/


	SystemTimeToFileTime(&start, (FILETIME*)&fTime1);
	SystemTimeToFileTime(&end, (FILETIME*)&fTime2);
	ULONGLONG dft = fTime2.QuadPart > fTime1.QuadPart ? fTime2.QuadPart - fTime1.QuadPart : fTime1.QuadPart - fTime2.QuadPart;
	return (double)dft / double(10000000);
}

std::wstring GetInstallFolder()
{
    WCHAR folder[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, folder, MAX_PATH);

    WCHAR* p = wcsrchr(folder, L'\\');
    if (p) *p = 0;

    return std::wstring(folder);
}

const std::wstring& GetWorkerNameW()
{
    static std::wstring _worker_name;

    if (!_worker_name.empty())
        return _worker_name;

    WCHAR* cmd = GetCommandLineW();
    if (cmd == NULL || !cmd[0])
        return L"";

    int _argc = 0;
    WCHAR** _argv = CommandLineToArgvW(cmd, &_argc);
    if (_argc <= 0 || _argv == NULL)
        return L"";

    for (int i = 0; i < _argc; i++)
    {
        if (_wcsicmp(_argv[i], L"-w") == 0) {
            i++;
            if (i < _argc && _argv[i]) {
                _worker_name = _argv[i];
            }
            break;
        }
    }

    return _worker_name;
}

const std::string& GetWorkerNameA()
{
    static std::string _worker_name;

    if (!_worker_name.empty())
        return _worker_name;

    _worker_name = WStringToString(GetWorkerNameW());
    return _worker_name;
}

const uint8_t* BinarySearch(const uint8_t* buff, uint32_t buff_len, const uint8_t* sub, uint32_t sub_len)
{
    if (nullptr == buff || buff_len == 0 || sub == nullptr || sub_len == 0)
        return nullptr;

    for (size_t i = 0; i <= buff_len - sub_len; i++)
    {
        if (memcmp(&buff[i], sub, sub_len) == 0)
            return &buff[i];
    }

    return nullptr;
}

static const std::vector<BYTE> s_vecHex = { '0','1','2','3','4', '5','6','7','8','9', 'A','B','C','D','E','F' };
std::string CharToHexView(_In_ const char kchIn, _In_ const bool kbBigEnd)
{
    int nLow = kchIn & 0xF;
    int nHigh = (kchIn & 0xF0) >> 4;
    if (kbBigEnd)
    {
        return std::string{ (char)s_vecHex[nHigh], (char)s_vecHex[nLow] };
    }
    else
    {
        return std::string{ (char)s_vecHex[nLow], (char)s_vecHex[nHigh] };
    }
}

char ConvertByteToViewChar(_In_ const BYTE kbyIn)
{
    if ((0x20 <= kbyIn) && (0x7E >= kbyIn))
    {
        return (char)kbyIn;
    }
    else
    {
        return '.';
    }
}

std::string BufferToUserView(_In_ const BYTE* kpbyDataIn, _In_ const int knDataLength)
{
    std::string strUserView = "";
    if ((nullptr != kpbyDataIn) && (0 < knDataLength))
    {
        for (int i = 0; i < knDataLength; ++i)
        {
            char chView = ConvertByteToViewChar(kpbyDataIn[i]);
            strUserView.push_back(chView);
        }
    }
    return strUserView;
}

std::string BufferToHexView(_In_ const BYTE* pbyDataIn, _In_ const uint32_t knDataLengthIn, _In_ const bool kbAppendUserView, _In_ const int knAlign) throw()
{
    std::string strBaseUserView;
    std::string strDataHexView = "";
    if (nullptr != pbyDataIn && 0 < knDataLengthIn)
    {
        for (int i = 1; i <= knDataLengthIn; ++i)
        {
            std::string strCurCHHexView = CharToHexView(pbyDataIn[i - 1], true);
            if (0 == i % knAlign)
            {
                if (kbAppendUserView)
                {
                    strCurCHHexView.push_back('\t');
                    strCurCHHexView.push_back('\t');
                    std::string strAlignItemUserView = BufferToUserView(pbyDataIn + i - knAlign, knAlign);
                    strBaseUserView.append(strAlignItemUserView);
                    strCurCHHexView.append(strAlignItemUserView);
                }
                strCurCHHexView.push_back('\n');
            }
            else
            {
                strCurCHHexView.push_back(' ');
            }
            strDataHexView.append(strCurCHHexView);
        }
        if ('\n' != strDataHexView[strDataHexView.length() - 1])
        {
            if (kbAppendUserView)
            {
                int nRemainder = knDataLengthIn % knAlign;
                for (int i = 0; i < (knAlign - nRemainder); ++i)
                {
                    strDataHexView.push_back(' ');
                    strDataHexView.push_back(' ');
                    strDataHexView.push_back(' ');
                }
                strDataHexView[strDataHexView.length() - 1] = '\t';
                strDataHexView.push_back('\t');
                std::string strAlignItemUserView = BufferToUserView(pbyDataIn + knDataLengthIn - nRemainder, nRemainder);
                strBaseUserView.append(strAlignItemUserView);
                strDataHexView.append(strAlignItemUserView);
            }
            strDataHexView.push_back('\n');
        }
    }

    return strDataHexView;
}

void PrintSocketData(const std::string& data)
{
    if (data.empty())
        return;

    std::string log_path = UnicodeToUTF8(GetInstallFolder());
    log_path.append("\\log\\");
    log_path.append(GetWorkerNameA());
    log_path.append("\\SocketData.log");

    FILE* p = NULL;
    fopen_s(&p, log_path.c_str(), "a");
    if (p)
    {
        fwrite(data.c_str(), data.length(), 1, p);
        fflush(p);
        fclose(p);
    }
}

}