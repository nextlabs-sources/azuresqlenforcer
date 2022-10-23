#ifndef _LOGS_H_COMMONLIB_
#define _LOGS_H_COMMONLIB_

#include <stdio.h>
#include <string>
#include <atomic>

#pragma warning(push, 0)
#pragma warning(disable:4702 26812 6388 26451 26498 26495 6387 6258 6001 6255 6031 26439 5204 5219 5045 4355 4548 28251)

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp> 
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp> // boost::log::add_console_log
#include <boost/log/utility/setup/file.hpp> // boost::log::add_file_log
#include <boost/filesystem.hpp>
#include <boost/log/sinks/async_frontend.hpp>

#pragma warning(pop)

#define TEST_LOG 0   // 1 or 0
enum emLogLevel
{
    log_trace = boost::log::trivial::trace,
    log_debug = boost::log::trivial::debug,
    log_info = boost::log::trivial::info,
    log_warning = boost::log::trivial::warning,
    log_error = boost::log::trivial::error,
    log_fatal = boost::log::trivial::fatal
};
const int LOG2CONSOLE = 1;
const int LOG2FILE = 2;


typedef std::function<std::string ()> PrintFunc;

class CLog
{
private:
    static std::atomic<bool> m_initialized;
public:

    static CLog *Instance()
    {
        static CLog *inst = new CLog();
        return inst;
    }

    bool InitLog();
    bool InitLog(const std::string &directory_path, const std::string &file_name);

    int WriteLog(int lvl, const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int nLog = VWriteLog(lvl, fmt, args);
        va_end(args);

        return nLog;
    }
    int WriteLogWithTag(int lvl,  const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int nLog = VWriteLog(lvl,  fmt, args);
        va_end(args);

        return nLog;
    }
    int WriteLog(int lvl, const wchar_t* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int nLog = VWriteLog(lvl, fmt, args);
        va_end(args);

        return nLog;
    }

    int WriteLog(int ctrl, int lvl, const char* fmt, ...) {

        va_list args;
        va_start(args, fmt);
        int nLog = VWriteLog2Console(ctrl, lvl, fmt, args);
        va_end(args);

        return nLog;
        
    }

    int WriteLogFunc(int nlevel, const PrintFunc &func);

    void UpdateLogLevel(const emLogLevel levle);
    void UpdateLogLevel(const std::string &strlevel);

protected:
    CLog();
    CLog(const CLog &);

private:
    std::string GetLogPath();
    void WriteLogInternal(int lvl, const char *msg);

    int VWriteLog(int lvl,  const char* fmt, va_list args);
    int VWriteLog(int lvl,  const wchar_t *fmt, va_list args);

    int VWriteLog2Console(int ctrl, int lvl, const char* fmt, va_list args);
    int VWriteLog2Console(int ctrl, int lvl, const wchar_t* fmt, va_list args);
private:
    int m_logLevel;
};

#define theLog CLog::Instance()

#endif