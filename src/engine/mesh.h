#include "core/core.h"

struct Mesh{
    count_t vertexCount;
    u4 vertexBufferObject;
    count_t indexCount;
    u4 indexBufferObject;
    Bool isIndexed;
};