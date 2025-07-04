#include "chunk.h"
#include "core/log.h"
#include "engine/engine.h"
#include <memory.h>

#define FNL_IMPL
#include <FastNoiseLight/FastNoiseLite.h>


index_t getBlockIndex(i4 worldPos[3]){
    return worldPos[1] * CHUNK_SIZE_X * CHUNK_SIZE_Z + (worldPos[2] % CHUNK_SIZE_Z) * CHUNK_SIZE_X + (worldPos[0] % CHUNK_SIZE_X);
}

void getChunkPos(i4 worldPos[3],i4 destPos[2]){
    destPos[0] = (worldPos[0] / CHUNK_SIZE_X) ;
    destPos[0] += (worldPos[0] > 0 ? 0 : -1);
    destPos[1] = (worldPos[2] / CHUNK_SIZE_X);
    destPos[1] += (worldPos[2] > 0 ? 0 : -1);
}

void chunkCreate(i4 chunkPos[2], struct Chunk *dest)
{
    memset(dest, 0, sizeof(struct Chunk));
    dest->position[0] = chunkPos[0];
    dest->position[1] = chunkPos[1];
    dest->isLoading = TRUE;
    // TODO turn this into stack array
    dest->blockTypeArr = malloc(CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * sizeof(u1));
    struct Mesh *mesh;
    for(index_t i = 0; i < TOTAL_LOAD_COUNT; i++){
        mesh = dynamicArrayGet(engineGet()->cubeDefferedRenderer.geometryPass.meshes, i);
        if(!mesh->isInUse){
            dest->mesh = mesh;
            mesh->isInUse = TRUE;
            mesh->isLoading = TRUE;
            break;
        }
    }

    // Allocate memory for the chunk
    if (dest->blockTypeArr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for chunk!\n");
        exit(1);
    }

    // Create a FastNoiseLite state
    fnl_state noise_state = fnlCreateState();
    noise_state.seed = random();                  // You can change the seed for different terrain generation
    noise_state.frequency = 0.009f;            // Adjust frequency for terrain detail
    noise_state.noise_type = FNL_NOISE_PERLIN; // You can experiment with different noise types
    noise_state.fractal_type = FNL_FRACTAL_FBM;
    // 4 den sonrası değişiklik yaratmıyo
    noise_state.octaves = 5;
    noise_state.gain = 0.3f;

    for (int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for (int z = 0; z < CHUNK_SIZE_Z; z++)
        {
            // Calculate world coordinates
            float world_x = (float)(chunkPos[0] * CHUNK_SIZE_X + x);
            float world_z = (float)(chunkPos[1] * CHUNK_SIZE_Z + z);
            float world_y = 0.0f; // Start at ground level

            // Sample noise to determine terrain height
            float height_sample = fnlGetNoise2D(&noise_state, world_x, world_z);
            int terrain_height = 20 + (int)((height_sample * height_sample ) * 200);
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                unsigned int block_id = 0; // Default: air

                if (y <= terrain_height)
                {
                    block_id = 2; // Stone
                    if (y > terrain_height - 3)
                    {
                        block_id = 1; // Dirt near surface
                        if (y > terrain_height - 1)
                        {
                            block_id = 3; // Grass
                        }
                    }
                }
                // Store the block ID in the chunk array.
                dest->blockTypeArr[y * CHUNK_SIZE_X * CHUNK_SIZE_Z + x + z * CHUNK_SIZE_X] = block_id;
            }
        }
    }
    dest->mesh->isLoading = FALSE;
    dest->isLoading = FALSE;
}

void chunkDestroy(struct Chunk *chunk)
{
    if (chunk->blockTypeArr != NULL)
    {
        free(chunk->blockTypeArr);
        chunk->blockTypeArr = NULL;
    }
    chunk->mesh->isInUse = FALSE;
}

#include <rapidhash.h>

u4 hashPosition(i4 position[2])
{
    return (u4)rapidhash(position, sizeof(i4) * 2);
}

struct ChunkTableMeta
{
    u4 length;
    u4 count;
};

ChunkTable chunkTableCreate(u4 length)
{
    ChunkTable table = malloc(sizeof(struct ChunkTableMeta) + length * sizeof(struct Chunk));
    memset(table, 0, sizeof(struct ChunkTableMeta) + length * sizeof(struct Chunk));
    struct ChunkTableMeta *meta = (struct ChunkTableMeta *)table;
    meta->length = length;
    meta->count = 0;

    return (ChunkTable)((u1 *)table + sizeof(struct ChunkTableMeta));
}

struct ChunkTableMeta *chunkTableGetMeta(ChunkTable table)
{
    return (struct ChunkTableMeta *)((u1 *)table - sizeof(struct ChunkTableMeta));
}

struct Result chunkTableInsert(ChunkTable table, i4 position[2], struct Chunk chunk)
{
    struct ChunkTableMeta *meta = chunkTableGetMeta(table);
    if (meta->count == meta->length)
    {
        return mErr("Hash table is full!");
    }
    u4 key = hashPosition(position) % meta->length;
    struct Chunk c = {0};
    c = table[(key) % meta->length];
    index_t i = 0;
        while (c.blockTypeArr != NULL && i < meta->length)
        {
            i++;
            c = table[(key + i) % meta->length];
        }
        if (c.blockTypeArr == NULL)
        {
            table[(key + i) % meta->length] = chunk;
            meta->count++;
        }
        else
        {
            mInfo("Cant insert chunk at position %d %d\n", position[0], position[1]);
        }
        return ok();
   
}

void chunkTableRemove(ChunkTable table, i4 position[2])
{
    struct ChunkTableMeta *meta = chunkTableGetMeta(table);
    u4 key = hashPosition(position) % meta->length;
    struct Chunk c = {0};
    index_t i = 0;

    while (i < meta->length)
    {
        c = table[(key + i) % meta->length];
        if (c.blockTypeArr != NULL && c.position[0] == position[0] && c.position[1] == position[1])
        {
            // clear Chunk before removing
            chunkDestroy(&c);
            table[(key + i) % meta->length] = (struct Chunk){0};
            meta->count--;
            return;
        }
        i++;
    }
    mInfo("Cant remove chunk at position %d %d\n", position[0], position[1]);
}

// create a callback funtion typedef for validating the key

struct Chunk *chunkTableGet(ChunkTable table, i4 position[2])
{
    struct ChunkTableMeta *meta = chunkTableGetMeta(table);
    u4 key = hashPosition(position) % meta->length;

    struct Chunk *c = NULL;
    index_t i = 0;
    while (i < meta->length)
    {
        c = table + (key + i) % meta->length;
        if (c->blockTypeArr != NULL && c->position[0] == position[0] && c->position[1] == position[1])
        {
            return c;
        }
        i++;
    }
    return NULL;
}

void chunkTableDestroy(ChunkTable table)
{
    struct ChunkTableMeta *meta = chunkTableGetMeta(table);
    for (int i = 0; i < meta->length; i++)
    {
        chunkDestroy(table + i);
    }
    free(meta);
}
