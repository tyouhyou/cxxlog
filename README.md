# cxxlog
*C/C++ log utility.*

With this logging util you can output messages to std::err or any specified log file by including header file cxxlog.hpp in your project. It's very easy to use and you can control which level's log can be output or not during compilation.

Also, we provide clog.h for dealing with projects before c++11, you may have a look into the file for detailed information.

**********
## cxxlog.hpp

This utility is used with c++11 or above.

### Usage:

We can deal with outputting as follows
- All logs go to std::err
- All logs go to one file
- More than one log files for outputting.

There are 4 log levels, debug, info, warn, error. Use Macros
- D to output debug level log
- I for information log
- W for warning log
- and E for error log

```#include cxxlog.cpp```
```
D << "This is debug message. i=" << i;
I << "For your information.";
W << "I warn you.";
E << "Error!!! => " << e;
```

Each log take one line. The carriage return will be added automatically.
The outputting are as follows:

```
[DEBUG][May  3 2020][09:26:55][main.cpp][4][main] - This is debug message. i=2
[INFO][May  3 2020][09:26:55] - For your information.
[ERROR][May  3 2020][09:26:55] - I warn you.
[ERROR][May  3 2020][09:26:55] - Error!!! => exception message.
```

As you may notice, the debug log has code information --> code file, line, and method name. While other logs do not have such information.

So, where does the logs go?

If _LOG_FILE macro is defined, the logs will be output to the file defined by this macro.
```#define _LOG_FILE "mylog.txt"```
(or uncomment the line of _LOG_FILE in cxxlog.hpp)

By defining _LOG_FILE, all the logs will be output to that file. if no _LOG_FILE macro defined, the outputting will go to std::err.

Macros
- DE
- IE
- WE
- EE
will alway output logs to std::err, in spite of _LOG_FILE definition.

Then, how to output logs to specified file(s)? It can be accomplished by using macros
- DF(file)
- IF(file)
- WF(file)
- EE(file)

File names passed to these macros, are not necessarily to be same. Any valid file can be used. Logs will be deliverred to the specified file(s). 

In regard to contorl which level log should be output, define or modify the LOG_LEVEL micro. When LOG_LEVEL is greater the log macro level, the macro will not output any log.

Say, if LOG_LEVEL is set to _LOG_INFO, the D / DE / DF macros will be quiet. and if level is set to _LOG_WARN, Debug and Info level macros shut up.


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

And all the messages will be output to std::err. If outputting to file is desired, use redirect can send all logs to the re-directed file.
