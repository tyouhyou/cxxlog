#include "../include/cxxlog.hpp"
#include "../include/stopwatch.hpp"

int main(int argc, char *argv[])
{
    th_util::StopWatch sw;
    sw.start();

    D("bin/debug.log") << "This is debug message.";
    I("bin/info.log") << "Information for you.";
    W("warn.log") << "I warn you.";
    E("error.log") << "An error message goes here.";

    INFOL << "first 4 logs elasped: " << sw.perf() << " microseconds";

    D("bin/debug.log") << "This is debug message again.";
    I("bin/info.log") << "Information for you again.";
    W("bin/warn.log") << "I warn you again.";
    E("bin/error.log") << "An error message goes here again.";

    INFOL << "second 4 logs elasped: " << sw.perf() << " microseconds";

    DEBUGL << "bug to cerr";
    INFOL << "info to cerr";

    INFOL << "last 2 logs elasped: " << sw.perf() << " microseconds";
    INFOL << "lap: " << sw.lap() << " microseconds.";
}