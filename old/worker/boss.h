#pragma once
#include "core/core.h"
#include "core/result.h"
#include "core/worker/job.h"
#include "core/worker/worker.h"

struct Boss
{
    struct Worker workers[3];
    count_t workerCount;
    struct JobQueue jobQueue;
};

void bossCreate(struct Boss *dest);

struct Result bossHireWorker(struct Boss *self);

void bossAssignJob(struct Boss *self, struct Job* job);

void bossWaitForAllJobs(struct Boss *self);

void bossDestroy(struct Boss *self);