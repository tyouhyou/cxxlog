#include "cxxlog.hpp"
#include "stopwatch.hpp"

int main()
{
    th_util::StopWatch sw;
    sw.start();

    VF("test/info.log") << "verbose" << std::endl;
    DF("test/debug.log") << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "first 5 logs elasped: " << sw.perf() << " microseconds";

    VF("test/info2.log") << "verbose again." << std::endl;
    DF("test/debug2.log") << "This is debug message again.";
    IF("test/info2.log") << "Information for you again.";
    WF("test/warn2.log") << "I warn you again.";
    EF("test/error2.log") << "An error message goes here again.";

    IE << "second 5 logs elasped: " << sw.perf() << " microseconds";

    SET_LOGGER("test/oo.log");
    V << "verbose" << std::endl;
    D << "debug to cerr";
    I << "info to cerr";
    W << "warn to cerr";
    E << "error to cerr";

    IE << "last 5 logs (to std::cerr) elasped: " << sw.perf() << " microseconds";
    IE << "lap: " << sw.lap() << " microseconds.";
}