#pragma once
#include "core/core.h"
#include "core/result.h"

enum JobType
{
    DEFAULT,
    LOAD_CHUNK,
    GENERATE_MESH,
};

typedef Bool (*JobFunc)(Allocated);

struct Job
{
    id_t id;
    struct Job* next;
    struct Job* prev;
    JobFunc func;
    Allocated data;
    enum JobType type;
    Bool isRequireMainThread;
};

struct Job* jobCreate(JobFunc func, Allocated data, Bool isRequireMainThread, enum JobType type);

void jobExecute(struct Job *self);

struct JobQueue
{
    struct Job *head;
    struct Job *tail;
    count_t count;
};

struct JobQueueNode *jobQueueNodeCreate(struct Job job, id_t id);

struct JobQueue jobQueueCreate();

void jobQueuePushToStart(struct JobQueue *self, struct Job *node);

void jobQueuePushToEnd(struct JobQueue *self, struct Job *node);

struct Job *jobQueuePopFromStart(struct JobQueue *self);

struct job *jobQueuePopFromEnd(struct JobQueue *self);

struct Result jobQueueRemoveFromStart(struct JobQueue *self);

struct Result jobQueueRemove(struct JobQueue *self, id_t id);
