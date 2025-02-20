#include "core/core.h"
#include "core/thread.h"
#include "core/result.h"



mThreadCreateFunc(threadFunction, threadData, {
    printf("Thread %d waiting for start signal...\n", threadData->threadId);
    mMutexUse(threadData->mutex, counter, int)
    {
        *counter += 1;
        printf("Thread %d incrementing counter to %d\n", threadData->threadId, *counter);
    }
    return 0;
})

int main()
{
    struct ThreadManager threadManager = {0};
    threadManagerCreate(&threadManager);

    struct Mutex mutex = {0};
    InitializeCriticalSection(&mutex.critSection);

    int counter = 0;

    mutex.sharedState = &counter;

    CACHE_RESULT(threadManagerSpawnThread(&threadManager, &mutex, threadFunction));

    CACHE_RESULT(threadManagerWaitForAll(&threadManager, INFINITE));

    printf("Counter: %d\n", counter);

    threadManagerDestroy(&threadManager);
    return 0;
}
