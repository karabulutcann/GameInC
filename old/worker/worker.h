#pragma once 
#include "core/worker/job.h"

struct Worker
{
    struct JobQueue jobQueue;
    Bool isRunning;
    Bool isWorking;
};

void workerCreate(struct Worker *dest);

void workerFunction(void* data);
