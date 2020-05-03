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
#define V(f) *log::Log::get_logger(f)
#define VL(f) (log::ENDL(f), V(f))
#define D(f) (log::ENDL(f), V(f) << "[DEBUG][" << __FILENAME__ << "][" << __LINE__ << "][" << __func__ << "][" << __DATE__ << "][" << __TIME__ << "] - ")
#define I(f) (log::ENDL(f), V(f) << "[INFO ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define W(f) (log::ENDL(f), V(f) << "[WARN ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define E(f) (log::ENDL(f), V(f) << "[ERROR][" << __DATE__ << "][" << __TIME__ << "] - ")

// output message to wcerr
#if defined(UNICODE) || defined(_UNICODE)
#define VERB std::wcerr
#else
#define VERB std::cerr
#endif
#define VERBL (log::ENDL(), std::wcerr)
#define DEBUGL (log::ENDL(), VERB << "[DEBUG][" << __FILENAME__ << "][" << __LINE__ << "][" << __func__ << "][" << __DATE__ << "][" << __TIME__ << "] - ")
#define INFOL (log::ENDL(), VERB << "[INFO ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define WARNL (log::ENDL(), VERB << "[WARN ][" << __DATE__ << "][" << __TIME__ << "] - ")
#define ERRORL (log::ENDL(), VERB << "[ERROR][" << __DATE__ << "][" << __TIME__ << "] - ")

// TODO: performance-metrics, stopwatch, etc.


///// The objects backing the log macros. Do not use them directly /////

#define __LOG_LOCK_GET__

namespace log
{

class Log;
struct KV
{
    std::string file;
    std::shared_ptr<Log> logger;
};
class Logendl{};

static constexpr Logendl __lendl__;
static std::vector<KV> __loggers__;

#ifdef __LOG_LOCK_GET__
static std::mutex __get_logger_mtx__;
#endif

class Log
{

public:
    static std::shared_ptr<Log> get_logger(const std::string &file_name, bool create=true)
    {
#ifdef __LOG_LOCK_GET__
        std::lock_guard<std::mutex> lock(__get_logger_mtx__);
#endif

        KV mkv;
        if (std::any_of(log::__loggers__.begin(), log::__loggers__.end(), [file_name, &mkv](KV &kv) {
                mkv = kv;
                return kv.file.compare(file_name) == 0;
            }))
        {
            return mkv.logger;
        }

        if (create)
        {
            std::shared_ptr<Log> p { new Log(file_name) };
            mkv = {file_name, p};
            log::__loggers__.push_back(mkv);
        }

        return mkv.logger;
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