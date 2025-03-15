#pragma once
#include "core/core.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "core/dynamic_array.h"

struct ShaderAttribute{
    index_t location;
    GLenum type;
    Bool normalized;
    size_t stride;
    size_t offset;
};



struct Shader{
    u4 id;

    //TODO turn this into dynamic array
    struct ShaderAttribute attributes[MAX_ATTRIBUTE_COUNT];
    count_t attributeCount;

    //TODO turn this into dynamic array
    struct Uniform uniforms[MAX_UNIFORM_COUNT];
    count_t uniformCount;

    struct Texture textures[16];
    count_t textureCount;

    //TODO change this with custom string
    const char* vertexPath;
    const char* fragmentPath;
    GlVertexArray vertexArrayObject;

    //TODO change with dynamic array 
    struct Mesh* meshes;
    count_t meshCount;
};

struct Result shaderCreate(const char *vertexPath, const char *fragmentPath, const char *geometryPath, struct Shader *dest);

void shaderCompileAndAttach(unsigned int shaderId, const char *shaderCode, GLenum shaderType);

void shaderBindBuffers(struct Shader *self,count_t bindingCount,struct ShaderBufferBinding* bindings,GlVertexBuffer buffer);

// samplerName cannot be "texture" because it is a reserved word
void shaderCreateTexture(struct Shader *self, const char *imagePath, const char *samplerName);

void shaderBindUniforms(struct Shader* self,count_t uniformCount, struct Uniform* uniforms);

void shaderUse(struct Shader* self);

void shaderPrepareForDraw(struct Shader *self);

void shaderSetBool(struct Shader* self,const char* name,Bool value);

void shaderSetInt(struct Shader* self,const char* name,int value);

void shaderSetFloat(struct Shader* self,const char* name,float value);

void shaderSetVec3(struct Shader* self,const char* name,vec3 value);

void shaderSetVec3Arr(struct Shader* self, const char *name,size_t arrSize,vec3 value);

void shaderSetVec4(struct Shader* self,const char* name,vec4 value);

void shaderSetMat4(struct Shader* self, const char *name, mat4 value);

void shaderDestroy(struct Shader* self);
