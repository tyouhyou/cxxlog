// #define _WLOGGER // just for test on windows

#include "../include/cxxlog.hpp"
#include "../include/decorator.hpp"
#include "../include/stopwatch.hpp"
#include "../include/strcvt.hpp"
#include <codecvt>
#include <locale>
#include <thread>
#include <chrono>

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
    Stopwatch<> sw;
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

    std::cerr << "elapsed: " << sw.elaspsed() << " nanoseconds.";
#endif
}

// template <class TF, class TB, class TA>
// auto deco_beforeafter(TF &&fun, TB &&before, TA &&after)
// {
//     return
//         [fun = std::forward<TF>(fun),
//          before = std::forward<TB>(before),
//          after = std::forward<TA>(after)](auto &&...args) -> decltype(auto)
//     {
//         deco<std::result_of_t<TF(decltype(args)...)>, decltype(args)...> ret;
//         ret.wrap(fun);
//         before();
//         auto rst = ret.call(std::forward<decltype(args)>(args)...);
//         after();
//         return rst;
//     };
// }

void test_logger()
{
#ifndef _WLOGGER

    Stopwatch_ns sw;

    SET_LOG_FILE(W2SL(L"test/ログ.txt"));
    SET_LOG_MAX_SIZE(20);

    DE << "Sleep a second before start.";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sw.start();
    sw.mark("start");

    DL << "Here you are, BUG.";
    IL << "Let me inform you.";
    WL << "Alert, paradise is falling";
    EL << "OMG, it crashed.";
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    DL << "バグ";
    IL << "Info";
#ifndef _WIN32
    WL << "警告";
    EL << "エラー";
#endif

    IE << "The first group elasped: " << sw.wrap() << " ns";
    sw.mark("first group end");
    IE << "start -> 1 (ns) : " << sw.measure("start", "first group end");

    DF(W2SL(L"test/debug.log")) << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "The second group elasped: " << sw.wrap() << " nanoseconds";
    sw.mark("second group end");
    IE << "1 -> 2: " << sw.measure("first group end", "second group end");

    DF("test/log.txt") << "バグ";
    IF("test/log.txt") << "Info";
#ifndef _WIN32
    WF("test/log.txt") << "警告";
    EF("test/log.txt") << "エラー";
#endif

    IE << "The third group elasped: " << sw.wrap() << " nanoseconds";
    sw.mark("third group end");
    IE << "2 -> 3: " << sw.measure("second group end", "third group end");

    IE << "start -> Third group: " << sw.measure("start", "third group end");
    IE << "Totally: " << sw.elaspsed() << " nanoseconds.";

    // auto callable = deco_beforeafter(
    //     [](int a, int b) -> int
    //     { return a + b; },
    //     [&]()
    //     { IE << "before callable"; },
    //     [&]()
    //     { IE << "after callable"; });
    // auto ret = callable(11, 22);
    // IE << "callable returns " << ret;

    bool rst_before = false;
    bool rst_after = false;
    Deco_func<int, int> cal;
    cal.wrap([](int count) -> int
             {
        auto ret = 0;
        for(int i=0; i<count; i++)
        {
            ret += i;
        }
        return ret; })
        .wrap_before([&sw](int) -> bool
                     {
        IE << "func starts.";
        sw.mark("count");
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        return true; })
        .wrap_after([&sw](int) -> bool
                    { IE << "func ended. elapsed: " << sw.measure("count") << " ns.";
                return true; });

    // auto val = cal.call(1000).result(rst_before, rst_after);
    auto val = cal(100);
    IE << "count result: " << val;
    IE << "func before result: " << rst_before;
    IE << "func after result: " << rst_after;

    rst_before = false;
    rst_after = false;
    Deco_action<int>()
        .wrap([](int count) -> void
              {
        auto ret = 0;
        for(int i=0; i<count; i++)
        {
            ret += i;
        } 
                  IE << "action, loop result: " << ret; })
        .wrap_before([&sw](int) -> bool
                     {
        IE << "action starts.";
        sw.mark("noreturn");
        return true; })
        .wrap_after([&sw](int) -> bool
                    { IE << "action ended. elapsed: " << sw.measure("noreturn") << " ns.";
                      return true; })
        .call(100000)
        .result(rst_before, rst_after);
    IE << "action before result: " << rst_before;
    IE << "action after result: " << rst_after;

    IE << "Totally: " << ns2ms(sw.measure("start")) << " ms.";

#endif
}