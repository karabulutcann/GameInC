#pragma once
#include <windows.h>
#include "core/core.h"
#include "core/result.h"


// Constants for thread demo
#define MAX_THREADS 3

struct ThreadData
{
    u4 threadId;
    HANDLE eventHandle;
    void (*func)(void *);
    void* data;
};

struct ThreadState
{
    u4 threadId;
    HANDLE handle;
    HANDLE eventHandle;
    struct ThreadData data;
};

struct ThreadManager
{
    count_t threadCount;
    struct ThreadState threadStates[MAX_THREADS];
};

typedef void (*ThreadFunctionT)(void*);

// #define mThreadCreateFunc(funcName,threadData, FuncBody)             \
//     DWORD WINAPI funcName(LPVOID lpParam)                             \
//     {                                                                 \
//         struct ThreadData *threadData = (struct ThreadData *)lpParam; \
//         FuncBody                                                      \
//     }


struct Result threadManagerCreate(struct ThreadManager *dest);

struct Result threadManagerSpawnThread(struct ThreadManager *self, void *data, ThreadFunctionT threadFunction);

struct Result threadManagerWaitFor(struct ThreadManager *self,index_t threadId, DWORD timeout);

struct Result threadManagerWaitForAll(struct ThreadManager *self, DWORD timeout);

struct Result threadManagerCloseThread(struct ThreadManager *self, u4 threadId);

struct Result threadManagerDestroy(struct ThreadManager *self);
