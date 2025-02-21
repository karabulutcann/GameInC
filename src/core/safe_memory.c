#include "core/core.h"
#include "malloc.h"

struct FreeList{
    void* pMemory;
    size_t blockSize;
    struct FreeList* next;
};

void freeListAdd(struct FreeList* pFreeList, void* pMemory, size_t blockSize)
{
    struct FreeList* pNewFreeList = safeMemoryAlloc(sizeof(struct FreeList));
    pNewFreeList->pMemory = pMemory;
    pNewFreeList->blockSize = blockSize;
    pNewFreeList->next = pFreeList;
}

static void* staticMemory = NULL;
static size_t staticMemorySize = 0;

static struct FreeList freeList = {0};

void safeMemoryZero(void* pMemory, size_t size)
{
    if(pMemory == NULL)
        return;

    for(size_t i = 0; i < size; i++)
        ((char*)pMemory)[i] = 0;
}


void safeMemoryInit(size_t allocationSize)
{
    staticMemory = malloc(allocationSize);
    staticMemorySize = allocationSize;
    safeMemoryZero(staticMemory, allocationSize);

    freeList.pMemory = staticMemory;
    freeList.blockSize = allocationSize;
}

void* safeMemoryAlloc(size_t size)
{
    if(size == 0)
        return NULL;

    if(size > staticMemorySize)
        return NULL;

    struct FreeList* pFreeList = &freeList;

    while(pFreeList->next != NULL)
    {
        pFreeList = pFreeList->next;
        if(pFreeList->blockSize >= size)
            break;
    }

    if(pFreeList->blockSize < size)
        return NULL;

    void* pMemory = pFreeList->pMemory;
    pFreeList->pMemory = ((char*)pFreeList->pMemory) + size;
    pFreeList->blockSize -= size;

    return pMemory;
}

void safeMemoryFree()
{
    free(staticMemory);
    staticMemory = NULL;
    staticMemorySize = 0;
}




