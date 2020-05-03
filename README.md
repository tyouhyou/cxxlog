# cxxlog
*C/C++ log utility.*

There has two head files to provide the logging functions, clog.h and cxxlog.hpp. 

As the file names indicate, clog.h is mainly for using with c or c++ (c++03 and early) project while cxxlog.hpp is expected to be with c++  project. However, you may also use clog.h in your c++11 project. These two utility are not of the same functions.

**********
##cxxlog.hpp

####_Usage:_

```
D(file_d) << "This is debug message. i=" << i;
I(file_i) << "For your information.";
W(file_w) << "I warn you.";
E(file_e) << "Error!!! => " << e;
```
The output of logs are as follows:

```
[DEBUG][main.cpp][4][main][May  3 2020][09:26:55] - This is debug message. i=2
[ERROR][May  3 2020][09:26:55] - An error message goes here.
```
Debug log has file, line, method information while other messages do not have it.
Even carriage return is not added at the end of each log, one message take one line exactly, saving us from typing [ << endl ].

If you need outputting message without carriage return added automatically, use V(file) macro. And when carriage return is needed, VL(file) will add it.

Also, V(f) does not output any formatted information at the beginning of the message.

*__In Regard to the log file(s)__*

The above functions will output message to the file(s) in brackets. To make things simplier, you may define some new macros to omit the typing of file path. For example:

`#define DF D("path/to/mylog.log")`

The specified log files will be opened all the life span of the application. And be closed when the application goes end.

Also messages can be output to stderr instead of file(s).
```
DEBUGL << "This is debug message. i=" << i;
INFOL << "For your information.";
WARNL << "I warn you.";
ERRORL << "Error!!! => " << e;
```

**NOTE:** 
- While creating / openning log file is thread safe, outputting message is NOT thread safe for. If performance does matter, comment out the following line in the hpp file.
`#define __LOG_LOCK_GET__`
It will omit the locking when creating / openning log file.
- There have four log levels. However, you cannot control which level's log can be output and which ones should not during compile. If this is what you want, use functions in clog.h.

******
##clog.h

####_Usage:_
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