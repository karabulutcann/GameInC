#pragma once
#include "core/core.h"
#include "chunk.h"

#define WORLD_SIZE_X 15

struct World
{
    ChunkTable chunkTable;
};

void worldCreate(struct World *dest);

void worldGenerateChunkMesh(struct World *self, i4 chunkPos[2]);

void worldDestroy(struct World *self);
