#include "core/file.h"
#include "core/log.h"
#include "core/result.h"
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "shader.h"

#include <string.h>

const char *defaultVertexShader = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "uniform mat4 projection;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = projection * vec4(aPos, 1.0);\n"
                                  "}\0";

const char *defaultFragmentShader = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "uniform vec3 color;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(color, 1.0);\n"
                                    "}\n\0";

struct Result shaderCreate(const char *vertexPath, const char *fragmentPath, const char *geometryPath, struct Shader *dest)
{
    *dest = (struct Shader){0};
    dest->vertexPath = vertexPath;
    dest->fragmentPath = fragmentPath;
    int success;

    dest->id = glCreateProgram();

    char *shaderCode = NULL;
    Bool isFileUsed = FALSE;

    if (vertexPath != NULL)
    {
        fileRead(vertexPath, &shaderCode);
        isFileUsed = TRUE;
    }
    else
    {
        shaderCode = defaultVertexShader;
    }
    shaderCompileAndAttach(dest->id, shaderCode, GL_VERTEX_SHADER);

    if (fragmentPath != NULL)
    {
        fileRead(fragmentPath, &shaderCode);
        isFileUsed = TRUE;
    }
    else
    {
        shaderCode = defaultFragmentShader;
    }
    shaderCompileAndAttach(dest->id, shaderCode, GL_FRAGMENT_SHADER);

    if (geometryPath != NULL)
    {
        fileRead(geometryPath, &shaderCode);
        isFileUsed = TRUE;
        shaderCompileAndAttach(dest->id, shaderCode, GL_GEOMETRY_SHADER);
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

    GLint attributeCount;
    glGetProgramiv(dest->id, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    mDebug("Active Attribute count: %d\n", attributeCount);
    dest->attributeCount = attributeCount;

    for (index_t i = 0; i < attributeCount; i++)
    {
        char name[MAX_SHADER_VARIABLE_NAME_LENGTH];
        GLsizei written = 0;
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
    GLint uniformCount;
    glGetProgramiv(dest->id, GL_ACTIVE_UNIFORMS, &uniformCount);
    mDebug("Active Uniform count: %d\n", uniformCount);
    dest->uniformCount = uniformCount;

    for (index_t i = 0; i < uniformCount; i++)
    {
        char name[MAX_SHADER_VARIABLE_NAME_LENGTH];
        GLsizei written = 0;
        GLint size = 0;
        GLenum type = 0;
        glGetActiveUniform(dest->id, (GLuint)i, MAX_SHADER_VARIABLE_NAME_LENGTH, &written, &size, &type, name);

        strcpy(dest->uniforms[i].name, name);
        dest->uniforms[i].type = type;
        dest->uniforms[i].size = size;
    }

    glCreateVertexArrays(1, &dest->vertexArrayObject);
    if (isFileUsed)
    {
        free(shaderCode);
    }
    
    return ok();
}

// TODO return a result
void shaderCompileAndAttach(unsigned int shaderId, const char *shaderCode, GLenum shaderType)
{

    if (shaderCode == NULL)
    {
        mError("Shader code cant be null");
        exit(1);
    }

    GLint success;
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

    u4 shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLsizei infoLogSizeWithoutNull = 0;
        glGetShaderInfoLog(shader, 0, &infoLogSizeWithoutNull, NULL);
        // TODO change this with custom string
        char *infoLog = malloc(infoLogSizeWithoutNull + 1);
        glGetShaderInfoLog(shader, infoLogSizeWithoutNull, NULL, infoLog);
        mError("%s shader compilation failed: %s\n ", shaderTypeName, infoLog);
        exit(1);
    }

    glAttachShader(shaderId, shader);

    glDeleteShader(shader);
}

void shaderBindBuffers(struct Shader *self, count_t bindingCount, struct ShaderBufferBinding *bindings, GlVertexBuffer buffer)
{
    glBindVertexArray(self->vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    for (index_t i = 0; i < bindingCount; i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, bindings[i].valueCount, bindings[i].type, bindings[i].normalized, bindings[i].stride, bindings[i].offset);
    }
}

void shaderBindUniforms(struct Shader *self, count_t uniformCount, struct Uniform *uniforms)
{
    for (index_t i = 0; i < uniformCount; i++)
    {
        if (uniforms[i].isSet)
        {
            switch (uniforms[i].type)
            {
            case GL_FLOAT:
                shaderSetFloat(self, uniforms[i].name, *(float *)uniforms[i].data);
                break;
            case GL_FLOAT_VEC3:
                shaderSetVec3(self, uniforms[i].name, *(vec3 *)uniforms[i].data);
                break;
            case GL_FLOAT_MAT4:
                shaderSetMat4(self, uniforms[i].name, *(mat4 *)uniforms[i].data);
                break;
            default:
                break;
            }
        }
    }
}

void shaderPrepareForDraw(struct Shader *self)
{
    glUseProgram(self->id);
    for (int i = 0; i < self->textureCount; i++)
    {
        glBindTextureUnit(i, self->textures[i].id);
    }
}

void shaderCreateTexture(struct Shader *self, const char *imagePath, const char *samplerName)
{
    textureCreate(imagePath, samplerName, &self->textures[self->textureCount]);
    self->textureCount++;

    shaderUse(self);
    shaderSetInt(self, samplerName, (i4)(self->textureCount - 1));
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

void shaderSetVec4(struct Shader *self, const char *name, vec4 value)
{
    glProgramUniform4fv(self->id, glGetUniformLocation(self->id, name), 1, value);
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
