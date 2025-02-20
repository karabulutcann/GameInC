#pragma once
#include <malloc.h>
#include <stdlib.h>
#include "core.h"
#include "log.h"

// error handler
struct Result
{
    struct DisposableString errMessage;
    Bool success;
};

struct Result _err(struct DisposableString message);

struct Result _errFromStr(const char *str);

#define mErr(str) _Generic((str), \
    char *: _errFromStr,          \
    struct DisposableString: _err)(str)

struct Result ok();

#define FATAL 1

// Main macro that handles both cases
#define _CACHE_RESULT_W_OPTIONS(result, isFatal)                         \
    {                                                                    \
        struct Result tmpR = result;                                     \
        if (tmpR.success != 1)                                           \
        {                                                                \
            if (isFatal)                                                 \
            {                                                            \
                mUseDisposableString(tmpR.errMessage)                    \
                {                                                        \
                    mError("Fatal error: %s\n", tmpR.errMessage.string); \
                }                                                        \
                exit(1);                                                 \
            }                                                            \
            else                                                         \
            {                                                            \
                mUseDisposableString(tmpR.errMessage)                    \
                {                                                        \
                    mError(tmpR.errMessage.string);                      \
                }                                                        \
            }                                                            \
        }                                                                \
    }

// Overload macro: Default isFatal to 0
#define _CACHE_RESULT_DEFAULT(result) _CACHE_RESULT_W_OPTIONS(result, 0)

// Macro dispatcher to choose the correct version

#define CACHE_RESULT(...) GET_MACRO2(__VA_ARGS__, _CACHE_RESULT_W_OPTIONS, _CACHE_RESULT_DEFAULT)(__VA_ARGS__)
