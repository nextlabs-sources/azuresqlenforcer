#ifndef __ENFORCER_MODULE_DAE_BOOTSTRAP_LOGGER_CLASS_H__
#define __ENFORCER_MODULE_DAE_BOOTSTRAP_LOGGER_CLASS_H__

#include <string>
#include <iostream>

#include "util_funcs.h"

namespace daebootstrap {

class Logger {
public:
	enum class Level {
		kTrace,
		kDebug,
		kInfo,
		kWarning,
		kError,
		kFatal,
	};

	static Logger& Instance() {
		static Logger logger{};
		return logger;
	}

	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;
	Logger(Logger &&) = delete;
	Logger &operator=(Logger &&) = delete;

private:
	Logger() = default;

public:
	template <typename ... Args>
	void Trace(const std::string& msg, Args ... args) const noexcept {
		if (level_ <= Level::kTrace) {
			std::cout << utils::StringFormat(msg, args ...) << std::endl;
		}
	}
	
	template <typename ... Args>
	void Debug(const std::string& msg, Args ... args) const noexcept {
		if (level_ <= Level::kDebug) {
			std::cout << utils::StringFormat(msg, args ...) << std::endl;
		}
	}

	template <typename ... Args>
	void Info(const std::string &msg, Args ... args) const noexcept {
		if (level_ <= Level::kInfo) {
			std::cout << utils::StringFormat(msg, args ...) << std::endl;
		}
	}

	template <typename ... Args>
	void Warning(const std::string& msg, Args ... args) const noexcept {
		if (level_ <= Level::kWarning) {
			std::cout << utils::StringFormat(msg, args ...) << std::endl;
		}
	}
	
	template <typename ... Args>
	inline void Error(const std::string& msg, Args ... args) const noexcept {
		if (level_ <= Level::kError) {
            HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);

			std::cout << utils::StringFormat(msg, args ...) << std::endl;
            
            hdl = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hdl, 0x7);
		}
	}
	
	template <typename ... Args>
	void Fatal(const std::string& msg, Args ... args) const noexcept {
		if (level_ <= Level::kFatal) {
            HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);

			std::cout << utils::StringFormat(msg, args ...) << std::endl;

            hdl = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hdl, 0x7);
		}
	}

public:
    Level get_level() const noexcept {
        return level_;
    }

    void set_level(const Level level) {
        level_ = level;
    }

private:
	Level level_ = Level::kInfo;
};

} // namespace daebootstrap

#endif//__ENFORCER_MODULE_DAE_BOOTSTRAP_LOGGER_CLASS_H__