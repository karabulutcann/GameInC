#pragma once
#include "safe_strings.h"
#include <glad/glad.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define CHUNK_BLOCK_TYPE_LENGHT CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y 

#define CUBE_VERTEX_SIZE 8

#define sizeVertex 0.2f

#define cubeSize sizeVertex * 2

#define TEMP_MESH_COUNT 3

#define TRUE 1
#define FALSE 0

typedef short unsigned int Bool;

struct DisposableString format(const char *fmt, ...);

#define GET_MACRO2(_1, _2, NAME, ...) NAME

// types with their btye size

// 1 btye / 8 bit unsigned integer
typedef unsigned char u1;

// 2 btye / 16 bit unsigned integer
typedef unsigned short int u2;

// 4 btye / 32 bit unsigned integer
typedef unsigned long int u4;

// 8 btye / 64 bit unsigned integer
typedef unsigned long long int u8;

// 1 btye / 8 bit integer
// dont compare signed integers with unsigned integers
// its turns signed into unsigned and becouse -1 is turn int to unsigned it becomes 255
// -1 == 255 so code breaks
typedef char i1;

// 2 btye / 16 bit integer
typedef short int i2;

// 4 btye / 32 bit integer
typedef long int i4;

// 8 btye / 64 bit integer
typedef long long int i8;

// 4 btye / 32 bit float
typedef float f4;

// 8 btye / 64 bit float
typedef double f8;

// 10 btye / 80 bit float
typedef long double f10;

typedef void* Allocated;

// 8 btye / 64 bit size value
// Use only for representing size as bytes not as value count
// u8[3] size is 8 * 3 = 24 bytes
// typedef u8 size_t;

// 8 btye / 64 bit count value
// use only for representing count of values
typedef u8 count_t;

typedef u8 index_t;

#ifndef size_t
typedef u8 size_t;
#endif

typedef u8 clockT;

typedef u8 id_t;

typedef GLuint GlFrameBuffer;

typedef GLuint GlTexture;

typedef GLuint GlRenderBuffer;

typedef GLuint GlVertexBuffer;
typedef GLuint GlIndexBuffer;

typedef GLuint GlVertexArray;

#define MAX_UNIFORM_COUNT 16
#define MAX_ATTRIBUTE_COUNT 6
#define MAX_SHADER_VARIABLE_NAME_LENGTH 64

#define WORLD_SIZE_X 10

#define LOAD_DISTANCE 20

#define TOTAL_LOAD_COUNT (LOAD_DISTANCE * 2 + 1) * (LOAD_DISTANCE * 2 + 1)

#define MAX_MESH_COUNT TOTAL_LOAD_COUNT

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

int random();