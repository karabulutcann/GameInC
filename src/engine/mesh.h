#include "core/core.h"
#include <cglm/cglm.h>

struct ShaderBufferBinding{
    count_t valueCount;
    GLenum type;
    GLboolean normalized;
    size stride;
    size offset;
};


struct Mesh{
    count_t bindingCount;
    struct ShaderBufferBinding bufferBindings[16];

    mat4 model;

    count_t vertexCount;
    GlVertexBuffer vertexBuffer;

    count_t indexCount;
    u4 indexBufferObject;

    Bool isIndexed;
};


void meshCreate(struct Mesh *dest);

void meshDestroy(struct Mesh *self);