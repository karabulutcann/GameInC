#include "mesh.h"
#include <string.h>
#include <memory.h>

//use gl function only call from main thread
void meshCreate(count_t uniformCount, struct Uniform *meshUniforms, count_t bindingCount, struct ShaderBufferBinding *bindings, struct Mesh *dest)
{
    *dest = (struct Mesh){0};
    dest->bindingCount = bindingCount;
    memcpy(dest->bufferBindings, bindings, sizeof(struct ShaderBufferBinding) * bindingCount);
    dest->uniformCount = uniformCount;
    memcpy(dest->uniforms, meshUniforms, sizeof(struct Uniform) * uniformCount);
    glCreateBuffers(1, &dest->vertexBuffer);
}

void meshCopyData(struct Mesh *self)
{
    glNamedBufferData(self->vertexBuffer, self->vertexCount * sizeof(f4) * CUBE_VERTEX_SIZE, self->tempMesh->mesh, GL_STATIC_DRAW);
    self->isLoading = FALSE;
    self->tempMesh->isFree = TRUE;
    self->tempMesh = NULL;
}

void meshSetUniform(struct Mesh *self, const char *name, size_t size, void *data)
{
    for (index_t i = 0; i < self->uniformCount; i++)
    {
        if (strcmp(self->uniforms[i].name, name) == 0)
        {
            self->uniforms[i].isSet = TRUE;
            if (self->uniforms[i].data != NULL)
            {
                free(self->uniforms[i].data);
            }
            self->uniforms[i].data = malloc(size);
            memcpy(self->uniforms[i].data, data, size);
            return;
        }
    }
}

void meshDestroy(struct Mesh *self)
{
    glDeleteVertexArrays(1, &self->vertexBuffer);
    glDeleteBuffers(1, &self->indexBufferObject);
}