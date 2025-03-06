#include "core/core.h"
#include "world/chunk.h"
#include "world/world.h"


#define TEMP_MESH_SIZE (CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * sizeof(f4) * CUBE_VERTEX_SIZE * 36)

struct TempMesh{
    Bool isFree;
    f4 mesh[TEMP_MESH_SIZE];
};

struct ChunkGenerator{
    struct TempMesh tempMeshArr[3];
};

void chunkGeneratorCreate(struct ChunkGenerator *dest){
    
}

struct TempMesh* chunkGeneratorGetTempMesh(struct ChunkGenerator *self){
    for (int i = 0; i < 3; i++)
    {
        if (self->tempMeshArr[i].isFree)
        {
            self->tempMeshArr[i].isFree = FALSE;
            for(int j = 0; j < TEMP_MESH_SIZE; j++)
            {
                self->tempMeshArr[i].mesh[j] = 0.0f;
            }
            return &self->tempMeshArr[i];
            break;
        }
    }
    return NULL;
}

void chunkGeneratorGenerateMesh(struct ChunkGenerator *self,struct World* world, i4 chunkPos[2]){

    struct Chunk *chunk = chunkTableGet(world->chunkTable, chunkPos);
    if (chunk == NULL)
    {
        return;
    }

    struct TempMesh *tempMesh = chunkGeneratorGetTempMesh(self);
    if(tempMesh == NULL)
    {
        mDebug("ChunkGenerator: No free temp meshes");
        return;
    }

    chunk->isLoading = TRUE;
    count_t totalWritten = 0;

    for (int i = 0; i < CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y; i++)
    {
        if (chunk->blockTypeArr[i] != AIR)
        {
            u4 coords[3] = {i % CHUNK_SIZE_X, i / CHUNK_SIZE_X / CHUNK_SIZE_Z, i / CHUNK_SIZE_X % CHUNK_SIZE_Z};
            vec3 position = {(coords[0]) * cubeSize, (coords[1]) * cubeSize, (coords[2]) * cubeSize};
            char neighborTypes[6] = {0};
            worldGetBlockNeighbors(world, i, *chunk, chunkPos, coords, neighborTypes);
            for (int j = 0; j < 6; j++)
            {
                if (neighborTypes[j] == AIR)
                {
                    totalWritten += blockGenerateMesh(j, chunk->blockTypeArr[i], position, tempMesh->mesh + totalWritten);
                }
            }
        }
    }
    chunk->vertexCount = totalWritten;
    chunk->isLoading = FALSE;
}

void chunkGeneratorDestroy(struct ChunkGenerator *self){    
    
}