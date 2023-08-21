#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

// ANSI Color Definitions
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"
#define ANSI_COLOR_MAGENTA    "\x1b[35m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_LIGHT_GRAY "\x1b[37m"
#define ANSI_COLOR_RESET      "\x1b[0m"

// Windows Color Definitions
#define WIN_COLOR_RED         FOREGROUND_RED
#define WIN_COLOR_GREEN       FOREGROUND_GREEN
#define WIN_COLOR_YELLOW      FOREGROUND_RED | FOREGROUND_GREEN
#define WIN_COLOR_BLUE        FOREGROUND_BLUE
#define WIN_COLOR_MAGENTA     FOREGROUND_RED | FOREGROUND_BLUE
#define WIN_COLOR_CYAN        FOREGROUND_GREEN | FOREGROUND_BLUE
#define WIN_COLOR_LIGHT_GRAY  FOREGROUND_INTENSITY
#define WIN_COLOR_WHITE       FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define WIN_COLOR_LIGHTER_GRAY  FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
typedef enum text_color {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_LIGHT_GRAY,
    COLOR_WHITE,
    COLOR_RESET
} text_color;

#ifdef PLATFORM_WINDOWS
void set_text_color(text_color color) {
    WORD win_color;
    switch (color) {
        case COLOR_RED:        win_color = WIN_COLOR_RED; break;
        case COLOR_GREEN:      win_color = WIN_COLOR_GREEN; break;
        case COLOR_YELLOW:     win_color = WIN_COLOR_YELLOW; break;
        case COLOR_BLUE:       win_color = WIN_COLOR_BLUE; break;
        case COLOR_MAGENTA:    win_color = WIN_COLOR_MAGENTA; break;
        case COLOR_CYAN:       win_color = WIN_COLOR_CYAN; break;
        case COLOR_LIGHT_GRAY: win_color = WIN_COLOR_LIGHTER_GRAY; break;
        case COLOR_WHITE:      win_color = WIN_COLOR_WHITE; break;
        case COLOR_RESET:      win_color = WIN_COLOR_WHITE; break;
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, win_color);
}
#else
void set_text_color(text_color color) {
    const char* ansi_color;
    switch (color) {
        case COLOR_RED:        ansi_color = ANSI_COLOR_RED; break;
        case COLOR_GREEN:      ansi_color = ANSI_COLOR_GREEN; break;
        case COLOR_YELLOW:     ansi_color = ANSI_COLOR_YELLOW; break;
        case COLOR_BLUE:       ansi_color = ANSI_COLOR_BLUE; break;
        case COLOR_MAGENTA:    ansi_color = ANSI_COLOR_MAGENTA; break;
        case COLOR_CYAN:       ansi_color = ANSI_COLOR_CYAN; break;
        case COLOR_LIGHT_GRAY: ansi_color = ANSI_COLOR_LIGHT_GRAY; break;
        case COLOR_WHITE:      ansi_color = ANSI_COLOR_RESET; break;
        case COLOR_RESET:      ansi_color = ANSI_COLOR_RESET; break;
    }
    printf("%s", ansi_color);
}
#endif

b8 initialize_logger() {
    //TODO: Create a log file
    return true;
}

void shutdown_logger() {
    //TODO: Shutdown the logger and dump to a log file
}

VAPI void log_output(log_level level, const char *file, i32 line, const char *format, ...) {
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
    char buffer[10240];
    vsprintf(buffer, format, args);
            va_end(args);

    // Getting the current time and formatting it
    time_t rawtime;
    struct tm * timeinfo;
    char timeBuffer[9];  // for hh:mm:ss

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", timeinfo);

    // Print LEVEL with color
    switch (level) {
        case LOG_LEVEL_FATAL:
            set_text_color(COLOR_RED);
            printf("FATAL");
            break;
        case LOG_LEVEL_DEBUG:
            set_text_color(COLOR_CYAN);
            printf("DEBUG");
            break;
        case LOG_LEVEL_TRACE:
            set_text_color(COLOR_BLUE);
            printf("TRACE");
            break;
        case LOG_LEVEL_INFO:
            set_text_color(COLOR_GREEN);
            printf("INFO");
            break;
        case LOG_LEVEL_WARN:
            set_text_color(COLOR_YELLOW);
            printf("WARN");
            break;
        case LOG_LEVEL_ERROR:
            set_text_color(COLOR_MAGENTA);
            printf("ERROR");
            break;
        default:
            set_text_color(COLOR_WHITE);
            printf("UNKNOWN");
            break;
    }

    // Print TIME
    set_text_color(COLOR_WHITE);
    printf(" | %s | %s:%d - %s\n", timeBuffer, file_name, line, buffer);
}

