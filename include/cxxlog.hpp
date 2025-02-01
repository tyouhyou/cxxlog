/* *
 * A simple log utility for c++ projects (c++11 and later).
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
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
#include <algorithm>
#include <thread>

#pragma endregion

#pragma region : Defines for using against wstring or string request.

/**
 * _WLOGGER macro is for using with std::wstring / std::wofstream on windows.
 * Do not define this macro on linux, otherwise compilation error may occur.
 * I do not think to use wchar on linux is a good idea, so left this issue unfixed.
 * */
#if defined(_WLOGGER)
#define __t(str) L##str
#define __t_char wchar_t
#define __t_string std::wstring
#define __t_ifstream std::wifstream
#define __t_ofstream std::wofstream
#define __t_stringstream std::wstringstream
#else
#define __t(str) str
#define __t_char char
#define __t_string std::string
#define __t_ifstream std::ifstream
#define __t_ofstream std::ofstream
#define __t_stringstream std::stringstream
#endif

#pragma endregion

#pragma region : Log level defines.Define LOG_LEVEL to _LOG_xxx value, if needed.

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

#pragma region : Macros definitions.Do not use them.

#if defined(_MSC_VER)
#define __PATH_SEP_STR__ "\\"
#define __PATH_SEP_CHR__ '\\'
#else
#define __PATH_SEP_STR__ "/"
#define __PATH_SEP_CHR__ '/'
#endif

#define __FILENAME__ strrchr(__PATH_SEP_STR__ __FILE__, __PATH_SEP_CHR__) + 1
#define __CODE_INFO__ __t("[") << __FILENAME__ << __t("(") << __LINE__ << __t(")") << __t("::") << __func__ << __t("]")

#define __V(lg, lv, leading) \
    if (LOG_LEVEL > lv)      \
    {                        \
    }                        \
    else                     \
        lg << std::boolalpha << leading << __t(" ")

#define __sd __t("[DEBUG]")
#define __si __t("[INFO ]")
#define __sw __t("[WARN ]")
#define __se __t("[ERROR]")

#pragma endregion

#pragma region : Logging macros.USE MACROS DEFINED IN THIS REGION ONLY.

#define SET_LOG_FILE(f) zb::Logger::set_g_log_file(f)
#define SET_LOG_MAX_SIZE(lz) zb::Logger::get_log_max_size(lz)

#define DL __V((*zb::Logger::get_g_logger()), _LOG_DEBUG, __sd << __CODE_INFO__)
#define IL __V((*zb::Logger::get_g_logger()), _LOG_INFO, __si)
#define WL __V((*zb::Logger::get_g_logger()), _LOG_WARN, __sw)
#define EL __V((*zb::Logger::get_g_logger()), _LOG_ERROR, __se << __CODE_INFO__)

#define DF(f) __V((zb::Logger(f)), _LOG_DEBUG, __sd << __CODE_INFO__)
#define IF(f) __V((zb::Logger(f)), _LOG_INFO, __si)
#define WF(f) __V((zb::Logger(f)), _LOG_WARN, __sw)
#define EF(f) __V((zb::Logger(f)), _LOG_ERROR, __se << __CODE_INFO__)

#define DE __V((zb::ender(std::cerr), std::cerr << zb::Logger::get_cur_datetime() << "[TID:" << std::this_thread::get_id() << "]"), _LOG_DEBUG, __sd << __CODE_INFO__)
#define IE __V((zb::ender(std::cerr), std::cerr << zb::Logger::get_cur_datetime() << "[TID:" << std::this_thread::get_id() << "]"), _LOG_INFO, __si)
#define WE __V((zb::ender(std::cerr), std::cerr << zb::Logger::get_cur_datetime() << "[TID:" << std::this_thread::get_id() << "]"), _LOG_WARN, __sw)
#define EE __V((zb::ender(std::cerr), std::cerr << zb::Logger::get_cur_datetime() << "[TID:" << std::this_thread::get_id() << "]"), _LOG_ERROR, __se << __CODE_INFO__)

#pragma endregion

#pragma region : Definitions of classes backing the log output macros.Do not use them directly.

