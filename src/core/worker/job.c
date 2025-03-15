#include "core/worker/job.h"
#include "memory.h"

void jobCreate(void (*func)(void *), void *data, struct Job *dest)
{
    dest->func = func;
    dest->data = data;
}

void jobExecute(struct Job *self)
{
    self->func(self->data);
}

struct JobQueueNode *jobQueueNodeCreate(struct Job job, id_t id)
{
    struct JobQueueNode *node = malloc(sizeof(struct JobQueueNode));
    memset(node, 0, sizeof(struct JobQueueNode));
    node->id = id;
    node->job = job;
    return node;
}

struct JobQueue jobQueueCreate()
{
    struct JobQueue queue = {0};
    return queue;
}

void jobQueuePushToStart(struct JobQueue *self, struct Job job)
{
    struct JobQueueNode *node = jobQueueNodeCreate(job, self->lastId++);
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
    self->length++;
}

void jobQueuePushNodeToEnd(struct JobQueue *self, struct JobQueueNode *node){
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
    self->length++;
}

void jobQueuePushToEnd(struct JobQueue *self, struct Job job)
{
    struct JobQueueNode *node = jobQueueNodeCreate(job, self->lastId++);
    jobQueuePushNodeToEnd(self, node);
}

//returned value must be freed or pushed to the queue again
struct JobQueueNode *jobQueuePopFromStart(struct JobQueue *self)
{
    if (self->head == NULL)
    {
        return;
    }
    struct JobQueueNode *node = self->head;
    if (self->length == 1)
    {
        self->head = NULL;
        self->tail = NULL;
    }else{
        self->head = self->head->next;
        if (self->length == 2)
        {
            self->tail = self->head;
        }
        self->head->prev = NULL;
    }
    self->length--;
    return node;
}

//returned value must be freed or pushed to the queue again
struct JobQueueNode *jobQueuePopFromEnd(struct JobQueue *self)
{

    if (self->tail == NULL)
    {
        return NULL;
    }
    struct JobQueueNode *node = self->tail;
    if (self->length == 1)
    {
        self->head = NULL;
        self->tail = NULL;
    }
    self->tail = self->tail->prev;
    if (self->length == 2)
    {
        self->head = self->tail;
    }
    self->tail->next = NULL;
    self->length--;

    return node;
}

// removes and frees the first node in the queue
struct Result jobQueueRemoveFromStart(struct JobQueue *self){
    struct JobQueueNode *node = jobQueuePopFromStart(self);
    if(node != NULL){
        free(node);
        return ok();
    }
}   

struct Result jobQueueRemove(struct JobQueue *self, id_t id)
{
    struct JobQueueNode *node = self->head;
    while (node != NULL)
    {
        if (node->id == id)
        {
            struct JobQueueNode *next = node->next;
            struct JobQueueNode *prev = node->prev;
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
            free(node);
            self->length--;
            return ok();
        }
        node = node->next;
    }
    return mErr("No such job");
}
