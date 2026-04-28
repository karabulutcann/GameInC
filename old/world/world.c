#include "world.h"
#include "core/core.h"
#include "core/log.h"
#include <memory.h>

#define textureAtlasSize 4

static struct World *staticWorld;

const float cubeVertices[] = {
    // Back face
    -sizeVertex, -sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left
    sizeVertex, sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top right
    sizeVertex, -sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom right
    sizeVertex, sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top right
    -sizeVertex, -sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left
    -sizeVertex, sizeVertex, -sizeVertex, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top left

    // front face
    -sizeVertex, -sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    sizeVertex, -sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
    sizeVertex, sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right
    sizeVertex, sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right
    -sizeVertex, sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top left
    -sizeVertex, -sizeVertex, sizeVertex, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left

    // left face
    -sizeVertex, sizeVertex, sizeVertex, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
    -sizeVertex, sizeVertex, -sizeVertex, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top-left
    -sizeVertex, -sizeVertex, -sizeVertex, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    -sizeVertex, -sizeVertex, -sizeVertex, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    -sizeVertex, -sizeVertex, sizeVertex, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
    -sizeVertex, sizeVertex, sizeVertex, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right

    // right face
    sizeVertex, sizeVertex, sizeVertex, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top-left
    sizeVertex, -sizeVertex, -sizeVertex, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    sizeVertex, sizeVertex, -sizeVertex, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
    sizeVertex, -sizeVertex, -sizeVertex, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    sizeVertex, sizeVertex, sizeVertex, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top-left
    sizeVertex, -sizeVertex, sizeVertex, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

    // bottom face
    -sizeVertex, -sizeVertex, -sizeVertex, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    sizeVertex, -sizeVertex, -sizeVertex, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
    sizeVertex, -sizeVertex, sizeVertex, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    sizeVertex, -sizeVertex, sizeVertex, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    -sizeVertex, -sizeVertex, sizeVertex, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -sizeVertex, -sizeVertex, -sizeVertex, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right

    // top face
    -sizeVertex, sizeVertex, -sizeVertex, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    sizeVertex, sizeVertex, sizeVertex, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    sizeVertex, sizeVertex, -sizeVertex, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    sizeVertex, sizeVertex, sizeVertex, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    -sizeVertex, sizeVertex, -sizeVertex, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    -sizeVertex, sizeVertex, sizeVertex, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
};

struct World *worldGet()
{
    if (staticWorld == NULL)
    {
        mError("World not initialized");
        exit(1);
    }
    return staticWorld;
}

void worldCreate(struct World *dest)
{
    staticWorld = dest;
    dest->chunkTable = chunkTableCreate(TOTAL_LOAD_COUNT);
}

void worldLoadChunk(struct World *self, i4 chunkPos[2])
{
    struct Chunk chunk = {0};
    chunkCreate(chunkPos, &chunk);
    struct Result r = chunkTableInsert(self->chunkTable, chunkPos, chunk);
    if (!r.success)
    {
        chunkDestroy(&chunk);
        CACHE_RESULT(r)
    }
}

Bool staticWorldLoadChunk(void *_chunkPos)
{
    i4 chunkPos[2] = {0};
    chunkPos[0] = ((i4 *)_chunkPos)[0];
    chunkPos[1] = ((i4 *)_chunkPos)[1];

    struct Chunk chunk = {0};
    chunkCreate(chunkPos, &chunk);
    struct Result r = chunkTableInsert(staticWorld->chunkTable, chunkPos, chunk);
    if (!r.success)
    {
        chunkDestroy(&chunk);
        CACHE_RESULT(r);
        return FALSE;
    }
    return TRUE;
}

void worldUnloadChunk(struct World *self, i4 chunkPos[2])
{
    struct Chunk *chunk = chunkTableGet(self->chunkTable, chunkPos);
    if (chunk == NULL)
    {
        return;
    }
    chunk->isLoading = TRUE;
    chunkTableRemove(self->chunkTable, chunkPos);
    memset(chunk, 0, sizeof(struct Chunk));
}

