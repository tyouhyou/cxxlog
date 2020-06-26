#include "../include/cxxlog.hpp"
#include "../include/stopwatch.hpp"

int main(int argc, char *argv[])
{
    th_util::StopWatch sw;
    sw.start();

    DF("test/debug.log") << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "first 4 logs elasped: " << sw.perf() << " microseconds";

    DF("test/debug2.log") << "This is debug message again.";
    IF("test/info2.log") << "Information for you again.";
    WF("test/warn2.log") << "I warn you again.";
    EF("test/error2.log") << "An error message goes here again.";

    IE << "second 4 logs elasped: " << sw.perf() << " microseconds";

    D << "debug to cerr";
    I << "info to cerr";
    W << "warn to cerr";
    E << "error to cerr";

    IE << "last 4 logs (to std::cerr) elasped: " << sw.perf() << " microseconds";
    IE << "lap: " << sw.lap() << " microseconds.";
}