#pragma once
#include "core/core.h"
#include "core/data_types.h"

struct DynamicArrayMeta
{
    count_t capacity;
    count_t count;
    size_t stride;
};

typedef void *DynamicArray;

DynamicArray dynamicArrayCreate(enum DataType type, count_t capacity, size_t stride);

struct DynamicArrayMeta *dynamicArrayGetMeta(DynamicArray arr);

void* dynamicArrayGet(DynamicArray arr, index_t index);

//TODO resize yapmiyosun duzelt
void dynamicArrayPush(DynamicArray arr, void *data);

void dynamicArrayPop(DynamicArray arr);

void dynamicArrayDestroy(DynamicArray arr);