#include "core/file.h"
#include "core/log.h"
#include "core/result.h"
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "shader.h"

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

struct Shader shaderCreate(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
    struct Shader shader = {0};
    int success;

    shader.ID = glCreateProgram();
    shaderCompileAndAttach(shader.ID, vertexPath, GL_VERTEX_SHADER);
    shaderCompileAndAttach(shader.ID, fragmentPath, GL_FRAGMENT_SHADER);
    if (geometryPath != NULL)
    {
        shaderCompileAndAttach(shader.ID, geometryPath, GL_GEOMETRY_SHADER);
    }
    glLinkProgram(shader.ID);

    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        ERROR("Shader linking failed: %s\n", infoLog);
        exit(1);
    }

    return shader;
}

void shaderCompileAndAttach(unsigned int shaderId, const char *path, GLenum shaderType)
{
    char *shaderCode;
    int success;
    const char * shaderTypeName = NULL;

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
        ERROR("Unknown shader type\n");
        exit(1);
    }

    CACHE_RESULT(fileRead(path, &shaderCode), FATAL);

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        ERROR("%s shader compilation failed: %s\n ",shaderTypeName,infoLog);
        exit(1);
    }

    glAttachShader(shaderId, shader);

    glDeleteShader(shader);
    free(shaderCode);
}

void shaderUse(struct Shader shader)
{
    glUseProgram(shader.ID);
}

void shaderSetUniformBool(struct Shader shader, const char *name, Bool value)
{
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void shaderSetUniformInt(struct Shader shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void shaderSetUniformFloat(struct Shader shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader.ID, name), value);
}
void shaderSetUniformVec3(struct Shader shader, const char *name, vec3 value)
{
    glUniform3fv(glGetUniformLocation(shader.ID, name), 1, value);
}
void shaderSetUniformVec3Arr(struct Shader shader, const char *name,size_t arrSize,vec3 value)
{
    glUniform3fv(glGetUniformLocation(shader.ID, name), arrSize, value);
}
void shaderSetUniformMat4(struct Shader shader, const char *name, mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, &value[0][0]);
}

void shaderDestroy(struct Shader shader)
{
    glDeleteProgram(shader.ID);
}