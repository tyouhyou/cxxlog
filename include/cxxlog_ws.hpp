#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstring>

#if defined(_MSC_VER)
#define __PATH_SEP_STR__ "\\"
#define __PATH_SEP_CHR__ '\\'
#else
#define __PATH_SEP_STR__ "/"
#define __PATH_SEP_CHR__ '/'
#endif

#define __FILENAME__ strrchr(__PATH_SEP_STR__ __FILE__, __PATH_SEP_CHR__) + 1
#define __CODE_INFO__ "[" << __FILENAME__ << "(" << __func__ << ")(" << __LINE__ << ")]"

#define SET_LOG_FILE(f) th_util::wlogger::set_log_file(f)

#if defined(DEBUG) || defined(_DEBUG)
#define LD(log)                        \
    {                                  \
        std::wstringstream ss;         \
        ss << __CODE_INFO__;           \
        wlogger::Debug(log, ss.str()); \
    }
#else
#define LD(log)
#endif
#define LI(log) wlogger::Info(log)
#define LW(log) wlogger::Warn(log)
#define LE(log)                        \
    {                                  \
        std::wstringstream ss;         \
        ss << __CODE_INFO__;           \
        wlogger::Error(log, ss.str()); \
    }

namespace th_util
{

    class wlogger
    {
    public:
        static void set_log_file(std::wstring logfile);

        static void Debug(const std::wstring &log, const std::wstring &code_info = L"")
        {
            V(log, L"[DEBUG]", code_info);
        }

        static void Info(const std::wstring &log)
        {
            V(log, L"[INFO ]", L"");
        }

        static void Warn(const std::wstring &log)
        {
            V(log, L"[WARN ]", L"");
        }

        static void Error(const std::wstring &log, const std::wstring &code_info = L"")
        {
            V(log, L"[Error]", code_info);
        }

    private:
#if defined(_MSC_VER)
        static std::wstring log_file;
#else
        static std::string log_file;
#endif
        static std::mutex mtx;
        static void V(const std::wstring &log, const std::wstring &lv, const std::wstring &code_info);
        static std::wstring get_cur_datetime();
    };

} // namespace th_util