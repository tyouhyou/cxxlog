/* *
 * A simple log utility for c++ projects (c++11 and later).
 * 
 * @author  tyouhyou    github.com/tyouhyou
 * */

#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#pragma region : includes

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <locale>

#pragma endregion

/**
 * _WLOGGER macro is for using with std::wstring / std::wofstream on windows.
 * Do not define this macro on linux, otherwise compile error may occur.
 * I do not think to use wchar on linux is a good idea, so left this issue unfixed.
 * */
#if defined(_WLOGGER)
#define __t_string std::wstring
#define __t_ofstream std::wofstream
#define __t_stringstream std::wstringstream
#define __t(str) L##str
#else
#define __t_string std::string
#define __t_ofstream std::ofstream
#define __t_stringstream std::stringstream
#define __t(str) str
#endif

#define __sd __t("[DEBUG]")
#define __si __t("[INFO ]")
#define __sw __t("[WARN ]")
#define __se __t("[ERROR]")

#pragma region : log leve defines.set LOG_LEVEL macro during compilation or before use log macros, if needed.

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

#pragma endregion

#pragma region : do not use macros define in the region.

#if defined(_MSC_VER)
#define __PATH_SEP_STR__ "\\"
#define __PATH_SEP_CHR__ '\\'
#else
#define __PATH_SEP_STR__ "/"
#define __PATH_SEP_CHR__ '/'
#endif

#define __FILENAME__ strrchr(__PATH_SEP_STR__ __FILE__, __PATH_SEP_CHR__) + 1
#define __CODE_INFO__ __t("[") << __FILENAME__ << __t("(") << __LINE__ << __t(")") << __t("::") << __func__ << __t("]")

#define _VL(lv, lg)     \
    if (LOG_LEVEL > lv) \
    {                   \
    }                   \
    else                \
        (lg) << __t(" ")
#define _VLC(lv, lg)    \
    if (LOG_LEVEL > lv) \
    {                   \
    }                   \
    else                \
        (lg) << __CODE_INFO__ << __t(" ")

#define _VF(f, lv, lvstr) \
    if (LOG_LEVEL > lv)   \
    {                     \
    }                     \
    else                  \
        (th_util::logger(f, lvstr)) << __t(" ")
#define _VFC(f, lv, lvstr) \
    if (LOG_LEVEL > lv)    \
    {                      \
    }                      \
    else                   \
        (th_util::logger(f, lvstr)) << __CODE_INFO__ << __t(" ")

#define _VE(lv, lvstr)  \
    if (LOG_LEVEL > lv) \
    {                   \
    }                   \
    else                \
        (th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()) << lvstr << __t(" ")
#define _VEC(lv, lvstr) \
    if (LOG_LEVEL > lv) \
    {                   \
    }                   \
    else                \
        (th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()) << lvstr << __CODE_INFO__ << __t(" ")

#pragma endregion

#pragma region : macros outputting logs.USE THE MACROS DEFINED IN THIS REGION ONLY.

#define SET_LOG_FILE(f) th_util::logger::set_g_log_file(f)

#define DL _VLC(_LOG_DEBUG, *th_util::logger::get_g_logger(nullptr, __sd))
#define IL _VL(_LOG_INFO, *th_util::logger::get_g_logger(nullptr, __si))
#define WL _VL(_LOG_WARN, *th_util::logger::get_g_logger(nullptr, __sw))
#define EL _VLC(_LOG_ERROR, *th_util::logger::get_g_logger(nullptr, __se))

#define DF(f) _VFC(f, _LOG_DEBUG, __sd)
#define IF(f) _VF(f, _LOG_INFO, __si)
#define WF(f) _VF(f, _LOG_WARN, __sw)
#define EF(f) _VFC(f, _LOG_ERROR, __se)

#define DE _VEC(_LOG_DEBUG, __sd)
#define IE _VE(_LOG_INFO, __si)
#define WE _VE(_LOG_WARN, __sw)
#define EE _VEC(_LOG_ERROR, __se)

#pragma endregion

#pragma region : definitions of classes backing the log output macros.do not use them directly.

namespace th_util
{
    class util
    {
    public:
        static __t_string get_cur_datetime()
        {
            auto dat = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm buf;
#if defined(_MSC_VER)
            localtime_s(&buf, &dat);
#else
            localtime_r(&dat, &buf);
#endif
            __t_stringstream ss;
            ss << std::put_time(&buf, __t("[%Y-%m-%d, %H:%M:%S]"));
            return ss.str();
        }
    };

    class logger
    {
    public:
        static void set_g_log_file(const __t_string &file)
        {
            if (file.empty())
                return;

            get_g_logger(&file);
        }

        static std::shared_ptr<logger> get_g_logger(const __t_string *file = nullptr, const __t_string &lvstr = __t(""))
        {
            static __t_string _g_log_file;
            static std::shared_ptr<std::mutex> _g_log_locker;

            if (nullptr != file && !file->empty())
            {
                _g_log_file = *file;
                _g_log_locker = std::make_shared<std::mutex>();

                std::shared_ptr<logger> l;
                return l;
            }

            auto lg = std::make_shared<logger>(_g_log_file, lvstr, _g_log_locker);
            return lg;
        }

        logger(const __t_string &logfile, const __t_string &loglevel)
            : log_file{logfile}, log_level{loglevel}, log_lock{}
        {
            std::locale::global(std::locale(""));
            // TODO ? check if folder exist. If not, empty string to log file path .
        }

        logger(const __t_string &logfile, const __t_string &loglevel, const std::shared_ptr<std::mutex> &locker)
            : logger(logfile, loglevel)
        {
            log_lock = {locker};
        }

        template <typename T>
        logger &operator<<(const T &s)
        {
            if (ss.bad())
                return *this;
            if (ss.fail())
            {
                ss.clear();
                return *this;
            }

            ss << s;
            return *this;
        }

        ~logger()
        {
            if (logger::log_file.empty())
                return;

            if (log_lock)
                log_lock->lock();
            try
            {
                __t_ofstream ofs;
                ofs.open(log_file, std::ofstream::out | std::ofstream::app | std::ios::binary);
                ofs.imbue(std::locale());
                ofs << util::get_cur_datetime()
                    << log_level
                    << ss.str()
                    << std::endl;
                ofs.close();
            }
            catch (...)
            {
                // DO NOTHING
            }
            if (log_lock)
                log_lock->unlock();
        }

    private:
        logger() = default;
        logger(const logger &) = delete;
        logger(const logger &&) = delete;
        logger &operator=(const logger &) = delete;
        logger &operator=(const logger &&) = delete;

        __t_string log_file;
        __t_string log_level;
        __t_stringstream ss;
        std::shared_ptr<std::mutex> log_lock;
    };

    class ender
    {
    public:
        ender(const std::ostream &os)
        {
            this->os = std::make_shared<std::ostream>(os.rdbuf());
        }
        ~ender()
        {
            *os << std::endl;
        }

    private:
        std::shared_ptr<std::ostream> os;
    };

} // namespace th_util

#pragma endregion

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
