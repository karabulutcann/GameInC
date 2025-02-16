#pragma once
#include "safe_strings.h"
#include <glad/glad.h>

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

// 8 btye / 64 bit size value
// Use only for representing size as bytes not as value count
// u8[3] size is 8 * 3 = 24 bytes
typedef u8 size;

// 8 btye / 64 bit count value
// use only for representing count of values
typedef u8 count_t;

typedef u8 index_t;

