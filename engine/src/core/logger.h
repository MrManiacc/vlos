//
// Created by jwraynor on 8/21/2023.
//

#pragma once

#include "defines.h"

#define LOG_FATAL_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_WARN_ENABLED 1
#define LOG_ERROR_ENABLED 1

#if defined(VRELEASE)
#defineLOG_DEBUG_ENABLED 0
#defineLOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_TRACE = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_WARN = 4,
    LOG_LEVEL_ERROR = 5
} log_level;


b8 initialize_logger();

void shutdown_logger();

VAPI void log_output(log_level level, const char *file, i32 line, const char *format, ...);

#if LOG_FATAL_ENABLED == 1
#define FATAL(format, ...) log_output(LOG_LEVEL_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);
#elif
#define FATAL(format, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define INFO(format, ...) log_output(LOG_LEVEL_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#elif
#define INFO(format, ...)
#endif

#if LOG_WARN_ENABLED == 1
#define WARN(format, ...) log_output(LOG_LEVEL_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);
#elif
#define WARN(format, ...)
#endif

#if LOG_ERROR_ENABLED == 1
#define ERROR(format, ...) log_output(LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#elif
#define ERROR(format, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define DEBUG(format, ...) log_output(LOG_LEVEL_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#else
#define DEBUG(format, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define TRACE(format, ...) log_output(LOG_LEVEL_TRACE, __FILE__, __LINE__, format, ##__VA_ARGS__);
#else
#define TRACE(format, ...)
#endif