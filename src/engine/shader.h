#pragma once
#include "core/core.h"

struct Shader{
    unsigned int ID;
};

struct Shader shaderCreate(const char *vertexPath, const char *fragmentPath,const char *geometryPath);

void shaderCompileAndAttach(unsigned int shaderId, const char *path, GLenum shaderType);

void shaderUse(struct Shader shader);

void shaderSetUniformBool(struct Shader shader,const char* name,Bool value);

void shaderSetUniformInt(struct Shader shader,const char* name,int value);

void shaderSetUniformFloat(struct Shader shader,const char* name,float value);

void shaderSetUniformVec3(struct Shader shader,const char* name,vec3 value);

void shaderSetUniformVec3Arr(struct Shader shader, const char *name,size_t arrSize,vec3 value);

void shaderSetUniformMat4(struct Shader shader, const char *name, mat4 value);

void shaderDestroy(struct Shader shader);
