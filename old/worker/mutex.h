#pragma once
#include "core/core.h"
#include <windows.h>

struct Mutex
{
    CRITICAL_SECTION critSection;
    Bool isLocked;
    void *sharedState;
};

#define mMutexUse(mutex,data,dataType) \
    EnterCriticalSection(&mutex->critSection); \
    mutex->isLocked = TRUE; \
    dataType* data = (dataType *)mutex->sharedState; \
    for(u1 i = 0; i < 1;LeaveCriticalSection(&mutex->critSection),mutex->isLocked = FALSE , i++) 
