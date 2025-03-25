#pragma once
#include "core.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

struct DisposableString
{
    char *string;
};

void checkDisposed(struct DisposableString disposableS);

#define mUseDisposableString(disposableS)                            \
    _Generic((disposableS),struct DisposableString: checkDisposed(disposableS),default: NULL);\
    for (char *tmpStr = _Generic((disposableS),struct DisposableString: disposableS.string ,default: NULL); tmpStr != NULL; free(tmpStr), tmpStr = NULL, disposableS.string = NULL)

#define mUseFormatted(string,formatStr,...)                             \
    struct DisposableString string = format(formatStr,##__VA_ARGS__);   \
    mUseDisposableString(string)