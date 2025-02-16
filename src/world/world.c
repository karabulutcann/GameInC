#include "world.h"
#include "memory.h"

#define size 0.2f
#define cubeSize 0.4f

const float cubeVertices[] = {
    // Back face
    -size, -size, -size, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left
    size, size, -size, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top right
    size, -size, -size, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom right
    size, size, -size, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top right
    -size, -size, -size, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left
    -size, size, -size, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top left

    // front face
    -size, -size, size, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    size, -size, size, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
    size, size, size, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right
    size, size, size, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right
    -size, size, size, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top left
    -size, -size, size, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left

    // left face
    -size, size, size, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
    -size, size, -size, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
    -size, -size, -size, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -size, -size, -size, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -size, -size, size, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -size, size, size, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right

    // right face
    size, size, size, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
    size, -size, -size, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    size, size, -size, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
    size, -size, -size, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    size, size, size, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
    size, -size, size, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

    // bottom face
    -size, -size, -size, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    size, -size, -size, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
    size, -size, size, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    size, -size, size, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    -size, -size, size, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -size, -size, -size, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right

    // top face
    -size, size, -size, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    size, size, size, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    size, size, -size, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    size, size, size, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    -size, size, -size, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    -size, size, size, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
};

#define AIR 0

enum Face
{
    BACK,
    FRONT,
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
};



void worldCreate(struct World *dest)
{
    dest->chunkTable = chunkTableCreate(WORLD_SIZE_X * WORLD_SIZE_X);
    for (int i = 0; i < WORLD_SIZE_X * WORLD_SIZE_X; i++)
    {
        chunkCreate((u2[2]){i % WORLD_SIZE_X, i / WORLD_SIZE_X}, &dest->chunkTable[i]);
    }
    for(int i = 0; i < WORLD_SIZE_X * WORLD_SIZE_X; i++){
        worldGenerateChunkMesh(dest, (u2[2]){i % WORLD_SIZE_X, i / WORLD_SIZE_X});
    }
}

void worldGetBlockNeighbors(struct World *self, count_t i, struct Chunk currentChunk, i4 chunkCoords[2], count_t blockCoords[3], char neighborTypes[6])
{
    // Right face
    if ((blockCoords[0] + 1) < CHUNK_SIZE_X)
    {
        neighborTypes[RIGHT] = currentChunk.blockTypeArr[i + 1];
    }
    else
    {
        i4 rightChunkCoords[2] = {chunkCoords[0] + 1, chunkCoords[1]};
        struct Chunk* rightChunk = chunkTableGet(self->chunkTable, rightChunkCoords);
        if(rightChunk != NULL){
            neighborTypes[RIGHT] = rightChunk->blockTypeArr[blockCoords[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z + blockCoords[2] * CHUNK_SIZE_X];
        }
        else{
            neighborTypes[RIGHT] = AIR;
        }
    }

    // Left face
    if (blockCoords[0] > 0)
    {
        neighborTypes[LEFT] = currentChunk.blockTypeArr[i - 1];
    }
    else
    {
        i4 leftChunkCoords[2] = {chunkCoords[0] - 1, chunkCoords[1]};
        struct Chunk* leftChunk = chunkTableGet(self->chunkTable, leftChunkCoords);
        if(leftChunk != NULL){
            neighborTypes[LEFT] = leftChunk->blockTypeArr[blockCoords[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z + (blockCoords[2] + 1) * CHUNK_SIZE_X - 1];
        }
        else{
            neighborTypes[LEFT] = AIR;
        }
    }

    // Back face
    if (blockCoords[2] > 0)
    {
        neighborTypes[BACK] = currentChunk.blockTypeArr[i - CHUNK_SIZE_X];
    }
    else
    {
        i4 backChunkCoords[2] = {chunkCoords[0], chunkCoords[1] - 1};
        struct Chunk* backChunk = chunkTableGet(self->chunkTable, backChunkCoords);
        if(backChunk != NULL){
            neighborTypes[BACK] = backChunk->blockTypeArr[(blockCoords[1] + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Z - CHUNK_SIZE_X + blockCoords[0]];   
        }
        else{
            neighborTypes[BACK] = AIR;
        }
    }

    // Front face
    if (blockCoords[2] + 1 < CHUNK_SIZE_Z)
    {
        neighborTypes[FRONT] = currentChunk.blockTypeArr[i + CHUNK_SIZE_X];
    }
    else
    {
        i4 frontChunkCoords[2] = {chunkCoords[0], chunkCoords[1] + 1};
        struct Chunk* frontChunk = chunkTableGet(self->chunkTable, frontChunkCoords);
        if(frontChunk != NULL){
            neighborTypes[FRONT] = frontChunk->blockTypeArr[(blockCoords[1]) * CHUNK_SIZE_X * CHUNK_SIZE_Z + blockCoords[0]];
        }
        else{
            neighborTypes[FRONT] = AIR;
        }
    }

    // Bottom face
    if (blockCoords[1] > 0)
    {
        neighborTypes[BOTTOM] = currentChunk.blockTypeArr[i - CHUNK_SIZE_X * CHUNK_SIZE_Z];
    }
    else
    {
        neighborTypes[BOTTOM] = 1;
    }

    // Top face
    if (blockCoords[1] + 1 < CHUNK_SIZE_Y)
    {
        neighborTypes[TOP] = currentChunk.blockTypeArr[i + CHUNK_SIZE_X * CHUNK_SIZE_Z];
    }
    else
    {
        neighborTypes[TOP] = AIR;
    }
}

count_t blockGenerateMesh(u2 face, vec3 position, float *mesh)
{
    memcpy(mesh, cubeVertices + face * CUBE_VERTEX_SIZE * 6, CUBE_VERTEX_SIZE * sizeof(float) * 6);
    for (int i = 0; i < 6; i++)
    {
        glm_vec3_add(position, mesh + i * CUBE_VERTEX_SIZE, mesh + i * CUBE_VERTEX_SIZE);
    }
    return CUBE_VERTEX_SIZE * 6;
}

void worldGenerateChunkMesh(struct World *self, i4 chunkPos[2])
{
    struct Chunk* chunk = chunkTableGet(self->chunkTable, chunkPos);
    count_t totalWritten = 0;
    chunk->mesh = malloc(CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * sizeof(float) * CUBE_VERTEX_SIZE * 36);
    for (int i = 0; i < CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y; i++)
    {
        if (chunk->blockTypeArr[i] != AIR)
        {
            count_t coords[3] = {i % CHUNK_SIZE_X, i / CHUNK_SIZE_X / CHUNK_SIZE_Z, i / CHUNK_SIZE_X % CHUNK_SIZE_Z};
            vec3 position = {(coords[0]) * cubeSize, (coords[1]) * cubeSize, (coords[2]) * cubeSize};
            char neighborTypes[6] = {0};
            worldGetBlockNeighbors(self, i, *chunk, chunkPos, coords, neighborTypes);
            for (int j = 0; j < 6; j++)
            {
                if (neighborTypes[j] == AIR)
                {
                    totalWritten += blockGenerateMesh(j, position, chunk->mesh + totalWritten);
                }
            }
        }
    }
    chunk->vertexCount = totalWritten;
}