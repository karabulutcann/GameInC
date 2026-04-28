#include "core/worker/worker.h"
#include "world/world.h"
#include "world/chunk_generator.h"

void workerCreate(struct Worker *dest)
{
    *dest = (struct Worker){0};
    dest->jobQueue = jobQueueCreate();
    dest->isRunning = TRUE;
}

void workerFunction(void *data)
{
    struct Worker* self = data;
    struct ChunkGenerator chunkGenerator = {0};
    chunkGeneratorCreate(&chunkGenerator);
    Bool isJobDone = FALSE;
    while (self->isRunning)
    {
        if (self->jobQueue.count > 0)
        {
            isJobDone = FALSE;
            switch (self->jobQueue.head->type)
            {

            case GENERATE_MESH:
                struct World *world = worldGet();
                isJobDone = chunkGeneratorGenerateMesh(&chunkGenerator, world, self->jobQueue.head->data);
                break;
            case LOAD_CHUNK:
            default:
                isJobDone = self->jobQueue.head->func(self->jobQueue.head->data);
                break;
            }
            // TODO if job is not done push the job to the end of the queue
            if (isJobDone)
            {
                jobQueueRemoveFromStart(&self->jobQueue);
            }
            else
            {
                jobQueuePushToEnd(&self->jobQueue, jobQueuePopFromStart(&self->jobQueue));
            }
        }
    }
}
