#include "commfun.h"
#include "EMDBConfig.h"
#include <assert.h>
#include <locale>
#include <codecvt>
#ifdef WIN32
#include <windows.h>
#include <Shlobj.h>
#include <shellapi.h>
#else
#include <dlfcn.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif
#include <regex>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "DAEEncryptTool.h"

void* CommonFun::LoadShareLibrary(const char* szPath)
{
#ifdef WIN32
    return LoadLibraryA(szPath);
#else
    return dlopen(szPath, RTLD_NOW|RTLD_GLOBAL);
#endif
}

void*  CommonFun::GetProcAddress(void* libhandle, const char* funname)
{
#ifdef WIN32
    return ::GetProcAddress((HMODULE)libhandle, funname);
#else
    return dlsym(libhandle, funname);
#endif
}


std::string getmodulefullpath(_In_opt_ HMODULE hModuleIn)
{
    static const DWORD dwSize = 4096;
    char szFileFullPath[dwSize] = { 0 };
    ::GetModuleFileNameA(hModuleIn, szFileFullPath, dwSize - 1);
    return szFileFullPath;
}
std::string getexefullpath()
{
    static std::string s_strExeFullPath = getmodulefullpath(NULL);
    return s_strExeFullPath;
}

template <typename tyStr>
tyStr get_file_folder(_In_ const tyStr& wstrFilePath, _In_ const bool kbIncludeBackSlash)
{
    static const tyStr strBackSlash({ '\\' });
    tyStr wstrRet;
    typename tyStr::size_type nPos = wstrFilePath.rfind(strBackSlash);
    if (nPos != tyStr::npos)
    {
        if (kbIncludeBackSlash)
        {
            wstrRet = wstrFilePath.substr(0, nPos + 1);
        }
        else
        {
            wstrRet = wstrFilePath.substr(0, nPos);
        }
    }
    return wstrRet;
}

std::string CommonFun::ShareLibraryError() {
#ifdef WIN32
    char c[64] = { 0 };
    sprintf_s(c, "0X%x", GetLastError());
    return c;
#else
    char *r = dlerror();
    if (r == nullptr)
        return "NULL";
    else
        return r;
#endif
}

void CommonFun::CloseShareLibrary(void *s) {
#ifdef WIN32
    //FreeLibrary(s);
    FreeLibrary((HMODULE)s);
#else
    dlclose(s);
#endif
}

std::string CommonFun::GetQueryCloudAZSDKCppModule() {
    const std::string strInstallPath = EMDBConfig::GetInstance().get_global_install_path();
#ifdef WIN32
#ifdef _WIN64
    //std::string strPath = strInstallPath + "\\Common\\bin64\\QueryCloudAZSDKCpp.dll";
    std::string strPath ="QueryCloudAZSDKCpp.dll";
#else
    std::string strPath = strInstallPath + "\\Common\\bin32\\QueryCloudAZSDKCpp.dll";
#endif
#else
    std::string strPath = strInstallPath + "/Common/bin64/libQueryCloudAZSDKCpp.so";
#endif
    return strPath;
}

std::string CommonFun::GetEnforcerModule() {
    const std::string strInstallPath = EMDBConfig::GetInstance().get_global_install_path();
#ifdef WIN32
#ifdef _WIN64
    //std::string EnforcerPath = strInstallPath + "\\Common\\bin64\\sqlenforcer.dll";
    std::string EnforcerPath = "sqlenforcer.dll";
#else
    std::string EnforcerPath = strInstallPath + "\\Common\\bin32\\sqlenforcer.dll";
#endif
#else
    std::string EnforcerPath = strInstallPath + "/Common/bin64/libsqlenforcer.so";
#endif
    return EnforcerPath;
}

std::string CommonFun::GetSqlParserModule() {
    //get sqlparser component path
    const std::string strInstallPath = EMDBConfig::GetInstance().get_global_install_path();
#ifdef WIN32
#ifdef _WIN64
    //std::string strParserPath = strInstallPath + "\\Common\\bin64\\sql2003.dll";
    std::string strParserPath = "sql2003.dll";
#else
    std::string strParserPath = strInstallPath + "\\Common\\bin32\\sql2003.dll";
#endif
#else
    std::string strParserPath = strInstallPath + "/Common/bin64/libsql2003.so";
#endif
    return strParserPath;
}

