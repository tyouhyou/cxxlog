/* *
 * A log utility for c/c++ projects.
 * And also a convient tool to output err message (on errno) into log.
 *
 * Defaultly, log will be output to stderr.
 * Use redirection to put messages to file if stderr is not desired.
 *
 * If used with c++11 (and later) project, cxxlog.hpp is recommended.
 * 
 * @author  tyouhyou    github.com/tyouhyou
 * */

#ifndef TH_CLOG_H
#define TH_CLOG_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

/////////// LOG DEFINE REGION ///////////

#define _LOG_ALL 0
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
#define _LOG_NONE 9

#ifndef LOG_LEVEL
#define LOG_LEVEL _LOG_ALL
#endif

#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#define _OUTPUT_LOG(loglevel, msg_w_fmt, ...)                           \
    do                                                                  \
    {                                                                   \
        fprintf(stderr, loglevel "[%s(%d)::%s][%s:%s] " msg_w_fmt "\n", \
                __FILENAME__,                                           \
                __LINE__,                                               \
                __func__,                                               \
                __DATE__,                                               \
                __TIME__,                                               \
                ##__VA_ARGS__);                                         \
    } while (0)

#if (LOG_LEVEL <= _LOG_DEBUG)
#define LOGD(msg_w_fmt, ...) _OUTPUT_LOG("[  LOG]", msg_w_fmt, ##__VA_ARGS__)
#else
#define LOGD(msg_w_fmt, ...)
#endif
#if LOG_LEVEL <= _LOG_INFO
#define LOGI(msg_w_fmt, ...) _OUTPUT_LOG("[ INFO]", msg_w_fmt, ##__VA_ARGS__)
#else
#define LOGI(msg_w_fmt, ...)
#endif
#if LOG_LEVEL <= _LOG_WARN
#define LOGW(msg_w_fmt, ...) _OUTPUT_LOG("[ WARN]", msg_w_fmt, ##__VA_ARGS__)
#else
#define LOGW(msg_w_fmt, ...)
#endif
#if LOG_LEVEL <= _LOG_ERROR
#define LOGE(msg_w_fmt, ...) _OUTPUT_LOG("[ERROR]", msg_w_fmt, ##__VA_ARGS__)
#else
#define LOGE(msg_w_fmt, ...)
#endif

/////////// ERROR CHECKER REGION ///////////

#if LOG_LEVEL <= _LOG_ERROR
#define INIERR()   \
    do             \
    {              \
        errno = 0; \
    } while (0)
#define CHKERR()                                                    \
    do                                                              \
    {                                                               \
        if (0 != errno)                                             \
            LOGE("errno: %d, message: %s", errno, strerror(errno)); \
    } while (0)
#define CHKERR_F(ret, func, ...)                                    \
    do                                                              \
    {                                                               \
        errno = 0;                                                  \
        ret = func(__VA_ARGS__);                                    \
        if (0 != errno)                                             \
            LOGE("errno: %d, message: %s", errno, strerror(errno)); \
    } while (0)
#define CHKERR_M(method, ...)                                       \
    do                                                              \
    {                                                               \
        errno = 0;                                                  \
        method(__VA_ARGS__);                                        \
        if (0 != errno)                                             \
            LOGE("errno: %d, message: %s", errno, strerror(errno)); \
    } while (0)
#else
#define INIERR()
#define CHKERR()
#define CHKERR_F(ret, func, ...)
#define CHKERR_M(func, ...)
#endif

#endif