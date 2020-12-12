# cxxlog.hpp

## TL;NR

It is easy to output logs to any specified log file(s) or std::cerr by including this header file, as follows:

```c++11
#include cxxlog.hpp

/* Specify the log path, both absolute and relative are OK. */
SET_LOGFILE("test/log.txt");

/* thread safe */
DL << "This is debug message. i=" << i;
IL << "For your information. x=" << foo << "; y=" << bar;
WL << "Caution!";
EL << "Error! => " << e.what();
```

All these logs will be output to log.txt file.

```txt
[2020-05-03, 13:28:54][DEBUG][main.cpp(4)::main] - This is debug message. i=2
[2020-05-03, 13:28:54][INFO ] - For your information.
[2020-05-03, 13:28:54][WARN ] - Caution!
[2020-05-03, 13:28:54][ERROR][main.cpp(7)::main] - Error! => exception message.
```

_As you might notice, the DEBUG and ERROR logs have code information, that is "[file_name(line)::method_name". While INFO/WARN logs do not have such information._

_**Support**_

c++11 and above.

## Introduction

With the macros shown above, writting log can be fulfilled in an "iostream style". And, std::endl will be add to the end of the line automatically.

Before log can be output, the log file path should be specified via SET_LOGFILE(path). The path can be relative or absolute. But the folder should be an existing one. If any folder in the specified file path does ont exist, it will not be created, thus, logs cannot be written, and no exception will be thrown. Even writting is performed before file path setting, it causes no exception.

Usually one application or library use only one log file. In case multiple files are desired, there have another set of macros to fullfill it.

```c++11
/* not thread safe */
DF(file) << "debug";
IF(file) << "info";
WF(file) << "warn";
EF(file) << "error";
```

The usage and outputting is the same as DL/IF/WF/EF macros except that SET_LOGFILE() has no effect on these macros. The file path set by SET_LOGFILE() just be applied to DL/IL/WL/EL macros.

Also, if no file specified, or the path is not correct, no execption occurres.

DF/IF/WF/EF are not thread safe. You may implement it if that is desired. The reason non-thread-safe version is designed, is that it's a bit faster than the thread-safe one. And also the provide flexibility when multiple logs are expected in one application or library.

There has a trick to use the DF/IF/WF/EF ones.

```c++11
#define DFM DF("my.log")
... and so on

DFM << "buf information";
```

And, there has a third set of macros, DE/IE/WE/EE allow you write logs to std::cerr.

## Log levels

Four log levels defined.

```c++11
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
```

To contorl which (and above) level's log could be output, define `LOG_LEVEL` micro during compilation or in header file. Only those logs whose level is equal to or greater than `LOG_LEVEL`, will be written to file. The lower level logs are overlooked.

For instance, when set `LOG_LEVEL` to "\_LOG_INFO", the DL/DE/DF macros will be quiet. Set `LOG_LEVEL` to "\_LOG_WARN", DEBUG and INFO logs shut up.

If no `LOG_LEVEL` is defined, default value will be taken into effect. When `DEBUG` or `_DEBUG` macro is defined, the default log level is "\_LOG_DEBUG", otherwise, it's "\_LOG_INFO".

---

# wstring support on windows

By adding '''#define \_WLOGGER''' or define \_WLOGGER on compile command line, std::wstring can be used with the logger. 

However, this is just for using on Windows. On linux, doing like this may cause build error at std::wofstream.open().
And I don't think use wstring on linux is a good idea, so just left it unfixed.

Just do not use \_WLOGGER on linux.

---

# clog.h

_***Usage:***_

```c
LOGD("%s. (i=%d)", "Hello", 2);
LOGI("%s", "I am information.");
LOGW("%s", "I am warning.");
LOGE("%s", "I am error.");
```

They somehow look like fprintf. The pros over fprintf are:

- A carriage return will be added automatically.
- Print formatted information before your message.
- Log level contorl.

And all the messages are written to std::cerr. If outputting to file is desired, use redirect can send all logs to the re-directed file.

---

Any bug reporting and suggestion is welcome. Thanks in advance.