namespace zb
{
    class Logger
    {
    public:
#pragma region : static methods
        static void set_g_log_file(const __t_string &file)
        {
            if (file.empty())
                return;

            get_g_logger(&file);
        }

        static unsigned int get_log_max_size(const unsigned int &max_size = 0)
        {
            static unsigned int _g_log_file_size = 0;
            if (max_size > 0)
            {
                _g_log_file_size = max_size;
            }
            return _g_log_file_size;
        }

        static std::shared_ptr<Logger> get_g_logger(const __t_string *file = nullptr)
        {
            static __t_string _g_log_file = "log.txt";
            static std::shared_ptr<std::mutex> _g_log_locker = std::make_shared<std::mutex>();

            std::lock_guard<std::mutex> lk(*_g_log_locker);
            if (nullptr != file && !file->empty())
            {
                _g_log_file = *file;
                return (std::shared_ptr<Logger>)nullptr;
            }
            auto lg = std::make_shared<Logger>(_g_log_file, _g_log_locker);
            return lg;
        }

        static __t_string get_cur_datetime()
        {
            auto now = std::chrono::system_clock::now();
            auto dat = std::chrono::system_clock::to_time_t(now);
            std::tm buf;
#if defined(_MSC_VER)
            localtime_s(&buf, &dat);
#else
            localtime_r(&dat, &buf);
#endif
            auto ms = (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()) % 1000;
            __t_stringstream ss;
            ss << std::put_time(&buf, __t("[%Y-%m-%d, %H:%M:%S")) << "." << std::setfill('0') << std::setw(3) << ms << "]";
            return ss.str();
        }

#pragma endregion

        Logger(const __t_string &logfile, const std::shared_ptr<std::mutex> &locker = nullptr, const bool& leading_info = true)
            : log_file{logfile}, log_lock{locker}, need_leading{leading_info}
        {
        }

        template <typename T>
        Logger &operator<<(const T &s)
        {
            if (ss.bad() || ss.fail())
            {
                ss.clear();
                return *this;
            }
            ss << s;
            return *this;
        }

        ~Logger()
        {
            if (log_file.empty())
                return;
            if (log_lock)
                std::lock_guard<std::mutex> lock(*log_lock);

#if defined(_WLOGGER)
            auto curloc = std::locale::global(std::locale(""));
#endif
            try
            {
                auto max_size = get_log_max_size();
                __t_stringstream iss;
                int replaceln = 0;
                if (max_size > 0)
                {
                    __t_ifstream ifs(log_file);
                    auto linesize = std::count(std::istreambuf_iterator<__t_char>(ifs),
                                               std::istreambuf_iterator<__t_char>(), '\n');
                    ifs.seekg(0);
                    __t_string ln;
                    replaceln = linesize - max_size + 1;
                    for (int i = 0; i < replaceln; i++)
                    {
                        std::getline(ifs, ln);
                    }
                    iss << ifs.rdbuf();
                    ifs.close();
                }

                __t_ofstream ofs;
                ofs.imbue(std::locale());
                if (replaceln > 0)
                {
                    ofs.open(log_file, std::ofstream::out | std::ofstream::trunc);
                    ofs << iss.rdbuf();
                }
                else
                {
                    ofs.open(log_file, std::ofstream::out | std::ofstream::app);
                }
                
                if (need_leading)
                ofs << get_cur_datetime()
                    << "[TID:" << std::this_thread::get_id() << "]";
                ofs << ss.str()
                    << std::endl;
                ofs.close();
            }
            catch (...)
            {
                // DO NOTHING
            }
#if defined(_WLOGGER)
            std::locale::global(curloc);
#endif
        }

    private:
        Logger() = delete;
        Logger(const Logger &) = delete;
        Logger(const Logger &&) = delete;
        Logger &operator=(const Logger &) = delete;
        Logger &operator=(const Logger &&) = delete;

        __t_string log_file;
        __t_stringstream ss;
        std::shared_ptr<std::mutex> log_lock;
        bool need_leading;
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
            *os << std::noboolalpha << std::endl;
        }

    private:
        std::shared_ptr<std::ostream> os;
    };

} // namespace zb

#pragma endregion

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif