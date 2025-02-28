#include <cJSON/cJSON.h>
#include <stdio.h>
#include <rapidhash.h>
#include "core.h"
#include "result.h"


struct Chunk{
    i4 position[2];
    Bool isVboCreated;
    Bool isLoading;
    u1* blockTypeArr;
    float* mesh;
    u4 vertexBufferObject;
    count_t vertexCount;
};

u4 hashPosition(i4 position[2])
{
    return (u4)rapidhash(position, sizeof(i4) * 2);
}

struct ChunkTableMeta
{
    u4 length;
    u4 count;
};

typedef struct Chunk *ChunkTable;

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

int main()
{
    // get the user input and hash it then print it until the user enters 'quit'
    char input[100];
    while (strcmp(input, "quit") != 0)
    {
        printf("Enter a string to hash: ");
        scanf("%s", input);
        printf("Hash: %d\n hashtable: %d\n", rapidhash(input, strlen(input)), rapidhash(input, strlen(input)) % 10);
    }
    return 0;
}
