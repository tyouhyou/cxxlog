/* *
 * A simple log utility for c++ projects (c++11 or later).
 * 
 * Note: 
 *  1. The caller take the responsibility to check if the path of the log file is valid.
 *  2. The log file will be opened all along the process life.
 *  3. Although the outputting to log is NOT thread safe, creating/getting log stream is thread safe.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * */

#pragma once

#include <iostream>
#include <fstream>
#include <string.h>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <mutex>

// define via compile flag or uncomment the following lines

// #define _LOG_LOCK
// #define _LOG_FILE "test/log.txt"

#define _LOG_ALL 0
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
#define _LOG_NONE 9

#ifndef LOG_LEVEL
#define LOG_LEVEL _LOG_ALL
#endif

#define __CODE_INFO "[" << __FILENAME__ << "][" << __LINE__ << "][" << __func__ << "]"
#define __TIME_INFO "[" << __DATE__ << "][" << __TIME__ << "]"
#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#define __LOG_W_CODEINFO__(s, lv) (th_util_log::ENDL(), s << lv << __TIME_INFO << __CODE_INFO << " - ")
#define __LOG_WO_CODEINFO__(s, lv) (th_util_log::ENDL(), s << lv << __TIME_INFO << " - ")

#define __LOGF_W_CODEINFO__(f, s, lv) (th_util_log::ENDL(f), s << lv << __TIME_INFO << __CODE_INFO << " - ")
#define __LOGF_WO_CODEINFO__(f, s, lv) (th_util_log::ENDL(f), s << lv << __TIME_INFO << " - ")

// output message to file specified via argument
#define VF(f) *th_util_log::Log::get_logger(f)
#define VFL(f) (th_util_log::ENDL(f), VF(f))

#define DF(f) if(LOG_LEVEL > _LOG_DEBUG) {} else __LOGF_W_CODEINFO__(f, VF(f), "[DEBUG]")
#define IF(f) if(LOG_LEVEL > _LOG_INFO) {} else __LOGF_WO_CODEINFO__(f, VF(f), "[INFO ]")
#define WF(f) if(LOG_LEVEL > _LOG_WARN) {} else __LOGF_WO_CODEINFO__(f, VF(f), "[WARN ]")
#define EF(f) if(LOG_LEVEL > _LOG_ERROR) {} else __LOGF_W_CODEINFO__(f, VF(f), "[ERROR]")

// output message to cerr
#define VE std::cerr
#define VEL (th_util_log::ENDL(), VE)

#define DE if(LOG_LEVEL > _LOG_DEBUG) {} else __LOG_W_CODEINFO__(VE, "[DEBUG]")
#define IE if(LOG_LEVEL > _LOG_INFO) {} else __LOG_WO_CODEINFO__(VE, "[INFO ]")
#define WE if(LOG_LEVEL > _LOG_WARN) {} else __LOG_WO_CODEINFO__(VE, "[WARN ]")
#define EE if(LOG_LEVEL > _LOG_ERROR) {} else __LOG_W_CODEINFO__(VE, "[ERROR]")

// output to _LOG_FILE
#ifdef _LOG_FILE
#define V VF(_LOG_FILE)
#define VL VFL(_LOG_FILE)
#define D DF(_LOG_FILE)
#define I IF(_LOG_FILE)
#define W WF(_LOG_FILE)
#define E EF(_LOG_FILE)
#else
#define V VE
#define VL VEL
#define D DE
#define I IE
#define W WE
#define E EE
#endif

///// The objects backing the above log macros. Do not use them directly /////

namespace th_util_log
{

#ifdef _LOG_LOCK
static std::mutex __get_logger_mtx__;
#endif

class Log;
class Logendl{};

static constexpr Logendl __lendl__;
static std::vector<std::pair<std::string, std::shared_ptr<Log>>> __loggers__;

class Log
{

public:
    static std::shared_ptr<Log> get_logger(const std::string &file_name)
    {
#ifdef _LOG_LOCK
        std::lock_guard<std::mutex> lock(__get_logger_mtx__);
#endif

        std::pair<std::string, std::shared_ptr<Log>> mkv;
        if (std::any_of(th_util_log::__loggers__.begin(), 
						th_util_log::__loggers__.end(), 
						[file_name, &mkv](std::pair<std::string, std::shared_ptr<Log>> &kv) {
							mkv = kv;
							return kv.first.compare(file_name) == 0;
						}))
        {
            return mkv.second;
        }

		std::shared_ptr<Log> p { new Log(file_name) };
		mkv = {file_name, p};
		th_util_log::__loggers__.push_back(mkv);

        return mkv.second;
    }

public:
    ~Log()
    {
        ofs.flush();
        ofs.close();
    }

    template <class T>
    Log &operator<<(const T &s)
    {
        ofs << s;
        return *this;
    }

    Log &operator<<(const Logendl &e)
    {
        ofs << std::endl;
        return *this;
    }

private:
    Log() = delete;
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log &operator=(const Log&) = delete;
    Log &operator=(const Log&&) = delete;
    
    explicit Log(const std::string &file_name)
    {
#ifdef _LOG_LOCK
        std::lock_guard<std::mutex> lock(mtx);
#endif
        ofs.open(file_name, std::ofstream::out | std::ofstream::app);
        ofs.setf(std::ios_base::boolalpha);
    }

    std::ofstream ofs;
#ifdef _LOG_LOCK
    std::mutex mtx;
#endif
};

class ENDL
{
public:
    ENDL() = default;

    explicit ENDL(const std::string &file_name)
    {
        this->file_name = file_name;
    }

    ~ENDL()
    {
        if (file_name.empty() && typeid(VE) != typeid(Log))
        {
            VE << std::endl;
            return;
        }
        VF(file_name) << __lendl__;
    }

private:
    std::string file_name;
    
};

} // namespace log