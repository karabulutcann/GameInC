#include "core/worker/thread.h"
#include "core/worker/boss.h"


static struct ThreadManager* staticThreadManager = NULL;

void bossCreate(struct Boss *dest)
{
    *dest = (struct Boss){0};
    dest->jobQueue = jobQueueCreate();

    if (staticThreadManager == NULL)
    {
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

void bossAssignJob(struct Boss *self, struct Job job)
{
    if (job.isRequireMainThread)
    {
        jobQueuePushToEnd(&self->jobQueue, job);
    }
    else
    {
        count_t leastJobCount = 0;
        index_t workerIndex = 0;
        for (count_t i = 0; i < self->workerCount; i++)
        {
            if (self->workers[i].jobQueue.length < leastJobCount)
            {
                leastJobCount = self->workers[i].jobQueue.length;
                workerIndex = i;
            }
        }
        jobQueuePushToEnd(&self->workers[workerIndex].jobQueue, job);
    }
}

void bossDestroy(struct Boss *self)
{
}