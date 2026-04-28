#include "core/worker/job.h"
#include "memory.h"


static count_t lastId = 0;

struct Job* jobCreate(JobFunc func, Allocated data, Bool isRequireMainThread, enum JobType type)
{
    struct Job *job = malloc(sizeof(struct Job));
    if (job == NULL)
    {
        return NULL;
    }
    memset(job, 0, sizeof(struct Job));
    job->func = func;
    job->data = data;
    job->isRequireMainThread = isRequireMainThread;
    job->type = type;
    job->id = lastId++;
    return job;
}

void jobExecute(struct Job *self)
{
    self->func(self->data);
}

void JobDestroy(struct Job *self){
    if(self != NULL){
        if(self->data != NULL){
            free(self->data);
        }
        free(self);
    }
}

struct JobQueue jobQueueCreate()
{
    struct JobQueue queue = {0};
    return queue;
}

void jobQueuePushToStart(struct JobQueue *self, struct Job *node)
{
    if (self->head == NULL)
    {
        self->head = node;
        self->tail = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        node->next = self->head;
        node->prev = NULL;
        self->head->prev = node;
        self->head = node;
    }
    self->count++;
}

void jobQueuePushToEnd(struct JobQueue *self, struct Job *node){
    if (self->head == NULL)
    {
        self->head = node;
        self->tail = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        node->next = NULL;
        node->prev = self->tail;
        self->tail->next = node;
        self->tail = node;
    }
    self->count++;
}

//returned value must be freed or pushed to the queue again
struct Job *jobQueuePopFromStart(struct JobQueue *self)
{
    if (self->head == NULL)
    {
        return NULL;
    }
    struct Job *node = self->head;
    if (self->count == 1)
    {
        self->head = NULL;
        self->tail = NULL;
    }else{
        self->head = self->head->next;
        if (self->count == 2)
        {
            self->tail = self->head;
        }
        self->head->prev = NULL;
    }
    self->count--;
    return node;
}

//returned value must be freed or pushed to the queue again
struct job *jobQueuePopFromEnd(struct JobQueue *self)
{
    if (self->tail == NULL)
    {
        return NULL;
    }
    struct Job *node = self->tail;
    if (self->count == 1)
    {
        self->head = NULL;
        self->tail = NULL;
    }
    self->tail = self->tail->prev;
    if (self->count == 2)
    {
        self->head = self->tail;
    }
    self->tail->next = NULL;
    self->count--;
    return node;
}

// removes and frees the first node in the queue
struct Result jobQueueRemoveFromStart(struct JobQueue *self){
    struct Job *node = jobQueuePopFromStart(self);
    JobDestroy(node);
    return ok();
}   

struct Result jobQueueRemove(struct JobQueue *self, id_t id)
{
    struct Job *node = self->head;
    while (node != NULL)
    {
        if (node->id == id)
        {
            struct Job *next = node->next;
            struct Job *prev = node->prev;
            if (prev != NULL)
            {
                prev->next = next;
            }
            if (next != NULL)
            {
                next->prev = prev;
            }
            if (self->head->id == id)
            {
                self->head = next;
            }
            if (self->tail->id == id)
            {
                self->tail = prev;
            }
            JobDestroy(node);
            self->count--;
            return ok();
        }
        node = node->next;
    }
    return mErr("No such job");
}
