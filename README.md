# cxxlog
*C/C++ log utility.*

There have two head files to provide the logging functions, clog.h and cxxlog.hpp. 

As the file names indicate, clog.h is mainly for using with c or c++ (c++03 and early) project while cxxlog.hpp is expected to be with c++ (c++11 and later) project. However, you may also use clog.h in your c++11 project. These two utility are not of the same functions.

**********
## cxxlog.hpp

_***Usage:***_

```
D(file) << "This is debug message. i=" << i;
I(file) << "For your information.";
W(file) << "I warn you.";
E(file) << "Error!!! => " << e;
```
(The specified log files may vary, are not necessarily to be same.)

The output of logs are as follows:

```
[DEBUG][main.cpp][4][main][May  3 2020][09:26:55] - This is debug message. i=2
[INFO][May  3 2020][09:26:55] - For your information.
[ERROR][May  3 2020][09:26:55] - I warn you.
[ERROR][May  3 2020][09:26:55] - An error message goes here.
```
Debug log has file name, line number, method name etc. information while other logs not.
Carriage return is added automatically at the end of each log message. One message take one line exactly, saving us from typing [ << endl ].

If outputting message without carriage return is desired, use V(file) macro, which outputting message as it is, without any extra info or formatting. And use VL(file) to add the carriage return. (endl cannot be added directly to the log stream.)

__*In Regard to the log file(s)*__

The above functions will output message to the file(s) in brackets. To make things simplier, you may define some new macros to omit the typing of file path. For example:

`#define DF D("path/to/mylog.log")`

The specified log files will be opened through the app life span. And be closed when the app goes end.

Also messages can be output to stderr instead of file(s). Use the following macros.
```
DEBUGL << "This is debug message. i=" << i;
INFOL << "For your information.";
WARNL << "I warn you.";
ERRORL << "Error!!! => " << e;
```

**NOTE:** 
- While creating / openning log file is thread safe, outputting message is NOT thread safe. If performance does matter, comment out the following line in the hpp file.
`#define __LOG_LOCK_GET__`
It will also omit locking when create / open log file.
- There have four log levels (debug, info, warn, error). However, you cannot control which level(s) can be output and which ones should not during compile. Anyway, you may use clog.h to control which level can be output, or just define other macros to mask the exist ones. 

******
## clog.h

_***Usage:***_
```
LOGD("%s. (i=%d)", "Hello", 2);
LOGI("%s", "I am information.");
LOGW("%s", "I am warning.");
LOGE("%s", "I am error.");
```
They somehow look like fprintf. The pros over fprintf are:
- Carriage return will be added automatically.
- Print formatted information before your message.
- Log level contorl. You can determine which level's log could be output by define LOG_LEVEL. Say `#define LOG_LEVEL _LOG_WARN` makes LOGD and LOGI not work, while LOGW and LOGE can output their logs.

All the log messages will be output to stderr. If outputting to file is desired, use redirect can send all logs to the re-directed file.
