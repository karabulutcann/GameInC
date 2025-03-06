#include "core/core.h"
#include "core/thread.h"


struct Job
{
    void (*func)(void *);
    void *data;
    Bool isRequireMainThread;
};

void jobCreate(void (*func)(void *), void *data, struct Job *dest)
{
    dest->func = func;
    dest->data = data;
}

void jobExecute(struct Job *self)
{
    self->func(self->data);
}

 

struct Worker
{
    struct Job jobQueue[20];
    Bool isRunning;
};

// mThreadCreateFunc(workerThread, threadData,
//                   {
//                       struct Worker *worker = (struct Worker *)threadData->data;
//                       waitForSingleObject(threadData->eventHandle, INFINITE);
//                       while (worker->isRunning)
//                       {

//                       } 
//                   })


struct Boss
{
    struct ThreadManager threadManager;
    struct Worker workers[3];
    struct Job jobQueue[20];
};

void bossCreate(struct Boss *dest)
{
    
}

void bossHireWorker(struct Boss *self, struct Worker *worker)
{
    
}

void bossAssignJob(struct Boss *self, struct Job *job)
{
    if(job->isRequireMainThread)
    {
        
    }else{

    }
}

void bossDestroy(struct Boss *self)
{
    
}   