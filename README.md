# Logging utilities

In this toolkit, there have loggers for c, c++11, and a stopwatch.

They are implemented in separate header files, in [include] folder. Just include the one you desire and have fun.

## cxxlog.hpp

### TL;NR

It is easy to write log in "stream style", to any specified log file(s) or std::cerr by including this header file.

```c++11
#include cxxlog.hpp

SET_LOG_FILE("test/log.txt");   // Specify log path for using with DL/IL/WL/EL
SET_LOG_MAX_SIZE(10000);        // The max lines in one log file.

/* The macros below are of thread-safe */
DL << "This is debug message. i=" << i;
IL << "For your information. x=" << foo << "; y=" << bar;
WL << "Caution!";
EL << "Error! => " << e.what();
```

All these logs will be output to [test/log.txt] file, as follows:

```outputting sample
[2020-05-03, 13:28:54][DEBUG][main.cpp(4)::main] This is debug message. i=2
[2020-05-03, 13:28:54][INFO ] For your information.
[2020-05-03, 13:28:54][WARN ] Caution!
[2020-05-03, 13:28:54][ERROR][main.cpp(7)::main] Error! => exception message.
```

_**Support**_

c++11 and above.

### Introduction

There have 3 groups of macros for logging.

- DL/IL/WL/EL
- DF/IF/WF/EF
- DE/IE/WE/EE

```remark
"Dx" - DEBUG level logs.
"Ix" - INFO level logs.
"Wx" - WARN level logs.
"Ex" - ERROR level logs.

the DEBUG and ERROR logs have code information, that is the "[file_name(line)::method_name]" portion of the above outputting sample.
While INFO/WARN logs do not have such information.

And, std::endl will be add to the end of the line automatically.
```

DL/IL/WL/EL should be used with SET_LOG_FILE(), which set the log file path for outputting text. The "L" after D/I/W/E means they are "locked" when writting file. If thread safe is a concern, use this group macro.

DF/IF/WF/EF has nothing to do with the log file set by SET_LOG_FILE(). The "F" after D/I/W/F stand for "file". That is they write text to the file passed as parameter.

```sample
/* not thread safe */

DF(hello.txt) << "World";
DF(debug.txt) << "I found you, bug";
EF(error.log) << ex.what();
...

The file to the parameter can be any valid one, not necessarily be the same file.
```

For the above two groups, the log file path can be relative or absolute one. But the folder should exist. If any folder in the specified file path does ont exist, it will not be created, thus, logs cannot be written, and no exception will be thrown.

DE/IE/WE/EE group allows you write logs to std::cerr.

```sample code
IE << "output to standard error.";
```

### Max lines in one log file

```SET_LOG_MAX_SIZE(line_number);``` can specify the max lines in one log file. If the max number has reached, the first line will be removed and the new one be appended to the end of the file. Set to [0] or just not call this function means no max limit. It may cause problem when file goes big. Take care.

The max line limit apply to DL/IL/WL/EL and DF/IF/WF/EF groups only. It has no effect on DE/IE/WE/EE macros.

### Log levels

There have four levels defined.

```c++11
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
```

To contorl which (and above) level's log could be output, define `LOG_LEVEL` micro during compilation or in header file. Only those logs whose level is equal to or greater than `LOG_LEVEL`, will be written to file. The lower level logs are overlooked. (In fact, they will be optimized or removed by the compiler during compilation.)

For instance, when set `LOG_LEVEL` to "\_LOG_INFO", the DL/DE/DF macros will be quiet. Set `LOG_LEVEL` to "\_LOG_WARN", DEBUG and INFO logs shut up.

If no `LOG_LEVEL` is defined, the default value will be taken into effect. When `DEBUG` or `_DEBUG` macro is defined, the default log level is "\_LOG_DEBUG", otherwise, it's "\_LOG_INFO".

---

### wild char support on windows

By defining '''_WLOGGER''' macro, wild char (std::wstring) is used with the loggers.

However, this is just for using on Windows. On linux, doing so may cause compilation error at std::wofstream.open().

I don't think use wild char on linux is a good idea, so just left it un-fixed.

Just do not use \_WLOGGER on linux.

---

## stopwatch.hpp

And a simple stopwatch utility also included in this toolkit. It's simple and just have a look at the stopwatch.hpp.

---

## clog.h

A simple logging utility for using with c or c++ before 11.

_***Usage:***_

```c
LOGD("%s. (i=%d)", "Hello", 2);
LOGI("%s", "I am information.");
LOGW("%s", "I am warning.");
LOGE("%s", "I am error.");
```

They somehow look like fprintf. The pros over fprintf are:

- Print formatted information before your message.
- Log level contorl.

And all the messages are written to std::cerr. If outputting to file is desired, use redirect can send all logs to the re-directed file.

---

Have fun, and any bug reporting / suggestion is welcome. Thanks in advance.
