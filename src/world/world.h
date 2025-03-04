#pragma once
#include "core/core.h"
#include "chunk.h"
#include "FastNoiseLight/FastNoiseLite.h"

// 10 dan fazla iken zorlaniyo
#define WORLD_SIZE_X 60


#define cubeSize 0.4f

struct World
{
    ChunkTable chunkTable;
};

void worldCreate(struct World *dest);

void worldLoadChunk(struct World *self, i4 chunkPos[2]);

void worldUnloadChunk(struct World *self, i4 chunkPos[2]);

void worldGenerateChunkMesh(struct World *self, i4 chunkPos[2]);

void worldDestroy(struct World *self);
