#pragma once
#include "core/core.h"
#include "texture.h"

struct Shader{
    u4 id;
    struct Texture textures[16];
    count_t textureCount;
    const char* vertexPath;
    const char* fragmentPath;
    u4 vertexArrayObject;
    struct Mesh* meshes;
    count_t meshCount;
};

struct Result shaderCreate(const char *vertexPath, const char *fragmentPath, const char *geometryPath, struct Shader *dest);

void shaderCompileAndAttach(unsigned int shaderId, const char *path, GLenum shaderType);


// samplerName cannot be "texture" because it is a reserved word
void shaderCreateTexture(struct Shader *self, const char *imagePath, const char *samplerName);

void shaderUse(struct Shader* self);

void shaderPrepareForDraw(struct Shader *self);

void shaderSetUniformBool(struct Shader* self,const char* name,Bool value);

void shaderSetUniformInt(struct Shader* self,const char* name,int value);

void shaderSetUniformFloat(struct Shader* self,const char* name,float value);

void shaderSetUniformVec3(struct Shader* self,const char* name,vec3 value);

void shaderSetUniformVec3Arr(struct Shader* self, const char *name,size_t arrSize,vec3 value);

void shaderSetUniformMat4(struct Shader* self, const char *name, mat4 value);

void shaderDestroy(struct Shader* self);
