#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "platform/platform.h"

b8 logger_initialize() {
    vdebug("Initialized the logger")
    return true;
}

void logger_shutdown() {
    vdebug("Shutting down the logger")
}

VAPI void log_output(log_level level, const char *file, i32 line, const char *format, ...) {
    b8 is_error = level == LOG_LEVEL_ERROR || level == LOG_LEVEL_FATAL;
    void (*print)(conststr, u8) = is_error ? &platform_console_write_error : &platform_console_write;
    const char *file_name = strrchr(file, '/');
    if (!file_name) {
        file_name = strrchr(file, '\\');
    }
    if (file_name == NULL) {
        file_name = file;
    } else {
        file_name++;
    }

    va_list args;
    va_start(args, format);
    static char buffer[32000];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Getting the current time and formatting it
    time_t rawtime;
    struct tm *tm;
    char timeBuffer[9];  // for hh:mm:ss

    time(&rawtime);
    tm = localtime(&rawtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", tm);

    static conststr levels[6] = {"[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]"};
    (*print)(levels[level], level);

    //combine the time, file, and line number
    char header[256];
    if (level == LOG_LEVEL_WARN || level == LOG_LEVEL_INFO)
        sprintf(header, "  // %s // %s:%d // ", timeBuffer, file_name, line);
    else
        sprintf(header, " // %s // %s:%d // ", timeBuffer, file_name, line);
    (*print)(header, LOG_LEVEL_WARN);
    (*print)(buffer, 3);
    (*print)("\n", LOG_LEVEL_INFO);
}

