#pragma once
#include "core/core.h"
#include <cglm/cglm.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 16

#define CUBE_VERTEX_SIZE 8

struct Chunk{
    i4 position[2];
    u1* blockTypeArr;
    float* mesh;
    u4 vertexBufferObject;
    count_t vertexCount;
};

void chunkGenerateMesh(struct Chunk *self);

void chunkCreate(i4 chunkPos[2], struct Chunk *dest);

void chunkDestroy(struct Chunk chunk);

typedef struct Chunk *ChunkTable;

struct ChunkTableMeta *chunkTableGetMeta(ChunkTable table);

ChunkTable chunkTableCreate(u4 length);

void chunkTableInsert(ChunkTable table, i4 position[2], struct Chunk chunk);

void chunkTableRemove(ChunkTable table, i4 position[2]);

struct Chunk* chunkTableGet(ChunkTable table, long int position[2]);

void chunkTableDestroy(ChunkTable table);