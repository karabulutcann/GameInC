#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "core.h"

// Doğrudan kullanma bunun yerine format() kullan.
// DisposableString kullanıldıktan sonra free()lemiyor kullanma
// TODO son harfi yazdirmiyor
#ifndef FORMATTER
#define FORMATTER
int _formatter(char *buffer, const size_t size, const char *fmt, va_list args)
{
    if (fmt == NULL)
        return 0;

    const char *fmt_ptr = fmt;
    size_t total_written = 0;

    while (*fmt_ptr != '\0')
    {
        size_t written = 0;
        if (*fmt_ptr == '%')
        {
            switch (*(fmt_ptr + 1))
            {
            case 'D':
                struct DisposableString disposable = va_arg(args, struct DisposableString);
                written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", disposable.string) - 1;
                fmt_ptr += 2; // Skip the custom format specifier (e.g., "%D")
                if (buffer != NULL)
                    free(disposable.string);
                break;
            case 'L':
            case 'h':
            case 'l':
                if (*(fmt_ptr + 2) == 'l')
                {
                    char tmp[5] = {'%', *(fmt_ptr + 1), *(fmt_ptr + 2), *(fmt_ptr + 3), '\0'};
                    switch (*(fmt_ptr + 3))
                    {
                    case 'd':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, '\0', va_arg(args, long long)) - 1;
                        break;
                    case 'x':
                    case 'u':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, '\0', va_arg(args, unsigned long long)) - 1;
                        break;
                    case 'f':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, va_arg(args, long double)) - 1;
                        break;
                    default:
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", "(not defined)") - 1;
                        break;
                    }
                    fmt_ptr += 4;
                }
                else
                {
                    char tmp[4] = {'%', *(fmt_ptr + 1), *(fmt_ptr + 2), '\0'};
                    switch (*(fmt_ptr + 2))
                    {
                    case 'd':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, va_arg(args, long)) - 1;
                        break;
                    case 'x':
                    case 'u':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, va_arg(args, unsigned long)) - 1;
                        break;
                    case 'f':
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmp, va_arg(args, double)) - 1;
                    default:
                        written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", "(not defined)") - 1;
                    }
                    fmt_ptr += 3;
                }
                break;
            case '.':
                int i = 0;
                Bool isDigit = TRUE;
                while (isDigit)
                {
                    if (*(fmt_ptr + 2 + i) >= '0' && *(fmt_ptr + 2 + i) <= '9')
                    {
                        i++;
                    }
                    else
                    {
                        isDigit = FALSE;
                    }
                }
                char *tmpBuffer = (char *)malloc(i + 4);
                snprintf(tmpBuffer, i + 4, "%s", fmt_ptr);
                switch (*(fmt_ptr + 2 + i))
                {
                case 'f':
                    written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, tmpBuffer, va_arg(args, double)) - 1;
                    break;
                default:
                    written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", "(not defined)") - 1;
                }
                fmt_ptr += i + 3;
                free(tmpBuffer);
                break;
            case 'd':
                written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%d", va_arg(args, int)) - 1;
                fmt_ptr += 2;
                break;
            case 'f':
                written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%f", va_arg(args, double)) - 1;
                fmt_ptr += 2;
                break;
            case 's':
                written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", va_arg(args, const char *)) - 1;
                fmt_ptr += 2;
                break;
            default:
                written = snprintf(buffer + total_written, size == 0 ? 0 : size - total_written, "%s", "(not defined)") - 1;
                fmt_ptr += 2;
                break;
            }
            total_written += written;
        }
        else
        {
            if (buffer != NULL)
            {
                // TODO bu burada olmamali ama bunsuz calismiyor
                *(buffer + size - 1) = '\0';
                *(buffer + total_written) = *fmt_ptr;
            }
            total_written += 1;
            fmt_ptr += 1;
        }
    }
    return total_written;
}

// _formatter yerine vsnprintf kullanıncada bişey olmuyo DisposableString sadece string pointerinden oluştuğundan
// yinede pointerı free lemek için _formatter lazıım
struct DisposableString format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    // Determine the required length
    int len = _formatter(NULL, 0, fmt, args);
    va_end(args);

    if (len < 0)
    {
        return (struct DisposableString){0};
    }

    // Allocate memory for the formatted string
    char *buffer = (char *)malloc(len + 1);
    if (!buffer)
    {
        return (struct DisposableString){0}; // Formatting error
    }

    va_start(args, fmt);
    _formatter(buffer, len + 1, fmt, args);
    va_end(args);

    return (struct DisposableString){.string = buffer}; // Formatting error
}
#endif