// TODO bu fonksiyon dogrudan degeri dondurebilir duzelt
void worldGetBlock(struct World *self, i4 chunkPos[2], i4 blockPos[3], u4 *blockType)
{
    struct Chunk *chunk = chunkTableGet(self->chunkTable, chunkPos);
    if (chunk == NULL)
    {
        return;
    }

    i4 index = (blockPos[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (blockPos[2] * CHUNK_SIZE_X) + blockPos[0];
    *blockType = chunk->blockTypeArr[index];
}

u4 staticWorldGetBlock(i4 worldPos[3])
{
    u4 blockType = 0;
    i4 chunkPos[2] = {0};
    getChunkPos(worldPos,chunkPos);
    worldGetBlock(staticWorld,chunkPos, (i4[3]){worldPos[0] % CHUNK_SIZE_X, worldPos[1], worldPos[2] % CHUNK_SIZE_Z}, &blockType);
    return blockType;
}

Bool staticWorldSetBlock(i4 worldPos[3], u4 blockType)
{
    i4 chunkPos[2] = {0, 0};
    getChunkPos(worldPos, chunkPos);
    struct Chunk *chunk = chunkTableGet(staticWorld->chunkTable, chunkPos);
    index_t blockIndex = getBlockIndex(worldPos);
    if (chunk == NULL || worldPos[1] >= CHUNK_SIZE_Y || blockIndex > (CHUNK_BLOCK_TYPE_LENGHT))
        return false;
    chunk->blockTypeArr[blockIndex] = blockType;
    return true;
}

void worldGetBlockNeighbors(struct World *self, index_t i, struct Chunk currentChunk, i4 chunkCoords[2], u4 blockCoords[3], char neighborTypes[6])
{
    // Right face
    if ((blockCoords[0] + 1) < CHUNK_SIZE_X)
    {
        neighborTypes[RIGHT] = currentChunk.blockTypeArr[i + 1];
    }
    else
    {
        i4 rightChunkCoords[2] = {chunkCoords[0] + 1, chunkCoords[1]};
        struct Chunk *rightChunk = chunkTableGet(self->chunkTable, rightChunkCoords);
        if (rightChunk != NULL)
        {
            neighborTypes[RIGHT] = rightChunk->blockTypeArr[blockCoords[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z + blockCoords[2] * CHUNK_SIZE_X];
        }
        else
        {
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
        struct Chunk *leftChunk = chunkTableGet(self->chunkTable, leftChunkCoords);
        if (leftChunk != NULL)
        {
            neighborTypes[LEFT] = leftChunk->blockTypeArr[blockCoords[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z + (blockCoords[2] + 1) * CHUNK_SIZE_X - 1];
        }
        else
        {
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
        struct Chunk *backChunk = chunkTableGet(self->chunkTable, backChunkCoords);
        if (backChunk != NULL)
        {
            neighborTypes[BACK] = backChunk->blockTypeArr[(blockCoords[1] + 1) * CHUNK_SIZE_X * CHUNK_SIZE_Z - CHUNK_SIZE_X + blockCoords[0]];
        }
        else
        {
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
        struct Chunk *frontChunk = chunkTableGet(self->chunkTable, frontChunkCoords);
        if (frontChunk != NULL)
        {
            neighborTypes[FRONT] = frontChunk->blockTypeArr[(blockCoords[1]) * CHUNK_SIZE_X * CHUNK_SIZE_Z + blockCoords[0]];
        }
        else
        {
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

count_t blockGenerateMesh(u2 face, u1 blockType, vec3 position, float *mesh)
{
    memcpy(mesh, cubeVertices + face * CUBE_VERTEX_SIZE * 6, CUBE_VERTEX_SIZE * sizeof(float) * 6);
    vec2 uvCoords = {0.0f, 0.0f};
    float singleBlockSize = 1.0f / 4;

    switch (blockType)
    {
    case DIRT:
        uvCoords[0] = singleBlockSize * 0;
        uvCoords[1] = singleBlockSize * 2;
        break;

    case GRASS:
        switch (face)
        {
        case TOP:
            uvCoords[0] = singleBlockSize * 2;
            uvCoords[1] = singleBlockSize * 2;
            break;
        case BOTTOM:
            uvCoords[0] = singleBlockSize * 0;
            uvCoords[1] = singleBlockSize * 2;
            break;
        default:
            uvCoords[0] = singleBlockSize * 1;
            uvCoords[1] = singleBlockSize * 2;
            break;
        }
        break;
    case OAK_PLANK:
        uvCoords[0] = singleBlockSize * 2;
        uvCoords[1] = singleBlockSize * 1;
        break;
    case OAK_LOG:
        if (face == TOP)
        {
            uvCoords[0] = singleBlockSize * 1;
            uvCoords[1] = singleBlockSize * 1;
        }
        else
        {
            // uvCoords[0] = singleBlockSize * 0;
            uvCoords[1] = singleBlockSize * 1;
        }
        break;
    case CRAFTING_TABLE:
        if (face == TOP || face == BOTTOM)
        {
            uvCoords[0] = singleBlockSize * 3;
            uvCoords[1] = singleBlockSize * 3;
        }
        else if (face == FRONT)
        {
            uvCoords[0] = singleBlockSize * 2;
            uvCoords[1] = singleBlockSize * 3;
        }
        else
        {
            uvCoords[0] = singleBlockSize * 1;
            uvCoords[1] = singleBlockSize * 3;
        }
        break;
    case STONE_BRICK:
        uvCoords[0] = singleBlockSize * 1;
        // uvCoords[1] = singleBlockSize * 0;
    case MOSSY_STONE_BRICK:
        uvCoords[0] = singleBlockSize * 3;
        uvCoords[1] = singleBlockSize * 2;
    case CRACKED_STONE_BRICK:
        // uvCoords[0] = singleBlockSize * 0;
        uvCoords[1] = singleBlockSize * 3;
    case STONE:
    default:
        break;
    }
    for (int i = 0; i < 6; i++)
    {
        glm_vec3_add(position, mesh + i * CUBE_VERTEX_SIZE, mesh + i * CUBE_VERTEX_SIZE);
        glm_vec2_scale(mesh + i * CUBE_VERTEX_SIZE + 6, singleBlockSize, mesh + i * CUBE_VERTEX_SIZE + 6);
        glm_vec2_add(mesh + i * CUBE_VERTEX_SIZE + 6, uvCoords, mesh + i * CUBE_VERTEX_SIZE + 6);
    }
    return CUBE_VERTEX_SIZE * 6;
}

void worldDestroy(struct World *self)
{
    chunkTableDestroy(self->chunkTable);
}
