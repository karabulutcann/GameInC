#include "dynamic_array.h"
#include "core/core.h"
#include "core/log.h"
#include <memory.h>
#include <malloc.h>

DynamicArray dynamicArrayCreate(enum DataType type, count_t capacity, size_t stride)
{
    DynamicArray arr = malloc(sizeof(struct DynamicArrayMeta) + capacity * stride);
    memset(arr, 0, sizeof(struct DynamicArrayMeta) + capacity * stride);
    struct DynamicArrayMeta *meta = arr;
    meta->capacity = capacity;
    meta->count = 0;
    meta->stride = stride;
    return (DynamicArray)((u1 *)arr + sizeof(struct DynamicArrayMeta));
}

struct DynamicArrayMeta *dynamicArrayGetMeta(DynamicArray arr)
{
    return (struct DynamicArrayMeta *)((u1 *)arr - sizeof(struct DynamicArrayMeta));
}

void* dynamicArrayGet(DynamicArray arr, index_t index){
    struct DynamicArrayMeta *meta = dynamicArrayGetMeta(arr);
    return (u1 *)arr + meta->stride * index;
}

//TODO resize yapmiyosun duzelt
void dynamicArrayPush(DynamicArray arr, void *data)
{
    struct DynamicArrayMeta *meta = dynamicArrayGetMeta(arr);
    if (meta->count == meta->capacity)
    {
        return;
    }
    memcpy((u1 *)arr + meta->stride * meta->count, data, meta->stride);
    meta->count++;
}

void dynamicArrayPop(DynamicArray arr)
{
    struct DynamicArrayMeta *meta = dynamicArrayGetMeta(arr);
    if (meta->count == 0)
    {
        return;
    }
    meta->count--;
}

void dynamicArrayDestroy(DynamicArray arr)
{
    mDebug("darray destroyed\n");
    free((u1 *)arr - sizeof(struct DynamicArrayMeta));
}