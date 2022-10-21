//#define _WLOGGER // just for test on windows

#include <locale>
#include <codecvt>
#include "cxxlog.hpp"
#include "stopwatch.hpp"
#include "strcvt.hpp"

using namespace zb;

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
    StopWatch sw;
    sw.start();

    SET_LOG_FILE(S2WL("test/ログ.txt"));
    SET_LOG_MAX_SIZE(0);

    DL << L"バグ";
    IL << L"Info";
    WL << L"警告";
    EL << L"エラー";

    DF(S2WL("test/log.txt")) << L"バグ";
    IF(L"test/log.txt") << L"Info";
    WF(L"test/log.txt") << L"警告";
    EF(L"test/log.txt") << L"エラー";

    std::cerr << "elapsed: " << sw.elaspsed() << " microseconds.";
#endif
}

void test_logger()
{
#ifndef _WLOGGER

    StopWatch sw;

    SET_LOG_FILE(W2SL(L"test/ログ.txt"));
    SET_LOG_MAX_SIZE(20);

    sw.start();
    sw.mark("start");

    DL << "Here you are, BUG.";
    IL << "Let me inform you.";
    WL << "Alert, paradise is falling";
    EL << "OMG, it crashed.";
    DL << "バグ";
    IL << "Info";
    WL << "警告";
    EL << "エラー";

    IE << "The first group elasped: " << sw.wrap() << " microseconds";
    sw.mark("first group end");
    IE << "start -> 1: " << sw.measure("start", "first group end");

    DF(W2SL(L"test/debug.log")) << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "The second group elasped: " << sw.wrap() << " microseconds";
    sw.mark("second group end");
    IE << "1 -> 2: " << sw.measure("first group end", "second group end");

    DF("test/log.txt") << "バグ";
    IF("test/log.txt") << "Info";
    WF("test/log.txt") << "警告";
    EF("test/log.txt") << "エラー";

    IE << "The third group elasped: " << sw.wrap() << " microseconds";
    sw.mark("third group end");
    IE << "2 -> 3: " << sw.measure("second group end", "third group end");

    IE << "start -> Third group: " << sw.measure("start", "third group end");
    IE << "Totally: " << sw.elaspsed() << " microseconds.";

#endif
}