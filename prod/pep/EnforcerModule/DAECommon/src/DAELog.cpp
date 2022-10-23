#include "DAELog.h"

#include "commfun.h"
#include "EMDBConfig.h"
#include <exception>
#include <stdio.h>
#include <stdlib.h>
//#include <boost/program_options.hpp>
//#include <boost/log/attributes/scoped_attribute.hpp>
#ifdef WIN32
#include <windows.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#endif 


#define MAX_MESSAGE_SIZE_CHARS 1024*10

namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", attrs::current_thread_id::value_type)

std::atomic<bool> CLog::m_initialized { false };

CLog::CLog()
{
  m_logLevel =   boost::log::trivial::warning;
}

std::string CLog::GetLogPath()
{
	return CommonFun::GetLogDirectoryPath();
}

bool CLog::InitLog()
{
	if (!m_initialized.load())
	{
		boost::log::add_console_log();
		m_initialized.store(true);
		return true;
	}
	return false;
}

bool CLog::InitLog(const std::string& directory_path, const std::string& file_name)
{
	if (!m_initialized.load())
	{
		boost::shared_ptr< boost::log::core > core = boost::log::core::get();

		core->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
		boost::log::add_common_attributes(); // Add TimeStamp, ThreadID so that we can use those attributes in Format.

		/* log formatter: [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message */
		auto fmtTimeStamp = boost::log::expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S.%f");
		//auto fmtThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
		auto fmtSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>(severity.get_name());

		boost::log::formatter logFmt = boost::log::expressions::format("%1%|%2%|%3%|%4%")
			% fmtTimeStamp %  fmtSeverity % thread_id % boost::log::expressions::smessage;
	

#if 1
		{
			typedef sinks::synchronous_sink < sinks::text_file_backend > sink_t;
			const auto strLogPath = directory_path + "/" + file_name + "_%Y-%m-%d_%H-%M-%S.%N.log";
			const auto& emdb_config = EMDBConfig::GetInstance();
			const auto log_max_size = static_cast<unsigned long long>(emdb_config.get_log_max_size());
			const auto log_max_files = static_cast<unsigned long long>(emdb_config.get_log_max_files());
			const auto log_min_free_space = static_cast<unsigned long long>(emdb_config.get_log_min_free_space());

			boost::shared_ptr< sinks::text_file_backend > backend =
				boost::make_shared< sinks::text_file_backend >(
					boost::log::keywords::file_name =  strLogPath,
					boost::log::keywords::rotation_size = 10 * 1024 * 1024,
					boost::log::keywords::open_mode = std::ios_base::app);
			boost::shared_ptr< sink_t > fsSink(new sink_t(backend));

			fsSink->locked_backend()->set_file_collector(sinks::file::make_collector(
					boost::log::keywords::target = directory_path,
					boost::log::keywords::max_size = log_max_size * 1024 * 1024,				// maximum total size of the stored files, in bytes
					boost::log::keywords::min_free_space = log_min_free_space * 1024 * 1024, 	// minimum free space on the drive, in bytes
					boost::log::keywords::max_files = log_max_files								// maximum number of stored files
			));

			// Upon restart, scan the directory for files matching the file_name pattern
			fsSink->locked_backend()->scan_for_files(sinks::file::scan_all);
			fsSink->set_formatter(logFmt);
			fsSink->locked_backend()->auto_flush(true);
		
			core->add_sink(fsSink);
		}
#endif

#if 0
		/* Windows debugger output backend OutputDebugString */
		typedef sinks::asynchronous_sink< sinks::debug_output_backend > debug_sink_t;
		// Create the sink. The backend requires synchronization in the frontend.
		boost::shared_ptr< debug_sink_t > dbgSink = boost::make_shared<debug_sink_t>();
		// Set the special filter to the frontend in order to skip the sink when no debugger is available
	//	dbgSink->set_filter(boost::log::expressions::is_debugger_present());
		dbgSink->set_formatter(logFmt);
		core->add_sink(dbgSink);
#endif 

		m_initialized.store(true);
		return true;
	}

	return false;
}


