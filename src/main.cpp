// #define _WLOGGER // just for test on windows

#include "cxxlog.hpp"
#include "stopwatch.hpp"

using namespace th_util;

void test_logger();
void test_wlogger();

int main()
{
    test_logger();
    test_wlogger();
}

void test_wlogger()
{
#ifdef _WLOGGER
    th_util::StopWatch sw;
    sw.start();

    SET_LOG_FILE(L"test/ログ.txt");
    DL << L"バグ";
    IL << L"Info";
    WL << L"警告";
    EL << L"エラー";

    std::cerr << "elapsed: " << sw.elaspsed() << " microseconds.";
#endif
}

void test_logger()
{
#ifndef _WLOGGER

    th_util::StopWatch sw;
    sw.start();

    SET_LOG_FILE("test/ログ.txt");
    SET_LOG_MAX_SIZE(20);

    DL << "Here you are, BUG.";
    IL << "Let me inform you.";
    WL << "Alert, paradise is falling";
    EL << "OMG, it crashed.";
    DL << "バグ";
    IL << "Info";
    WL << "警告";
    EL << "エラー";

    IE << "The first 4 logs elasped: " << sw.wrap() << " microseconds";

    DF("test/debug.log") << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "The second 4 logs elasped: " << sw.wrap() << " microseconds";

    DF("test/debug2.log") << "This is debug message again.";
    IF("test/info2.log") << "Information for you again.";
    WF("test/warn2.log") << "I warn you again.";
    EF("test/error2.log") << "An error message goes here again.";

    IE << "The third 4 logs elasped: " << sw.wrap() << " microseconds";

    DF("test/log.txt") << "バグ";
    IF("test/log.txt") << "Info";
    WF("test/log.txt") << "警告";
    EF("test/log.txt") << "エラー";

    IE << "lap: " << sw.elaspsed() << " microseconds.";

#endif
}