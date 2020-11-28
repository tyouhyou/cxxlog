#include <locale>
#include <codecvt>
#include "cxxlog_ws.hpp"

using namespace th_util;

#if defined(_MSC_VER)
std::wstring wlogger::log_file;
#else
std::string wlogger::log_file;
#endif

std::mutex wlogger::mtx;

void wlogger::set_log_file(std::wstring logfile)
{
    std::lock_guard<std::mutex> lck(wlogger::mtx);
    std::locale::global(std::locale(""));
#if defined(_MSC_VER)
    wlogger::log_file = logfile;
#else
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(logfile);
    wlogger::log_file = s;
#endif
}

void wlogger::V(const std::wstring &log, const std::wstring &lv, const std::wstring &code_info)
{
    try
    {
        std::lock_guard<std::mutex> lck(wlogger::mtx);

        if (wlogger::log_file.empty())
            return;

        std::wofstream ofs;
        ofs.open(log_file, std::ofstream::out | std::ofstream::app | std::ios::binary);
        ofs.imbue(std::locale());
        ofs << get_cur_datetime()
            << lv
            << code_info
            << log
            << std::endl;
        ofs.close();
    }
    catch (...)
    {
        // DO NOTHING
    }
}

std::wstring wlogger::get_cur_datetime()
{
    auto d = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm buf;
#if defined(_MSC_VER)
    localtime_s(&buf, &d); // for windows
#else
    localtime_r(&d, &buf);
#endif
    std::wstringstream ss;
    ss << std::put_time(&buf, L"[%Y-%m-%d, %H:%M:%S]");
    return ss.str();
}