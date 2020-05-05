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

#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

// output message to file
#define V(f) *th_util_log::Log::get_logger(f)
#define VL(f) (th_util_log::ENDL(f), V(f))
#define D(f) (th_util_log::ENDL(f), V(f) << "[DEBUG][" << __FILENAME__ << "][" << __LINE__ << "][" << __func__ << "][" << __DATE__ << "][" << __TIME__ << "] - ")
#define I(f) (th_util_log::ENDL(f), V(f) << "[INFO ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define W(f) (th_util_log::ENDL(f), V(f) << "[WARN ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define E(f) (th_util_log::ENDL(f), V(f) << "[ERROR][" << __DATE__ << "][" << __TIME__ << "] - ")

// output message to wcerr
#define VERB std::cerr
#define VERBL (th_util_log::ENDL(), VERB)
#define DEBUGL (th_util_log::ENDL(), VERB << "[DEBUG][" << __FILENAME__ << "][" << __LINE__ << "][" << __func__ << "][" << __DATE__ << "][" << __TIME__ << "] - ")
#define INFOL (th_util_log::ENDL(), VERB << "[INFO ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define WARNL (th_util_log::ENDL(), VERB << "[WARN ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define ERRORL (th_util_log::ENDL(), VERB << "[ERROR][" << __DATE__ << "][" << __TIME__ << "] - ")


///// The objects backing the log macros. Do not use them directly /////

#define __LOG_LOCK_GET__

namespace th_util_log
{

class Log;
class Logendl{};
static constexpr Logendl __lendl__;
#ifdef __LOG_LOCK_GET__
static std::mutex __get_logger_mtx__;
#endif
static std::vector<std::pair<std::string, std::shared_ptr<Log>>> __loggers__;

class Log
{

public:
    static std::shared_ptr<Log> get_logger(const std::string &file_name, bool create=true)
    {
#ifdef __LOG_LOCK_GET__
        std::lock_guard<std::mutex> lock(__get_logger_mtx__);
#endif

        std::pair<std::string, std::shared_ptr<Log>> mkv;
        if (std::any_of(th_util_log::__loggers__.begin(), th_util_log::__loggers__.end(), [file_name, &mkv](std::pair<std::string, std::shared_ptr<Log>> &kv) {
                mkv = kv;
                return kv.first.compare(file_name) == 0;
            }))
        {
            return mkv.second;
        }

        if (create)
        {
            std::shared_ptr<Log> p { new Log(file_name) };
            mkv = {file_name, p};
            th_util_log::__loggers__.push_back(mkv);
        }

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
        ofs.open(file_name, std::ofstream::out | std::ofstream::app);
        ofs.setf(std::ios_base::boolalpha);
    }

    std::ofstream ofs;
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
        if (file_name.empty())
        {
            VERB << std::endl;
            return;
        }
        V(file_name) << __lendl__;
    }

private:
    std::string file_name;
};

} // namespace log