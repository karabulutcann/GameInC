#pragma once
#include "core.h"
#include "result.h"
#include "log.h"
#include <windows.h>
#include <stdio.h>

// Constants for thread demo
#define MAX_THREADS 3
#define BUFFER_SIZE 100


struct Mutex
{
    CRITICAL_SECTION critSection;
    void *sharedState;
};

#define mMutexUse(mutex,data,dataType) \
    EnterCriticalSection(&mutex->critSection); \
    dataType* data = (dataType *)mutex->sharedState; \ 
    for(u1 i = 0; i < 1; i++, LeaveCriticalSection(&mutex->critSection)) \

struct ThreadData
{
    u4 threadId;
    HANDLE eventHandle;
    struct Mutex *mutex;
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

typedef DWORD (*ThreadFunction)(LPVOID);

#define mThreadCreateFunc(funcName,threadData, FuncBody)             \
    DWORD WINAPI funcName(LPVOID lpParam)                             \
    {                                                                 \
        struct ThreadData *threadData = (struct ThreadData *)lpParam; \
        FuncBody                                                      \
    }

// Structure to pass data to threads
typedef struct {
    int threadId;
    HANDLE eventHandle;
    CRITICAL_SECTION* critSection;
    int* sharedCounter;
} THREAD_DATA;


struct Result threadManagerCreate(struct ThreadManager *dest);

struct Result threadManagerSpawnThread(struct ThreadManager *self, struct Mutex* mutexData, ThreadFunction threadFunction);

struct Result threadManagerWaitFor(struct ThreadManager *self,index_t threadId, DWORD timeout);

struct Result threadManagerWaitForAll(struct ThreadManager *self, DWORD timeout);

struct Result threadManagerCloseThread(struct ThreadManager *self, u4 threadId);

struct Result threadManagerDestroy(struct ThreadManager *self);

DWORD WINAPI WorkerThread(LPVOID lpParam);

void ErrorHandler(LPCSTR message);