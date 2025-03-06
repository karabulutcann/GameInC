#pragma once
#include "core/core.h"
#include "core/result.h"
#include <cglm/cglm.h>

struct Chunk{
    i4 position[2];
    Bool isVboCreated;
    Bool isLoading;
    u1* blockTypeArr;
    float* mesh;
    u4 vertexBufferObject;
    count_t vertexCount;
};

void chunkGenerateMesh(struct Chunk *self);

void chunkCreate(i4 chunkPos[2], struct Chunk *dest);

void chunkInitVbo(struct Chunk *self);

void chunkDestroy(struct Chunk* chunk);

typedef struct Chunk *ChunkTable;

struct ChunkTableMeta *chunkTableGetMeta(ChunkTable table);

ChunkTable chunkTableCreate(u4 length);

struct Result chunkTableInsert(ChunkTable table, i4 position[2], struct Chunk chunk);

void chunkTableRemove(ChunkTable table, i4 position[2]);

struct Chunk* chunkTableGet(ChunkTable table, long int position[2]);

void chunkTableDestroy(ChunkTable table);