void CLog::WriteLogInternal(int nlevel, const char* szLog)
{

	if(nlevel>=m_logLevel)
	{
	 switch(nlevel)
	 {
		 case boost::log::trivial::trace:
		      BOOST_LOG_TRIVIAL(trace)<<szLog;
		      break;
		 case boost::log::trivial::debug:
		      BOOST_LOG_TRIVIAL(debug)<<szLog;
		       break;
		case boost::log::trivial::info:
		      BOOST_LOG_TRIVIAL(info)<<szLog;
		       break;
		case boost::log::trivial::warning:
		      BOOST_LOG_TRIVIAL(warning)<<szLog;
		       break;
		case boost::log::trivial::error:
		      BOOST_LOG_TRIVIAL(error)<<szLog;
		       break;
		 case boost::log::trivial::fatal:
		      BOOST_LOG_TRIVIAL(fatal)<<szLog;
		       break;
		default:
		 break;
	 }

	}
}


int CLog::WriteLogFunc(int nlevel,  const PrintFunc & func )
{
    if(nlevel >= m_logLevel)
    {
        if(func){
            std::string log = func();

            WriteLogInternal(nlevel, log.c_str());

            return 1;
        }
    }

    return 0;
}

void CLog::UpdateLogLevel(const emLogLevel levle)
{
	if (levle >= boost::log::trivial::trace && levle <= boost::log::trivial::fatal)
	{
		boost::log::core::get()->set_filter(boost::log::trivial::severity >= levle);
		m_logLevel = levle;
	}
	else
	{
		throw std::out_of_range("Log level can not be set into " + std::to_string(levle));
	}
}

void CLog::UpdateLogLevel(const std::string& strlevel)
{
	int nLevel = std::stoi(strlevel);
	UpdateLogLevel(static_cast<emLogLevel>(nLevel));
}

int CLog::VWriteLog(int nlevel,  const char* szFmt, va_list args)
{
	if (nlevel >= m_logLevel)
	{
		char szLog[MAX_MESSAGE_SIZE_CHARS] = { 0 };
		int nLog = vsnprintf(szLog, MAX_MESSAGE_SIZE_CHARS - 1, szFmt, args);
		WriteLogInternal(nlevel,  szLog);

		return nLog;
	}

	return 0;
}


int CLog::VWriteLog(int nlevel, const wchar_t* wszFmt, va_list args)
{
	if (nlevel >= m_logLevel)
	{
		wchar_t wszLog[MAX_MESSAGE_SIZE_CHARS] = { 0 };
		int nLog = vswprintf(wszLog, MAX_MESSAGE_SIZE_CHARS - 1, wszFmt, args);
		//convert from wchar to char
		std::string u8_cvt_str;
		CommonFun::ToUTF8(wszLog, u8_cvt_str);
		WriteLogInternal(nlevel, u8_cvt_str.c_str());
		return nLog;
	}

	return 0;
}


int CLog::VWriteLog2Console(int ctrl, int nlevel, const wchar_t* wszFmt, va_list args)
{
	wchar_t wszLog[MAX_MESSAGE_SIZE_CHARS] = { 0 };
	int nLog = vswprintf(wszLog, MAX_MESSAGE_SIZE_CHARS - 1, wszFmt, args);
	//convert from wchar to char
	std::string u8_cvt_str;
	CommonFun::ToUTF8(wszLog, u8_cvt_str);
	if (ctrl & LOG2FILE != 0) {
		WriteLogInternal(nlevel, u8_cvt_str.c_str());
	}
	if (ctrl & LOG2CONSOLE != 0) {
		
		 if(nlevel <= 1)
			printf("SUCCEED-%s\n", u8_cvt_str.c_str());
		else if(nlevel == 3)
			printf("WARNING-%s\n", u8_cvt_str.c_str());
		else if(nlevel > 3)
			printf("ERROR  -%s\n",u8_cvt_str.c_str());
	}

	return nLog;
}

int  CLog::VWriteLog2Console(int ctrl, int nlevel, const char* szFmt, va_list args) {

	char szLog[MAX_MESSAGE_SIZE_CHARS] = { 0 };
	int nLog = vsnprintf(szLog, MAX_MESSAGE_SIZE_CHARS - 1, szFmt, args);

	if ((ctrl & LOG2FILE) != 0) {
		WriteLogInternal(nlevel, szLog);
	}
	if ((ctrl & LOG2CONSOLE) != 0) {
		if (nlevel < 3)
			printf("SUCCEED-%s\n", szLog);
		else if (nlevel == 3)
			printf("WARNING-%s\n", szLog);
		else if (nlevel > 3)
			printf("ERROR  -%s\n", szLog);
	}

	return nLog;
}