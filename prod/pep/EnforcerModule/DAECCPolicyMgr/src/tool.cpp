#include "tool.h"
#include <assert.h>
#include <locale>
#include <codecvt>
#include <string.h>
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <Shlobj.h>
#include <shlwapi.h>
#endif

#define BOOST_PP_STRINGIZE(text) BOOST_PP_STRINGIZE_I(text)
#define BOOST_PP_STRINGIZE_I(text) #text

const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

#pragma comment(lib, "shlwapi.lib.")

#ifdef _WIN32
HINSTANCE g_hDllHandleTool=NULL;
extern "C" BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Reserved);

    BOOLEAN bSuccess = TRUE;

    switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hDllHandleTool = hDllHandle;
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return bSuccess;
}
#endif

namespace CommonFun {

    std::string ToUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::string ToUTF8(const std::u16string &src, std::string &dst) {
        try
        {
           #if (defined _WIN32) && (_MSC_VER >= 1900)
             auto p = reinterpret_cast<const int16_t *>(src.data());
             dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
           #else
             dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(src);
           #endif
        }
        catch(const std::exception e)
        {
            dst.clear();
        }

        return dst;
    }

    std::string ToUTF8(const std::u16string& src) {
        try
        {
            std::string dst ;
            ToUTF8(src, dst);     
            return dst;
        }
        catch(const std::exception e)
        {
            return "";
        }
    }

    std::string ToUTF8(const std::u32string &src, std::string &dst) {
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = reinterpret_cast<const __int32 *>(src.data());
            dst = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.to_bytes(p, p + src.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(src);
            #endif
        }
        catch(const std::exception e)
        {
            dst.clear();
        }
        

        return dst;
    }

    std::string ToUTF8(const std::wstring &src, std::string &dst) {
        try
        {
            dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(src);
        }
        catch(const std::exception e)
        {
            dst.clear();
        }
        
        return dst; 
    }


    std::string FromUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::u16string FromUTF8(const std::string &src, std::u16string &dst) {
        dst = FromUTF8(src);
        return dst;
    }

    std::u16string FromUTF8(const std::string& src){
        std::u16string dst;
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = std::wstring_convert< std::codecvt_utf8<int16_t>, int16_t >{}.from_bytes(src);
            auto t = reinterpret_cast<const char16_t *>(p.data());
            dst = std::u16string(t, t + p.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(src);

                //        for (size_t i = 0; i < dst.length(); i++)
                //        {
                //            dst[i] = htobe16(dst[i]);
                //        }
        
            #endif
        }
        catch(const std::exception e)
        {
            dst.clear();
        }
        
        return dst;
    }

    std::u32string FromUTF8(const std::string &src, std::u32string &dst) {
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.from_bytes(src);
            auto t = reinterpret_cast<const char32_t *>(p.data());
            dst = std::u32string(t, t + p.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(src);
            #endif
        }
        catch(const std::exception e)
        {
            dst.clear();
        }
        
        return dst;
    }

    std::wstring FromUTF8(const std::string &src, std::wstring &dst) {
        try
        {
             dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(src);
        }
        catch(const std::exception e)
        {
            dst.clear();
        }

        return dst;
    }

    std::string UrlEncode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    int StrCaseCmp(const std::string& s1, const std::string& s2){
#ifndef _WIN32
        return strcasecmp(s1.c_str(), s2.c_str());
#else
        return stricmp(s1.c_str(), s2.c_str());
#endif
    }

    int StrCaseCmp(const std::u16string& s1, const std::u16string& s2) {
        std::string s11, s22;
        ToUTF8(s1, s11); ToUTF8(s2, s22);
#ifndef _WIN32
        return strcasecmp(s11.c_str(), s22.c_str());
#else
		return stricmp(s11.c_str(), s22.c_str());
#endif
    }

    int StrCaseCmp(const std::u16string& s1, const std::string& s2) {
        std::string s11;
        ToUTF8(s1, s11);
#ifndef _WIN32
        return strcasecmp(s11.c_str(), s2.c_str());
#else
		return stricmp(s11.c_str(), s2.c_str());
#endif
    }

    int StrCaseCmp(const char *s1, const char *s2) {
#ifndef _WIN32
        return strcasecmp(s1, s2);
#else
        return stricmp(s1, s2);
#endif
    }

#ifndef _WIN32
    unsigned long get_module_base(pid_t pid, const char* module_name)
    {
        FILE *fp = NULL;
        unsigned long addr = 0;
        char *pAddrRange = NULL;
        char filename[32] = {0};
        char line[1024] = {0};

        if (pid < 0)
        {
            snprintf(filename, sizeof(filename), "/proc/self/maps");
        }
        else
        {
            snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
        }
        fp = fopen(filename, "r");
        if (fp != NULL)
        {
            while (fgets(line, sizeof(line), fp))
            {
                if (strstr(line, module_name))
                {
                    pAddrRange = strtok(line, "-");
                    addr = strtoul(pAddrRange, NULL, 16);
#if defined(__x86_64__)
                    if (addr == 0x400000)
                    {
                        addr = 0;
                    }
#elif defined(__i386__)
                    if (addr == 0x08048000)
				{
					addr = 0;
				}
#endif
                    break;
                }
            }
            fclose(fp);
        }
        return addr;
    }
#endif

void split(const std::string& src, std::vector<std::string>& tokens, const std::string& delimiters)
{
    std::string::size_type lastPos = src.find_first_not_of(delimiters, 0);
    std::string::size_type pos = src.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos!=lastPos){
        tokens.push_back(src.substr(lastPos, pos-lastPos));
        lastPos = src.find_first_not_of(delimiters, pos);
        pos = src.find_first_of(delimiters, lastPos);
    }
}

uint64_t ConvertBCDNumber(unsigned char* pData, int nLen)
{
    unsigned char* pCurrent = pData + nLen - 1;
    int nRate = 1;
    uint64_t nValue = 0;
    do
    {
        uint64_t nValue1 = ((*pCurrent)&0x0F) * nRate;
        nRate *= 10;

        uint64_t nValue2 = (((*pCurrent)>>4)&0x0F) * nRate;
        nRate *= 10;

        nValue += nValue1 + nValue2;
        pCurrent--;

    } while (pCurrent>=pData);

    return nValue;
}

std::string  GetInstallPath()
{
#ifdef _WIN32
    char szPath[2*MAX_PATH]={0};
    DWORD dwLen = GetModuleFileNameA(g_hDllHandleTool, szPath, sizeof(szPath) );
    if(dwLen){
        char* pName = strrchr(szPath, '\\');
        if(pName){
            *pName='\0';  //remove the file name

            char* pBin = strrchr(szPath, '\\');
            if(pBin){
                *pBin= '\0'; //remove bin directory
            }
        }
    }
    return szPath;
#else
    return "/usr/nextlabs/DAE";
#endif 
}

std::string  GetProgramDataFolder()
{
    //get programdata folder
	std::string strDataFolder =  GetInstallPath() + "//data//"; // for enterprise environment, can't write data to c:/ driver
    if (!PathFileExistsA(strDataFolder.c_str())) {
        int nRes = SHCreateDirectoryExA(NULL, strDataFolder.c_str(), NULL);
        if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_ALREADY_EXISTS) && (nRes != ERROR_FILE_EXISTS))  {
            strDataFolder = "";
        }
    }

    return strDataFolder;
}

}// name space