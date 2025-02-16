#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define POINTER_SIZE sizeof(void *)

enum DataType
{
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    BOOL,
    CHAR,
    VOID
};

struct HashTableMeta
{
    unsigned long int length;
    unsigned long int count;
    enum DataType type;
};

typedef void **HashTable;

long hashKey(void* key)
{
    return position[0] | ~position[1];
}

HashTable hashTableCreate(long int length, enum DataType type)
{

    HashTable table = malloc(sizeof(struct HashTableMeta) + length * POINTER_SIZE);
    memset(table, 0, sizeof(struct HashTableMeta) + length * POINTER_SIZE);
    struct HashTableMeta *meta = (struct HashTableMeta *)table;
    meta->length = length;
    meta->count = 0;
    meta->type = type;
    // Add padding to the end of the table
    return (HashTable)((char *)table + sizeof(struct HashTableMeta));
}

struct HashTableMeta *hashTableGetMeta(HashTable table)
{
    return (struct HashTableMeta *)((char *)table - sizeof(struct HashTableMeta));
}

void hashTableInsert(HashTable table, void* key, void *value)
{
    struct HashTableMeta *meta = hashTableGetMeta(table);
    if (meta->count == meta->length)
    {
        printf("Hash table is full!\n");
        return;
    }
    unsigned long int key = hash(position) % meta->length;
    printf("Key is %ld\n", key);
    void **bucket = ((char *)table + key * POINTER_SIZE);
    printf("Bucket is %p\n", *bucket);
    if (*bucket == NULL)
    {
        *bucket = value;
        meta->count++;
    }
    else
    {
        int isFound = 0;
        while (isFound)
        {
            bucket = ((char *)table + key * POINTER_SIZE);
            if (*bucket == NULL)
            {
                *bucket = value;
                meta->count++;
                isFound = 1;
            }
            else
            {
                key++;
                if (key == meta->length)
                {
                    key = 0;
                }
            }
        }
    }
}

void hashTableRemove(HashTable table, long int position[2])
{
    struct HashTableMeta *meta = hashTableGetMeta(table);
    unsigned long int key = hash(position) % meta->length;
    void **bucket = ((char *)table + key * POINTER_SIZE);
    if (*bucket == NULL)
    {
        printf("Hash table does not contain a value at this position!\n");
        return;
    }
    *bucket = NULL;
    meta->count--;
}

// create a callback funtion typedef for validating the key

typedef void* (*hashTableCompareKey)(long int[2]);

void hashTableGet(HashTable table, long int position[2], void **value,)
{
    struct HashTableMeta *meta = hashTableGetMeta(table);
    unsigned long int key = hash(position) % meta->length;
    void **bucket = ((char *)table + key * POINTER_SIZE);
    if (*bucket == NULL)
    {
        printf("Hash table does not contain a value at this position!\n");
        return;
    }
    *value = *bucket;
}

struct Chunk
{
    int x;
    int z;
    int y;
};

int main()
{

    struct Chunk chunk = {1, 2, 3};
    struct Chunk chunk2 = {4, 5, 6};

    HashTable table = hashTableCreate(10, INT);

    hashTableInsert(table, (long int[2]){1, 2}, &chunk);
    hashTableInsert(table, (long int[2]){3, 4}, &chunk2);

    printf("Count is %ld\n", hashTableGetMeta(table)->count);

    return 0;
}
