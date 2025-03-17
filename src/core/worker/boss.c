#include "core/worker/thread.h"
#include "core/worker/boss.h"

static struct ThreadManager *staticThreadManager;

void bossCreate(struct Boss *dest)
{
    *dest = (struct Boss){0};
    dest->jobQueue = jobQueueCreate();

    if (staticThreadManager == NULL)
    {
        staticThreadManager = malloc(sizeof(struct ThreadManager));
        threadManagerCreate(staticThreadManager);
    }
}

struct Result bossHireWorker(struct Boss *self)
{
    if (self->workerCount == 3)
    {
        return mErr("Max worker count reached");
    }
    workerCreate(&self->workers[self->workerCount]);
    threadManagerSpawnThread(staticThreadManager, &self->workers[self->workerCount], workerFunction);
    self->workerCount++;
    return ok();
}

void bossAssignJob(struct Boss *self, struct Job* job)
{
    if (job->isRequireMainThread)
    {
        jobQueuePushToEnd(&self->jobQueue, job);
    }
    else
    {
        index_t workerIndex = 0;
        count_t leastJobCount = 0;
        for (count_t i = 0; i < self->workerCount; i++)
        {
            if (self->workers[i].jobQueue.count < leastJobCount)
            {
                leastJobCount = self->workers[i].jobQueue.count;
                workerIndex = i;
            }
        }
        jobQueuePushToEnd(&self->workers[workerIndex].jobQueue, job);
    }
}

void bossWaitForAllJobs(struct Boss *self)
{
    mDebug("Waiting for jobs to finish\n");
    Bool isAllJobsDone = FALSE;
    while (!isAllJobsDone)
    {
        isAllJobsDone = TRUE;
        for (count_t i = 0; i < self->workerCount; i++)
        {
            if (self->workers[i].jobQueue.count > 0)
            {
                isAllJobsDone = FALSE;
            }
        }
        Sleep(100);
    }
}

void bossDestroy(struct Boss *self)
{
}