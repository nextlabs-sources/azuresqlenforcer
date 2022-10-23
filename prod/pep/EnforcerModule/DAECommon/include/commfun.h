#ifndef COMMON_FUN_H
#define COMMON_FUN_H
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <memory>

#ifdef WIN32
#else
#define sprintf_s snprintf 
#endif

#define LOAD_MODULE_FAILED "load %s failed, error is %s !\n"
#define IS_DIGIT(s) ('0'<=(s)&&(s)<='9')

typedef uint64_t USER_CONTEXT;

enum USER_CONTEXT_INFO_TYPE
{
    CONTEXT_INFO_SERVER,
    CONTEXT_INFO_DATABASE,
    CONTEXT_INFO_SCHEMA,
    CONTEXT_INFO_DB_TYPE,
    CONTEXT_INFO_CLIENT_APP,
    CONTEXT_INFO_CLIENT_HOST_NAME,
    CONTEXT_INFO_CLIENT_IP,
};

class CommonFun
{
    public:
    static void* LoadShareLibrary(const char* szPath);
    static void* GetProcAddress(void* libhandle, const char* funname);
    static std::string ShareLibraryError();
    static void CloseShareLibrary(void *s);
    static const std::string& GetWorkerName();

    /* module list */
    static std::string GetQueryCloudAZSDKCppModule();
    static std::string GetEnforcerModule();
    static std::string GetSqlParserModule();
    static std::string GetSSLModule(const std::string&);
    static std::string GetODBCOriModule();

    static std::string GetConfigFilePath();
    static std::string GetLogDirectoryPath();

    static std::string GetProgramDataFolder();

    static std::string  UrlEncode(const std::string &value);
    static int StrCaseCmp(const char *s1, const char *s2);  // Compare S1 and S2, ignoring case
    static bool CaseInsensitiveEquals(const std::string& s1, const std::string& s2);

    static bool CopyIntToA(int i, char *buf, size_t size, int& result);
    static bool CopyIntToW(int i, wchar_t *buf, size_t size, int& result);
    static bool CopyStringToA(const std::string& src, char *buf, size_t size, int& result);
    static bool CopyWStringToW(const std::wstring& src, wchar_t *buf, size_t size, int& result);

    static std::string ToUTF8(const std::string& src, std::string& dst);
    static std::string ToUTF8(const std::u16string& src, std::string& dst);
    static std::string ToUTF8(const std::u32string& src, std::string& dst);
    static std::string ToUTF8(const std::wstring& src, std::string& dst) ;

    static std::string FromUTF8(const std::string& src, std::string& dst);
    static std::u16string FromUTF8(const std::string& src, std::u16string& dst) ;
    static std::u32string FromUTF8(const std::string& src, std::u32string& dst);
    static std::wstring FromUTF8(const std::string& src, std::wstring& dst);

    static void RemoveUTF8BOM(std::string& s) noexcept;

    static std::string ReadFstream(std::ifstream& fstream);

    static void SplitString(const std::string &src, const std::string & delims, std::vector<std::string> & output);

    static bool IsValidHttpUrl(const std::string& text);
    static bool IsValidPort(const std::string& text);
    static bool IsValidOdbcConnectionString(const std::string& text);
    static bool IsPositiveInt(const std::string& text);
    static bool IsEmptyString(const std::string& text);
    static bool IsNotEmptyString(const std::string& text);

    static std::string EncryptString(const std::string & src);
    static std::string DecryptString(const std::string & src);

    static std::string LTrim(const std::string & str);
    static std::string RTrim(const std::string & str);
    static std::string Trim(const std::string & str);

    static bool IsNum(const std::string& c);

    template <typename ... Args>
    static std::string StringFormat(const std::string& format, Args... args) {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        if (size_s <= 0) {
            throw std::runtime_error("Error happened during formatting.");
        }
        auto size = static_cast<size_t>(size_s);
        auto buf = std::make_unique<char[]>(size);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};
#endif 