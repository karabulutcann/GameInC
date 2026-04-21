#pragma once
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, fmt, ...)                                         \
    do {                                                               \
        if (!(cond)) {                                                 \
            fprintf(stderr, "[FATAL] " fmt "\n  cond: %s\n  func: %s\n  %s:%d\n", \
                    ##__VA_ARGS__, #cond, __func__, __FILE__, __LINE__); \
            abort();                                                   \
        }                                                              \
    } while(0)

#define PANIC(fmt, ...)                                                \
    do {                                                               \
        fprintf(stderr, "[PANIC] " fmt "\n  func: %s\n  %s:%d\n",    \
                ##__VA_ARGS__, __func__, __FILE__, __LINE__);          \
        abort();                                                       \
    } while(0)
