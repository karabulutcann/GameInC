#pragma once
#include "safe_strings.h"
#define DEBUG_ENABLED

#define COLOR_RESET 0
#define COLOR_ERROR 1
#define COLOR_INFO 2
#define COLOR_DEBUG 6
#define COLOR_WARN 3


// bu calismiyor 
#define mCglmPrint(var) _Generic((var), \
    float[3]: print_vec3,            \
    float[4]: print_vec4,            \
    float[4][4]: print_mat4)(var)

struct DisposableString print_vec3(float v[3]);

struct DisposableString print_vec4(float(v)[4]);

struct DisposableString print_mat4(float(m)[4][4]);

#define mPrintFormatted(formattedString)               \
    {                                                  \
        struct DisposableString tmp = formattedString; \
        mUseDisposableString(tmp)                      \
        {                                              \
            printf("%s", tmp.string);      \
        }                                              \
    }

#define INFO(...) _log("INFO", COLOR_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ERROR(...) _log("ERROR", COLOR_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARN(...) _log("WARN", COLOR_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef DEBUG_ENABLED
#define DEBUG(...) _log("DEBUG", COLOR_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define DEBUG(...)
#endif

void _log(const char *level, short unsigned int color, const char *file, int line, const char *func, const char *format, ...);
