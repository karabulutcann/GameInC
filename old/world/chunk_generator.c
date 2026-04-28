#include "world/chunk_generator.h"
#include "core/dynamic_array.h"
#include <memory.h>

void chunkGeneratorCreate(struct ChunkGenerator *dest){
    for(int i = 0; i < TEMP_MESH_COUNT; i++){
        dest->tempMeshArr[i].isFree = TRUE;
        dest->tempMeshArr[i].totalWritten = 0;
        dest->tempMeshArr[i].mesh = dynamicArrayCreate(DATA_TYPE_F4, TEMP_MESH_SIZE, sizeof(f4));
    }
}

struct TempMesh* chunkGeneratorGetTempMesh(struct ChunkGenerator *self){
    for (int i = 0; i < 3; i++)
    {
        if (self->tempMeshArr[i].isFree)
        {
            self->tempMeshArr[i].isFree = FALSE;
            self->tempMeshArr[i].totalWritten = 0;
            memset(self->tempMeshArr[i].mesh, 0, TEMP_MESH_SIZE);
            return (self->tempMeshArr + i);
            break;
        }
    }
    return NULL;
}

Bool chunkGeneratorGenerateMesh(struct ChunkGenerator *self, struct World* world, i4 chunkPos[2]){

     struct Chunk *chunk = chunkTableGet(world->chunkTable, chunkPos);
    if (chunk == NULL)
    {
        return FALSE;
    }

    struct TempMesh *tempMesh = chunkGeneratorGetTempMesh(self);
    if(tempMesh == NULL)
    {
        // mDebug("ChunkGenerator: No free temp meshes");
        return FALSE;
    }

    chunk->mesh->isLoading = TRUE;
    //TODO probably dont need this
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

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){chunkPos[0] * cubeSize * CHUNK_SIZE_X, 0.0f, chunkPos[1] * cubeSize * CHUNK_SIZE_Z});

    meshSetUniform(chunk->mesh, "model", sizeof(mat4), model);

    tempMesh->totalWritten = totalWritten;
    chunk->mesh->vertexCount = totalWritten;
    chunk->mesh->tempMesh = tempMesh;
    chunk->isLoading = FALSE;
    chunk->mesh->isCopied = FALSE;
    chunk->mesh->isLoading = FALSE;
    return TRUE;
}

void chunkGeneratorDestroy(struct ChunkGenerator *self){    
    
}