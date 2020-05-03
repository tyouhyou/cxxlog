#include "../include/cxxlog.hpp"

int main(int argc, char *argv[])
{
    D("bin/debug.log") << "This is debug message.";
    I("bin/info.log") << "Information for you.";
    W("warn.log") << "I warn you.";
    E("error.log") << "An error message goes here.";
    D("bin/debug.log") << "This is debug message again.";
    I("bin/info.log") << "Information for you again.";
    W("bin/warn.log") << "I warn you again.";
    E("bin/error.log") << "An error message goes here again.";

    DEBUGL << "bug to cerr";
    INFOL << "info to cerr";
}