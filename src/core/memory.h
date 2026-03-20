#pragma once
#include "core/result.h"
#include "core/types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef u8* core_Memory;

Result core_memoryAllocate(core_Memory* dest, sizeT size){
    void* temp = malloc(size);
    if(temp == NULL){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    };
    *dest = (u8*)temp;
    return OK;
};

void core_memoryCopy(core_Memory src,core_Memory dest,sizeT size){
    assert(src != NULL && dest != NULL);
    for(indexT i =0;i<size;i++){
        dest[i] = src[i];
    }
}

void core_memoryFree(core_Memory src){
    free(src);
}
