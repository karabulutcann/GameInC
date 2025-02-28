#include "mesh.h"

void meshCreate(struct Mesh *dest)
{
    glCreateVertexArrays(1, &dest->vertexBuffer);
}

void meshDestroy(struct Mesh *self)
{
    glDeleteVertexArrays(1, &self->vertexBuffer);
    glDeleteBuffers(1, &self->indexBufferObject);
}