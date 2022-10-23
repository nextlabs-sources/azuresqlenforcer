#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <windows.h>
#include <string>

namespace ProxyCommon
{
	std::string WStringToString(const std::wstring& wstr);
	std::wstring StringToWString(const std::string& str);
    std::string UnicodeToUTF8(const std::wstring& wstr);
    std::wstring UTF8ToUnicode(const std::string& str);
	std::wstring GetFileFolder(const std::wstring& filePath);
    std::wstring GetInstallFolder();
	std::wstring GetLocalTimeString();
	SYSTEMTIME LocalTimeStringToSYSTEMTIME(std::wstring);
	double IntervalOfSYSTEMTIME(SYSTEMTIME start, SYSTEMTIME end);
    const std::wstring& GetWorkerNameW();
    const std::string& GetWorkerNameA();
    const uint8_t* BinarySearch(const uint8_t* buff, uint32_t buff_len, const uint8_t* sub, uint32_t sub_len);

    std::string BufferToHexView(_In_ const BYTE* pbyDataIn, _In_ const uint32_t knDataLengthIn, _In_ const bool kbAppendUserView, _In_ const int knAlign) throw();
    void PrintSocketData(const std::string& data);
};

#endif // !COMMON_FUNCTION_H

