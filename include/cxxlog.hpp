/* *
 * A simple log utility for c++ projects (c++11 or later).
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

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#define rchr wcsrchr
#define c_size 2
#else
typedef std::string tstring;
#define rchr strrchr
#define c_size 1
#endif

#define _LOG_ALL 0
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
#define _LOG_NONE 9

#ifndef LOG_LEVEL
#if defined(DEBUG) || defined(_DEBUG)
#define LOG_LEVEL _LOG_ALL
#else
#define LOG_LEVEL _LOG_INFO
#endif
#endif

#define __FILENAME__ (rchr("/" __FILE__, '/') + c_size)
#define __CODE_INFO "[" << __FILENAME__ << "(" << __func__ << ")(" << __LINE__ << ")]"
#define __TIME_INFO "[" << __DATE__ << "][" << __TIME__ << "]"

#define __LOG_W_CODEINFO__(s, lv) (th_util::ENDL(&s), s << lv << __TIME_INFO << __CODE_INFO << " - ")
#define __LOG_WO_CODEINFO__(s, lv) (th_util::ENDL(&s), s << lv << __TIME_INFO << " - ")

#define _D(_V)                  \
    if (LOG_LEVEL > _LOG_DEBUG) \
    {                           \
    }                           \
    else                        \
        __LOG_W_CODEINFO__(_V, "[DEBUG]")
#define _I(_V)                 \
    if (LOG_LEVEL > _LOG_INFO) \
    {                          \
    }                          \
    else                       \
        __LOG_WO_CODEINFO__(_V, "[INFO ]")
#define _W(_V)                 \
    if (LOG_LEVEL > _LOG_WARN) \
    {                          \
    }                          \
    else                       \
        __LOG_WO_CODEINFO__(_V, "[WARN ]")
#define _E(_V)                  \
    if (LOG_LEVEL > _LOG_ERROR) \
    {                           \
    }                           \
    else                        \
        __LOG_W_CODEINFO__(_V, "[ERROR]")

//// USE THE FOLLOWING MACROS ONLY ////

#define SET_LOGGER(f) th_util::Log::set_log_file(f)

#define V *th_util::Log::get_logger()
#define D _D(V)
#define I _I(V)
#define W _W(V)
#define E _E(V)

#define VE *th_util::Log::get(std::cerr)
#define DE _D(VE)
#define IE _I(VE)
#define WE _W(VE)
#define EE _E(VE)

#define VF(f) *th_util::Log::get(f)
#define DF(f) _D(VF(f))
#define IF(f) _I(VF(f))
#define WF(f) _W(VF(f))
#define EF(f) _E(VF(f))

///// The classes below are of backing the above log macros. Do not use them directly /////

namespace th_util
{
    class Log
    {

    public:
        static void set_log_file(const tstring &);
        static std::shared_ptr<Log> get_logger();
        static std::shared_ptr<Log> get(const tstring &);
        static std::shared_ptr<Log> get(const std::ostream &);

    public:
        ~Log();

        template <class T>
        Log &operator<<(const T &s)
        {
#ifdef _LOG_LOCK
            std::lock_guard<std::mutex> lock(_mtx);
#endif
#ifndef _NOT_CLOSE_LOG
            auto ofs = dynamic_cast<std::ofstream *>(os.get());
            if (ofs && !ofs->is_open() && !_file.empty())
            {
                ofs->open(_file, std::ofstream::out | std::ofstream::app);
            }
#endif
            *os << s;
            os->flush();

            return *this;
        }

        Log &operator<<(std::ostream &(*endl)(std::ostream &));

        Log(Log &&) = default;

    private:
        Log() = delete;
        Log(const Log &) = delete;
        Log &operator=(const Log &) = delete;
        Log &operator=(Log &&) = delete;

        explicit Log(const tstring &file_name);
        explicit Log(const std::ostream &os);

        tstring _file;
        std::shared_ptr<std::ostream> os;

#ifdef _LOG_LOCK
        std::mutex _mtx;
#endif
    };

    class ENDL
    {

    public:
        explicit ENDL(Log *log)
        {
            this->log = log;
        }

        ~ENDL()
        {
            *(this->log) << std::endl;
        }

    private:
        ENDL() = delete;
        Log *log;
    };

} // namespace th_util
