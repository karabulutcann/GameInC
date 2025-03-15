#pragma once
#include "core/core.h"
#include "world/chunk.h"
#include "FastNoiseLight/FastNoiseLite.h"

// 10 dan fazla iken zorlaniyo

enum BlockType
{
    AIR,
    DIRT,
    STONE,
    GRASS,
    OAK_PLANK,
    OAK_LOG,
    CRAFTING_TABLE,
    STONE_BRICK,
    MOSSY_STONE_BRICK,
    CRACKED_STONE_BRICK,
};

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

Bool staticWorldLoadChunk(i4 chunkPos[2]);

// void worldGenerateChunkMesh(struct World *self, i4 chunkPos[2]);

count_t blockGenerateMesh(u2 face, u1 blockType, vec3 position, float *mesh);

void worldGetBlockNeighbors(struct World *self, index_t i, struct Chunk currentChunk, i4 chunkCoords[2], u4 blockCoords[3], char neighborTypes[6]);

void worldDestroy(struct World *self);
