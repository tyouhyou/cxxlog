# cxxlog

*TL;NR*

This is a C++ logging utility. Easy to output logs to std::cerr or any specified log file(s) with it.

```#include cxxlog.hpp```

```
SET_LOGFILE("test/log.txt");

D << "This is debug message. i=" << i;
I << "For your information. x=" << foo << "; y=" << bar;
W << "Caution!";
E << "Error!!! => " << e.what();
```

All these logs will be output to log.txt file, as follows.

```
[2020-05-03, 13:28:54][DEBUG][main.cpp][4][main] - This is debug message. i=2
[2020-05-03, 13:28:54][INFO] - For your information.
[2020-05-03, 13:28:54][WARN] - Caution!
[2020-05-03, 13:28:54][ERROR][main.cpp][7][main] - Error!!! => exception message.
```

*As you might notice, the debug and error logs have [code information], that includes "code file name", "code line", and "method name". While other logs do not have such information.*

_**Support**_

c++11 and above.

## Introduction

Use iostream style to output logs, and std::endl will be add to the end of the line automatically. And the outputting is thread-safe.
<br>
Usually one app or library has only one log file. In case multiple files are desired, there have another set of macros to fullfill it.
```
DF(file) << "debug";
IF(file) << "info";
WF(file) << "warn";
EF(file) << "error";
```
The usage is the same with D/I/W/E macros except that SET_LOGFILE() has no effect on these macros. The file path set by SET_LOGFILE() just be applied to D/I/W/E macros.

By the way, if no file specified, or the path is not correct, no execption occurres. Also no log output.

However, DF/IF/WF/EF are not thread-safe. (Should be put in the TODO list?)

A third set of macros, DE/IE/WE/EE allow you write logs to std::cerr.
<br>

## Log levels

There have 4 log levels defined: 
```
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
```

To contorl which (and above) level log could be output, define or modify the ```LOG_LEVEL``` micro. Only those logs whose level is equal to or greater than ```LOG_LEVEL```, will be written to file. The lower level logs are suppressed.

For instance, if ```LOG_LEVEL``` is set to _LOG_INFO, then DEBUG level(D / DE / DF) macros will be quiet. And if ```LOG_LEVEL``` is set to _LOG_WARN, DEBUG and INFO level macros shut up.

If "DEBUG" or "_DEBUG" macro is defined, the default log level is _LOG_DEBUG, otherwise, it's _LOG_INFO.

## cxxlog_ws.hpp
At this time being, functions in cxxlog.hpp handle std::string. If std::wstring is desired. Use cxxlog_ws.hpp. However the usage is a bit different. 
```
#include "cxxlog_ws.hpp"
/* thread safe */
D(L"debug");
I(L"info");
W("warn");
E("error");
```
The log format is same as D/I/W/E.

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
- Log level contorl. 

And all the messages will be output to std::cerr. If outputting to file is desired, use redirect can send all logs to the re-directed file.

*******

Any bug reporting and suggestion is welcome. Thanks in advance.
