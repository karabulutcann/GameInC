#include "cglm/types.h"
#include "core/types.h"
#include "game/world/chunk.h"

struct ChunkNode{
    struct Chunk* chunk;
    struct ChunkNode* next;
};

struct ChunkMap{
    struct ChunkNode** chunks;
    capacityT capacity;
    countT count;
};

void chunkMapCreate();
void chunkMapSet();
void chunkMapGet();
void chunkMapRemove();
void chunkMapDestroy();
