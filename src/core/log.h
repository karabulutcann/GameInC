#pragma once
#include <stdio.h>

// ANSI Color Codes
#define CLR_RESET  "\x1b[0m"
#define CLR_RED    "\x1b[31m"
#define CLR_GREEN  "\x1b[32m"
#define CLR_YELLOW "\x1b[33m"
#define CLR_BLUE   "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN   "\x1b[36m"

// The core logging engine with color support
#define LOG(color, level_str, fmt, ...) \
    fprintf(stdout, color "[" level_str "] " fmt CLR_RESET "  func: %s  %s:%d\n", \
            ##__VA_ARGS__, __func__, __FILE__, __LINE__)

// INFO (Cyan)
#define INFO(fmt, ...) \
    do { LOG(CLR_CYAN, "INFO", fmt, ##__VA_ARGS__); } while(0)

// WARN (Yellow)
#define WARN(fmt, ...) \
    do { LOG(CLR_YELLOW, "WARN", fmt, ##__VA_ARGS__); } while(0)

// ERROR (Red)
#define ERROR(fmt, ...) \
    do { LOG(CLR_RED, "ERROR", fmt, ##__VA_ARGS__); } while(0)