std::string CommonFun::GetSSLModule(const std::string& module_name) {
#ifdef WIN32
#ifdef _WIN64
    //std::string module_path = EMDBConfig::GetInstance().get_global_install_path() + "\\Common\\bin64\\" + module_name.c_str() + ".dll";
    std::string module_path = module_name + ".dll";
#else
    std::string module_path = EMDBConfig::GetInstance().get_global_install_path() + "\\Common\\bin32\\" + module_name.c_str() + ".dll";
#endif
#else
    std::string module_path = "libssl.so";
#endif
    return module_path;
}

std::string CommonFun::GetODBCOriModule() {
#ifdef WIN32
    return "odbc32_ms.dll";
#else
    //return "libodbc.so";
    //Debian-based Linux distributions, like Ubuntu
    //return "/usr/lib/x86_64-linux-gnu/libodbc_ms.so";
    //Fedora-based Linux distributions, like Redhat, Centos, Oracle Linux
    //return "/usr/lib64/libodbc_ms.so";
    
    //test in centos, OS can find the file directly by its filename.
    return "libodbc_ms.so";
#endif
}

std::string CommonFun::GetConfigFilePath() {
#ifdef WIN32
    return CommonFun::GetProgramDataFolder() + "\\Config\\config.ini";
#else
    return CommonFun::GetProgramDataFolder() + "/config/config.ini";
#endif
}

