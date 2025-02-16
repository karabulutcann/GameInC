#pragma once
#include "core/core.h"
#include "core/result.h"

struct Texture{
    u4 id;
    u4 width;
    u4 height;
    Bool hasAlpha;
    const char* path;
    const char* samplerName;
    u4 samplerId;
};

struct Result textureCreate(const char* path, const char* samplerName, struct Texture* dest);