#include "log.h"
#include "core.h"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>


//TODO byte sisteminde yazdırmak için fonksiyon yap toKilobyte toMegabyte gibi

void _log(const char *level, short unsigned int color, const char *file, int line, const char *func, const char *format, ...)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    va_list args;
    va_start(args, format);
    fprintf(stderr, "\x1b[3%dm[%s] [%s] [%s:%s():%d] ", color, time_str, level, file, func, line);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    fprintf(stderr, "\x1b[0m\n");
}

struct DisposableString print_vec3(float v[3])
{
    return format("vec3: [%.3f, %.3f, %.3f]\n", v[0], v[1], v[2]);
}

struct DisposableString print_vec4(float(v)[4])
{
    return format("vec4: [%.3f, %.3f, %.3f, %.3f]\n", (v)[0], (v)[1], (v)[2], (v)[3]);
}

struct DisposableString print_mat4(float(m)[4][4])
{
    return format("mat4:\n %D %D %D %D",
                  format("[%.3f, %.3f, %.3f, %.3f]\n", (m)[0][0], (m)[1][0], (m)[2][0], (m)[3][0]),
                  format("[%.3f, %.3f, %.3f, %.3f]\n", (m)[0][1], (m)[1][1], (m)[2][1], (m)[3][1]),
                  format("[%.3f, %.3f, %.3f, %.3f]\n", (m)[0][2], (m)[1][2], (m)[2][2], (m)[3][2]),
                  format("[%.3f, %.3f, %.3f, %.3f]\n", (m)[0][3], (m)[1][3], (m)[2][3], (m)[3][3]));
}
