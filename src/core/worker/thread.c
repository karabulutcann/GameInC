#include "thread.h"


DWORD __stdcall _threadFunction(LPVOID lpParam)
{
    struct ThreadData *threadData = (struct ThreadData *)lpParam;
    threadData->func(threadData->data);
}

struct Result threadManagerCreate(struct ThreadManager *dest)
{
    memset(dest, 0, sizeof(struct ThreadManager));
    return ok();
}

struct Result threadManagerSpawnThread(struct ThreadManager *self, void *data, ThreadFunctionT threadFunction)
{
    struct ThreadState *threadState = self->threadStates + self->threadCount;

    // Initialize thread state
    threadState->threadId = self->threadCount;
    threadState->eventHandle = CreateEvent(
        NULL,  // Default security attributes
        TRUE,  // Manual reset event
        FALSE, // Initial state is non-signaled
        NULL   // Unnamed event
    );

    threadState->data.eventHandle = threadState->eventHandle;
    threadState->data.func = threadFunction;
    threadState->data.data = data;


    threadState->handle = CreateThread(
        NULL,               // Default security attributes
        0,                  // Default stack size
        _threadFunction,     // Thread function
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

    HANDLE *threads = malloc(sizeof(HANDLE) * self->threadCount);
    memset(threads, 0, sizeof(HANDLE) * self->threadCount);

    for (index_t i = 0; i < self->threadCount; i++)
        threads[i] = self->threadStates[i].handle;

    if (WaitForMultipleObjects(self->threadCount, threads, TRUE, timeout) == WAIT_TIMEOUT)
    {
        return mErr("Timeout");
    }

    free(threads);
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
