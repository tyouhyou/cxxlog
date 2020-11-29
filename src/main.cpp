#include "cxxlog.hpp"
#include "cxxlog_ws.hpp"
#include "stopwatch.hpp"

using namespace th_util;

int main()
{
    th_util::StopWatch sw;
    SET_LOGFILE("test/log.txt");

    sw.start();

    DL << "Here you are, BUG.";
    IL << "Let me inform you.";
    WL << "Alert, paradise is falling";
    EL << "OMG, it crashed.";

    IE << "The first 4 logs elasped: " << sw.perf() << " microseconds";

    DF("test/debug.log") << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "The second 4 logs elasped: " << sw.perf() << " microseconds";

    DF("test/debug2.log") << "This is debug message again.";
    IF("test/info2.log") << "Information for you again.";
    WF("test/warn2.log") << "I warn you again.";
    EF("test/error2.log") << "An error message goes here again.";

    IE << "The third 4 logs elasped: " << sw.perf() << " microseconds";

    SET_LOG_FILE(L"test/ログ.txt");
    LD(L"バグ");
    LI(L"Info");
    LW(L"警告");
    LE(L"エラー");

    IE << "The forth 4 logs elasped: " << sw.perf() << " microseconds";

    IE << "lap: " << sw.lap() << " microseconds.";
}