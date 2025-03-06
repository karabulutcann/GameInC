#include "core/file.h"
#include "core/log.h"
#include "core/result.h"
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "shader.h"

#include <string.h>

const char *defaultVertexShader = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos, 1.0);\n"
                                  "}\0";

const char *defaultFragmentShader = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";

struct Result shaderCreate(const char *vertexPath, const char *fragmentPath, const char *geometryPath, struct Shader *dest)
{
    *dest = (struct Shader){0};
    int success;

    dest->id = glCreateProgram();
    shaderCompileAndAttach(dest->id, vertexPath, GL_VERTEX_SHADER);
    shaderCompileAndAttach(dest->id, fragmentPath, GL_FRAGMENT_SHADER);

    if (geometryPath != NULL)
    {
        shaderCompileAndAttach(dest->id, geometryPath, GL_GEOMETRY_SHADER);
    }

    glLinkProgram(dest->id);

    glGetProgramiv(dest->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(dest->id, 512, NULL, infoLog);
        glDeleteProgram(dest->id);
        return mErr(format("Shader linking failed: %s\n", infoLog));
    }

    i4 attributeCount;
    glGetProgramiv(dest->id, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    mDebug("Active Attribute count: %d\n", attributeCount);
    dest->attributeCount = attributeCount;

    for(index_t i = 0; i < attributeCount; i++){
        char name[MAX_SHADER_VARIABLE_NAME_LENGTH];
        size_t written = 0;
        GLint size = 0;
        GLenum type = 0;
        glGetActiveAttrib(dest->id, (GLuint)i, MAX_SHADER_VARIABLE_NAME_LENGTH, &written, &size, &type, name);

        dest->attributes[i].location = glGetAttribLocation(dest->id, name);
        dest->attributes[i].type = type;
        dest->attributes[i].normalized = GL_FALSE;
        dest->attributes[i].stride = 0;
        dest->attributes[i].offset = 0;
    }

    // eger uniformlar shaderda kullanilmiyosa yada out degerlerini degistirmiyosa active sayilmiyo 
    // oyuzden tum uniformlar gorunmuyor
    i4 uniformCount;
    glGetProgramiv(dest->id, GL_ACTIVE_UNIFORMS, &uniformCount);
    mDebug("Active Uniform count: %d\n", uniformCount);
    dest->uniformCount = uniformCount;

    for(index_t i = 0; i < uniformCount; i++){
        char name[MAX_SHADER_VARIABLE_NAME_LENGTH];
        size_t written = 0;
        GLint size = 0;
        GLenum type = 0;
        glGetActiveUniform(dest->id, (GLuint)i, MAX_SHADER_VARIABLE_NAME_LENGTH, &written, &size, &type, name);

        strcpy(dest->uniforms[i].name, name);
        dest->uniforms[i].type = type;
        dest->uniforms[i].size = size;
    }
    
    glCreateVertexArrays(1, &dest->vertexArrayObject);

    return ok();
}

//TODO return a result
void shaderCompileAndAttach(unsigned int shaderId, const char *path, GLenum shaderType)
{
    char *shaderCode;
    i4 success;
    const char *shaderTypeName = NULL;

    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        shaderTypeName = "Vertex";
        break;
    case GL_FRAGMENT_SHADER:
        shaderTypeName = "Fragment";
        break;
    case GL_GEOMETRY_SHADER:
        shaderTypeName = "Geometry";
        break;
    default:
        mError("Unknown shader type\n");
        exit(1);
    }

    CACHE_RESULT(fileRead(path, &shaderCode), FATAL);

    u4 shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        size_t infoLogSizeWithoutNull = 0;
        glGetShaderInfoLog(shader, 0, &infoLogSizeWithoutNull, NULL);
        //TODO change this with custom string
        char *infoLog = malloc(infoLogSizeWithoutNull + 1);
        glGetShaderInfoLog(shader, infoLogSizeWithoutNull, NULL, infoLog);
        mError("%s shader compilation failed: %s\n ", shaderTypeName, infoLog);
        exit(1);
    }

    glAttachShader(shaderId, shader);

    glDeleteShader(shader);
    free(shaderCode);
}



void shaderBindBuffers(struct Shader *self,count_t bindingCount,struct ShaderBufferBinding* bindings,GlVertexBuffer buffer){
    for(index_t i = 0; i < bindingCount; i++){
        glEnableVertexArrayAttrib(self->vertexArrayObject, i);
        glVertexArrayAttribFormat(self->vertexArrayObject, i, bindings[i].valueCount, bindings[i].type, bindings[i].normalized, bindings[i].offset);
        //glVertexArrayAttribBinding(self->vertexArrayObject, i, i);
        glVertexArrayVertexBuffer(self->vertexArrayObject, i, buffer, bindings[i].offset, bindings[i].stride);
    }
}

typedef void (*shaderPrepareCallback)(struct Shader* self);

void shaderPrepareForDraw(struct Shader *self)
{
    glUseProgram(self->id);
    for(int i = 0; i < self->textureCount; i++){
        glBindTextureUnit(i, self->textures[i].id);
    }
}

void shaderCreateTexture(struct Shader *self, const char *imagePath, const char *samplerName)
{
    textureCreate(imagePath, samplerName, &self->textures[self->textureCount]);
    self->textureCount++;

    shaderUse(self);
    shaderSetInt(self,samplerName, (i4)(self->textureCount - 1));
}

void shaderUse(struct Shader *self)
{
    glUseProgram(self->id);
}


void shaderSetBool(struct Shader *self, const char *name, Bool value)
{
    glProgramUniform1i(self->id, glGetUniformLocation(self->id, name), value);
}

void shaderSetInt(struct Shader *self, const char *name, int value)
{
    glProgramUniform1i(self->id, glGetUniformLocation(self->id, name), value);
}

void shaderSetFloat(struct Shader *self, const char *name, float value)
{
    glProgramUniform1f(self->id, glGetUniformLocation(self->id, name), value);
}

void shaderSetVec3(struct Shader *self, const char *name, vec3 value)
{
    glProgramUniform3fv(self->id, glGetUniformLocation(self->id, name), 1, value);
}

void shaderSetVec3Arr(struct Shader *self, const char *name, size_t arrSize, vec3 value)
{
    glProgramUniform3fv(self->id, glGetUniformLocation(self->id, name), arrSize, value);
}

void shaderSetMat4(struct Shader *self, const char *name, mat4 value)
{
    glProgramUniformMatrix4fv(self->id, glGetUniformLocation(self->id, name), 1, GL_FALSE, &value[0][0]);
}

void shaderDestroy(struct Shader *self)
{
    glDeleteVertexArrays(1, &self->vertexArrayObject);
    glDeleteProgram(self->id);
}