std::string CommonFun::GetLogDirectoryPath() {
#ifdef WIN32
    std::string strLogFile = CommonFun::GetProgramDataFolder() + "/log";
    CreateDirectoryA(strLogFile.c_str(), NULL);
#else
    struct passwd *pwd = getpwuid(getuid());
    assert(pwd != nullptr);
    std::string strLogFile = pwd->pw_dir;
    strLogFile += "/NextLabs";
    mkdir(strLogFile.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    strLogFile += "/DAE/Oracle";
    mkdir(strLogFile.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    strLogFile += "/";
    return strLogFile;
}


std::string CommonFun::GetProgramDataFolder()
{
    std::string strFolder;
#ifdef WIN32
    //create sub folder
    static std::string s_strExeFullPath = getexefullpath();
    static std::string s_strExeStandardFolder = get_file_folder(s_strExeFullPath, true);;

    //int nRes = SHCreateDirectoryExA(NULL, s_strExeStandardFolder.c_str(), NULL);
    //if ((nRes != ERROR_SUCCESS) &&
    //    (nRes != ERROR_ALREADY_EXISTS) &&
    //    (nRes != ERROR_FILE_EXISTS)) {
    //    s_strExeStandardFolder = "";
    //}
    return s_strExeStandardFolder;

#else
    strFolder = "/usr/nextlabs";
    mkdir(strFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    strFolder += "/emdb";
    mkdir(strFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	return strFolder;
}
/*
std::wstring CommonFun::ConvertUtf8ToUtf16(const char* szUtf8)
{
	  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
    std::wstring u16_cvt_str = cvt.from_bytes(szUtf8); // utf-16 from utf-8
		return u16_cvt_str;
}

std::string CommonFun::ConvertUtf16ToUtf8(const wchar_t* szUtf16)
{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
    std::string u8_cvt_str = cvt.to_bytes(szUtf16); // utf-16 to utf-8
		return u8_cvt_str;
}
*/
 
std::string CommonFun::UrlEncode(const std::string &value) {
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

int CommonFun::StrCaseCmp(const char *s1, const char *s2) {
    #ifndef WIN32
    return strcasecmp(s1, s2);
    #else
    return _stricmp(s1, s2);
    #endif
}

bool CommonFun::CaseInsensitiveEquals(const std::string& s1, const std::string& s2)
{
    return StrCaseCmp(s1.c_str(), s2.c_str()) == 0;
}

bool CommonFun::CopyIntToA(int i, char *buf, size_t size, int& result) {
    if (!buf || size <= 0) {
        return false;
    }
    std::string i_str = std::to_string(i);
    return CopyStringToA(i_str, buf, size, result);
}

bool CommonFun::CopyIntToW(int i, wchar_t *buf, size_t size, int& result) {
    if (!buf || size <= 0) {
        return false;
    }
    std::wstring i_str = std::to_wstring(i);
    return CopyWStringToW(i_str, buf, size, result);
}

bool CommonFun::CopyStringToA(const std::string& src, char *buf, size_t size, int& result) {
    if (!buf || size <= 0) {
        return false;
    }
    std::string i_str = src;
    bool ret = true;
    size_t len = i_str.length();
    if (len > size - 1) {
        len = size - 1;
        ret = false;
    }
    strncpy(buf, i_str.c_str(), len);
    buf[len] = '\0';
    result = (int)len;
    return ret;
}

bool CommonFun::CopyWStringToW(const std::wstring& src, wchar_t *buf, size_t size, int& result) {
  if (!buf || size <= 0) {
    return false;
  }
  std::wstring i_str = src;
  bool ret = true;
  size_t len = i_str.length();
  if (len > size - 1) {
    len = size - 1;
    ret = false;
  }
  wcsncpy(buf, i_str.c_str(), len);
  buf[len] = L'\0';
  result = (int)len;
  return ret;
}

std::string CommonFun::ToUTF8(const std::string& src, std::string& dst) {
      dst = src;
      return dst;
}
std::string CommonFun::ToUTF8(const std::u16string& src, std::string& dst) {
    try {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = reinterpret_cast<const int16_t *>(src.data());
        dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
#else
        dst = std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(src);
#endif
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
      return dst;
}
std::string CommonFun::ToUTF8(const std::u32string& src, std::string& dst) {
    try {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = reinterpret_cast<const __int32 *>(src.data());
        dst = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.to_bytes(p, p + src.size());
#else
        dst = std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(src);
#endif
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
    return dst;
}
std::string CommonFun::ToUTF8(const std::wstring& src, std::string& dst) {
    try
    {
        dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(src);
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
    return dst;
}


std::string CommonFun::FromUTF8(const std::string& src, std::string& dst) {
      dst = src;
      return dst;
}
std::u16string CommonFun::FromUTF8(const std::string& src, std::u16string& dst) {
    try{
#if (defined WIN32) && (_MSC_VER >= 1900)
      auto p = std::wstring_convert< std::codecvt_utf8<int16_t>, int16_t >{}.from_bytes(src);
      auto t = reinterpret_cast<const char16_t *>(p.data());
      dst = std::u16string(t, t + p.size());
#else
      dst = std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(src);
#endif
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
    return dst;
}
std::u32string CommonFun::FromUTF8(const std::string& src, std::u32string& dst) {
    try{
#if (defined WIN32) && (_MSC_VER >= 1900)
      auto p = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.from_bytes(src);
      auto t = reinterpret_cast<const char32_t *>(p.data());
      dst = std::u32string(t, t + p.size());
#else
        dst = std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(src);
#endif
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
    return dst;
}
std::wstring CommonFun::FromUTF8(const std::string& src, std::wstring& dst) {
    try {
        dst = std::wstring_convert< std::codecvt_utf8<wchar_t >, wchar_t >{}.from_bytes(src);
    }
    catch(const std::exception& e)
    {
        printf("error wchar:%s\n",e.what());
        dst.clear();
    }
    return dst;
}

void CommonFun::RemoveUTF8BOM(std::string& s) noexcept {
    if ((unsigned char)s[0] == 0xEF
        && (unsigned char)s[1] == 0xBB
        && (unsigned char)s[2] == 0xBF)
    {
        s.erase(0, 3);
    }
}

std::string CommonFun::ReadFstream(std::ifstream& fstream) {
    std::stringstream ss;
    ss << fstream.rdbuf();

    return ss.str();
}

#include "EMDBType.h"
EMDB_DB_TYPE get_dbtype_by_drivername(const std::string &strDriverName)
{
    EMDB_DB_TYPE dbType = EMDB_DB_UNKNOW;
    if (strDriverName.find("SQL Server") != std::string::npos)
    {
        dbType = EMDB_DB_SQLSERVER;
    }
    else if (strDriverName.find("MySQL") != std::string::npos)
    {
        dbType = EMDB_DB_MYSQL;
    }
    else if (strDriverName.find("Oracle") != std::string::npos)
    {
        dbType = EMDB_DB_ORACLE;
    }
    else if (strDriverName.find("HDB") != std::string::npos)
    {
        dbType = EMDB_DB_HANA;
    }
    else if (strDriverName.find("BigQuery") != std::string::npos)
    {
        dbType = EMDB_DB_BIGQUERY;
    }
    else if (strDriverName.find("Redshift") != std::string::npos)
    {
        dbType = EMDB_DB_REDSHIFT;
    } else if(strDriverName.find("DB2") != std::string::npos ||
        strDriverName.find("Db2") != std::string::npos ||
        strDriverName.find("IBM") != std::string::npos )
    {
        dbType = EMDB_DB_DB2;
    }


    return dbType;
}

void CommonFun::SplitString(const std::string &src, const std::string & delims, std::vector<std::string> & output){
    //std::vector<std::string> output;
    auto first = std::cbegin(src);

    while (first != std::cend(src))
    {
        const auto second = std::find_first_of(first, std::cend(src),
                                               std::cbegin(delims), std::cend(delims));

        if (first != second)
            output.emplace_back(first, second);

        if (second == std::cend(src))
            break;

        first = std::next(second);
    }

    return ;
}

bool CommonFun::IsValidHttpUrl(const std::string &text) {
    const std::regex urlReg("https?://(www.)?[-a-zA-Z0-9@:%._+~#=]{2,256}.[a-z]{2,4}([-a-zA-Z0-9@:%_+.~#?&//=]*)");
    return std::regex_match(text, urlReg);
}

bool CommonFun::IsValidPort(const std::string &text) {
    return IsPositiveInt(text);
}

bool CommonFun::IsValidOdbcConnectionString(const std::string &text) {
    std::regex odbc_connection_string_regex{R"d(([^=;]+=[^=;]+;)+)d"};
    return std::regex_match(text, odbc_connection_string_regex);
}

bool CommonFun::IsPositiveInt(const std::string &text) {
    const std::regex positiveIntReg("[0-9]+");
    return std::regex_match(text, positiveIntReg);
}

bool CommonFun::IsEmptyString(const std::string &text) {
    return text.empty();
}

bool CommonFun::IsNotEmptyString(const std::string& text) {
    return !IsEmptyString(text);
}

std::string  CommonFun::EncryptString(const std::string & src){
    AesEncryptor theAes;
    return theAes.EncryptString(src);
}
std::string CommonFun::DecryptString(const std::string & src){
    AesEncryptor theAes;
    return theAes.DecryptString(src);
}


std::string CommonFun::LTrim(const std::string& str)
{
    return str.substr(str.find_first_not_of(" \n\r\t"));
}

std::string CommonFun::RTrim(const std::string& str)
{
    return str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
}

std::string CommonFun::Trim(const std::string& str)
{
    return LTrim(RTrim(str));
}

bool  CommonFun::IsNum(const std::string& ss) {
    char* pEnd = nullptr;
    strtod(ss.c_str(), &pEnd);

    if (pEnd != nullptr && (pEnd - ss.c_str()) == ss.length())
        return true;
    else
        return false;
}

const std::string& CommonFun::GetWorkerName()
{
    static std::string _worker_name;

    if (!_worker_name.empty())
        return _worker_name;

#ifdef WIN32
    WCHAR* cmd = GetCommandLineW();
    if (cmd == NULL || !cmd[0])
        return "";

    int _argc = 0;
    WCHAR** _argv = CommandLineToArgvW(cmd, &_argc);
    if (_argc <= 0 || _argv == NULL)
        return "";

    for (int i = 0; i < _argc; i++)
    {
        if (_wcsicmp(_argv[i], L"-w") == 0) {
            i++;
            if (i < _argc && _argv[i]) {
                ToUTF8(_argv[i], _worker_name);
            }
            break;
        }
    }
#else

#endif

    return _worker_name;
}