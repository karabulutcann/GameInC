#pragma once
#include "core/core.h"
#include "world/world.h"

//TODO hem mesh.h da hemde burda tanimliyosun duzelt
#ifndef TEMP_MESH_DEFINE
#define TEMP_MESH_DEFINE
#define TEMP_MESH_COUNT 3

#define TEMP_MESH_SIZE (CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * sizeof(f4) * CUBE_VERTEX_SIZE * 36)

struct TempMesh{
    Bool isFree;
    count_t totalWritten;
    f4* mesh;
};
#endif

struct ChunkGenerator{
    struct TempMesh tempMeshArr[TEMP_MESH_COUNT];
};


void chunkGeneratorCreate(struct ChunkGenerator *dest);
struct TempMesh* chunkGeneratorGetTempMesh(struct ChunkGenerator *self);

Bool chunkGeneratorGenerateMesh(struct ChunkGenerator *self,struct World* world, i4 chunkPos[2]);

void chunkGeneratorDestroy(struct ChunkGenerator *self);
