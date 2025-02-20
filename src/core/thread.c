#include "thread.h"

struct Result threadManagerCreate(struct ThreadManager *dest)
{
    memset(dest, 0, sizeof(struct ThreadManager));
    return ok();
}

struct Result threadManagerSpawnThread(struct ThreadManager *self, struct Mutex *mutexData, ThreadFunction threadFunction)
{
    struct ThreadState *threadState = self->threadStates + self->threadCount;

    // Initialize thread state
    threadState->threadId = self->threadCount + 1;
    threadState->eventHandle = CreateEvent(
        NULL,  // Default security attributes
        TRUE,  // Manual reset event
        FALSE, // Initial state is non-signaled
        NULL   // Unnamed event
    );

    threadState->data.eventHandle = threadState->eventHandle;
    threadState->data.mutex = mutexData;

    threadState->handle = CreateThread(
        NULL,               // Default security attributes
        0,                  // Default stack size
        threadFunction,     // Thread function
        &threadState->data, // Thread argument
        0,                  // Default creation flags
        NULL                // Thread identifier
    );

    if (threadState->handle == NULL)
    {
        CloseHandle(threadState->eventHandle);
        memset(threadState, 0, sizeof(struct ThreadState));
        return mErr("Failed to create thread");
    }

    self->threadCount++;

    return ok();
}

struct Result threadManagerWaitFor(struct ThreadManager *self, index_t threadId, DWORD timeout)
{
    if (self->threadCount == 0)
        return mErr("No threads");

    if (self->threadStates[threadId].handle == NULL)
        return mErr("Thread not found");

    if (WaitForSingleObject(self->threadStates[threadId].handle, timeout) == WAIT_TIMEOUT)
    {
        return mErr("Timeout");
    }

    return ok();
}

struct Result threadManagerWaitForAll(struct ThreadManager *self, DWORD timeout)
{
    if (self->threadCount == 0)
        return mErr("No threads");

    HANDLE threads[self->threadCount];
    memset(threads, 0, sizeof(HANDLE) * self->threadCount);

    for (index_t i = 0; i < self->threadCount; i++)
        threads[i] = self->threadStates[i].handle;

    if (WaitForMultipleObjects(self->threadCount, threads, TRUE, timeout) == WAIT_TIMEOUT)
    {
        return mErr("Timeout");
    }

    return ok();
}

struct Result threadManagerCloseThread(struct ThreadManager *self, u4 threadId)
{
    if (self->threadStates[threadId].handle)
    {
        CloseHandle(self->threadStates[threadId].handle);
        CloseHandle(self->threadStates[threadId].eventHandle);
        memset(&self->threadStates[threadId], 0, sizeof(struct ThreadState));
    }
    else
    {
        return mErr("No such thread");
    }

    return ok();
}

struct Result threadManagerDestroy(struct ThreadManager *self)
{
    for (u4 i = 0; i < self->threadCount; i++)
    {
        CACHE_RESULT(threadManagerCloseThread(self, i));
    }
    return ok();
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    THREAD_DATA *pData = (THREAD_DATA *)lpParam;

    printf("Thread %d waiting for start signal...\n", pData->threadId);

    // Wait for start signal
    WaitForSingleObject(pData->eventHandle, INFINITE);

    printf("Thread %d started working\n", pData->threadId);

    // Simulate work with multiple iterations
    for (int i = 0; i < 5; i++)
    {
        // Enter critical section
        EnterCriticalSection(pData->critSection);

        // Access shared resource
        (*pData->sharedCounter)++;
        printf("Thread %d: incremented counter to %d\n",
               pData->threadId, *pData->sharedCounter);

        // Leave critical section
        LeaveCriticalSection(pData->critSection);

        // Simulate some work
        Sleep(1000);
    }

    printf("Thread %d finished\n", pData->threadId);
    return 0;
}

void ErrorHandler(LPCSTR message)
{
    DWORD errorCode = GetLastError();
    LPSTR errorMessage = NULL;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&errorMessage,
        0,
        NULL);

    printf("%s: %s\n", message, errorMessage);
    LocalFree(errorMessage);
}
