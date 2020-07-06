# cxxlog

*TL;NR*

This is a C++ logging utility. It is very easy to output logs to std::cerr or any specified log file(s). 

```#include cxxlog.hpp```

```
D << "This is debug message. i=" << i;
I << "For your information.";
W << "Caution!";
E << "Error!!! => " << e;
```

Output will be:

```
[DEBUG][May  3 2020][09:26:55][main.cpp][4][main] - This is debug message. i=2
[INFO][May  3 2020][09:26:55] - For your information.
[ERROR][May  3 2020][09:26:55] - Caution!
[ERROR][May  3 2020][09:26:55][main.cpp][7][main] - Error!!! => exception message.
```

*Support*

c++11 and above.

## Introduction

As you might notice, the debug and error logs have [code information], that includes "code file name", "code line", and "method name". While other logs do not have such information.

## Specify log file(s)

### _LOG_FILE macro

If ```_LOG_FILE``` macro is defined, logs will be written to that file.

e.g.

```#define _LOG_FILE "path/to/mylog.txt"```

(or un-comment the line of _LOG_FILE in cxxlog.hpp)

If no _LOG_FILE macro defined, the outputting will go to std::cerr.

On the contrary, the following macros will alway output logs to std::cerr, in spite of _LOG_FILE definition.
- DE
- IE
- WE
- EE

### Log files other than _LOG_FILE

In case there have log files other than ```_LOG_FILE``` for outputting, it can be accomplished by using following macros:
- DF(file)
- IF(file)
- WF(file)
- EE(file)

The usage and output formatting is same as E/I/W/E macros, but an argument of file added. The outputting of message will not go to ```_LOG_FILE```, but to the specified one.

File names passed to these macros, are not necessarily to be the same. Any valid file can be used. Logs will be deliverred to the specified file(s). 

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


**NOTE:** 
- The specified log files will be opened all through the app life span, and be closed when the app goes end.
- If thread safe is desired, define _LOG_LOCK (or uncomment the line in cxxlog.hpp). 

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
