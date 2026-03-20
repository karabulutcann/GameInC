#pragma once
#include "core/types.h"
#include "core/memory.h"

struct core_ArrayMeta{
    u64 stride;
    capacityT capacity;
    lengthT length;
};

typedef void* core_Array;

void core_arrayGetMeta();
void core_arrayCreate(core_Array* dest,u64 stride,capacityT capacity){
    core_Memory temp;
    core_memoryAllocate(&temp,sizeof(struct core_ArrayMeta) + stride * capacity);
    struct core_ArrayMeta meta = {
        .stride = stride,
        .capacity = capacity,
        .length = 0
    };
    core_memoryCopy(temp,(u8*)&meta,sizeof(meta));
    *dest = temp + sizeof(meta);
};
void core_arrayDelete(core_Array src){
    core_memoryFree(((u8*)src) - sizeof(struct core_ArrayMeta));
}
void core_arrayPush();
void core_arrayPop();
void core_arrayLength();
void core_arrayResize();
void core_arrayAt();
