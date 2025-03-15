#pragma once
#include "core/core.h"
#include <cglm/cglm.h>

#ifndef TEMP_MESH_DEFINE
#define TEMP_MESH_DEFINE
#define TEMP_MESH_COUNT 3

#define TEMP_MESH_SIZE (CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y * sizeof(f4) * CUBE_VERTEX_SIZE * 36)

struct TempMesh{
    Bool isFree;
    count_t totalWritten;
    f4* mesh;
};
#endif

struct ShaderBufferBinding{
    count_t valueCount;
    GLenum type;
    GLboolean normalized;
    size_t stride;
    size_t offset;
};

struct Uniform{
    // TODO change this with custom string
    char name[MAX_SHADER_VARIABLE_NAME_LENGTH];
    GLenum type;
    size_t size;
    Bool isSet;
    void *data;
};

struct Mesh{
    count_t bindingCount;
    struct ShaderBufferBinding bufferBindings[16];

    count_t uniformCount;
    struct Uniform uniforms[MAX_UNIFORM_COUNT];
    
    count_t vertexCount;
    size_t vertexSize;
    GlVertexBuffer vertexBuffer;

    count_t indexCount;
    u4 indexBufferObject;

    struct TempMesh* tempMesh;

    Bool isIndexed;
    Bool isInUse;
    Bool isGenerating;
    Bool isLoading;
};


void meshCreate(count_t uniformCount,struct Uniform* meshUniforms ,count_t bindingCount, struct ShaderBufferBinding *bindings, struct Mesh *dest);

void meshSetUniform(struct Mesh *self, const char *name, size_t size, void *data);

void meshCopyData(struct Mesh * self);

void meshDestroy(struct Mesh *self);