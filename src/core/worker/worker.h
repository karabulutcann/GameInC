#pragma once 
#include "core/worker/job.h"

struct Worker
{
    struct JobQueue jobQueue;
    Bool isRunning;
};

void workerCreate(struct Worker *dest);

void workerFunction(struct Worker *self);