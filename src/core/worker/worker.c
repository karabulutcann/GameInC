#include "core/worker/worker.h"
#include "world/chunk_generator.h"

void workerCreate(struct Worker *dest)
{
    *dest = (struct Worker){0};
    dest->jobQueue = jobQueueCreate();
    dest->isRunning = TRUE;
}

void workerFunction(struct Worker *self){
    struct ChunkGenerator chunkGenerator = {0};
    chunkGeneratorCreate(&chunkGenerator);
    mDebug("ChunkGenerator: Loading dddd");
    while (self->isRunning)
    {
        Bool isJobDone = FALSE;
        switch (self->jobQueue.head->job.type)
        {
        case LOAD_CHUNK:
            struct World *world = worldGet();
            mDebug("ChunkGenerator: Loading chunk");
            chunkGeneratorGenerateMesh(&chunkGenerator, world, self->jobQueue.head->job.data);
            isJobDone = TRUE;
            break;
        case GENERATE_MESH:
            break;
        default:
            isJobDone = self->jobQueue.head->job.func(self->jobQueue.head->job.data);
            break;
        }
        // TODO if job is not done push the job to the end of the queue
        if (isJobDone)
        {
            jobQueuePopFromStart(&self->jobQueue);
        }
    }
}

