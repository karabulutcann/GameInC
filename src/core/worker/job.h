#pragma once
#include "core/core.h"
#include "core/result.h"

enum JobType
{
    DEFAULT,
    LOAD_CHUNK,
    GENERATE_MESH,
};

struct Job
{
    Bool (*func)(void *);
    void *data;
    enum JobType type;
    Bool isRequireMainThread;
};

void jobCreate(void (*func)(void *), void *data, struct Job *dest);

void jobExecute(struct Job *self);

struct JobQueueNode
{
    struct JobQueueNode *next;
    struct JobQueueNode *prev;
    struct Job job;
    id_t id;
};

struct JobQueue
{
    struct JobQueueNode *head;
    struct JobQueueNode *tail;
    count_t length;
    id_t lastId;
};

struct JobQueueNode *jobQueueNodeCreate(struct Job job, id_t id);

struct JobQueue jobQueueCreate();

void jobQueuePushToStart(struct JobQueue *self, struct Job job);

void jobQueuePushToEnd(struct JobQueue *self, struct Job job);

void jobQueuePushNodeToEnd(struct JobQueue *self, struct JobQueueNode *node);

struct JobQueueNode* jobQueuePopFromStart(struct JobQueue *self);

struct JobQueueNode* jobQueuePopFromEnd(struct JobQueue *self);

struct Result jobQueueRemoveFromStart(struct JobQueue *self);

struct Result jobQueueRemove(struct JobQueue *self, id_t id);