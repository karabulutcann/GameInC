#pragma once
#include "core/core.h"
#include "world/chunk.h"
#include "FastNoiseLight/FastNoiseLite.h"

enum Face
{
    BACK,
    FRONT,
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
};

struct World
{
    ChunkTable chunkTable;
};

struct World *worldGet();

void worldCreate(struct World *dest);

void worldLoadChunk(struct World *self, i4 chunkPos[2]);

void worldUnloadChunk(struct World *self, i4 chunkPos[2]);

Bool staticWorldLoadChunk(void* chunkPos);

// void worldGenerateChunkMesh(struct World *self, i4 chunkPos[2]);

u4 staticWorldGetBlock(i4 worldPos[3]);

Bool staticWorldSetBlock(i4 worldPos[3],u4 blockType);

count_t blockGenerateMesh(u2 face, u1 blockType, vec3 position, float *mesh);

void worldGetBlockNeighbors(struct World *self, index_t i, struct Chunk currentChunk, i4 chunkCoords[2], u4 blockCoords[3], char neighborTypes[6]);

void worldDestroy(struct World *self);
