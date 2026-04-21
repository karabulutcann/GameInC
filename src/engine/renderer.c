#include "core/assert.h"
#include "core/file.h"
#include "core/types.h"
#include "defines.h"
#include "glad/glad.h"

void shaderCompile(GLenum type, const char *src, GLuint *dest) {
  *dest = glCreateShader(type);
  glShaderSource(*dest, 1, &src, NULL);
  glCompileShader(*dest);
  GLint ok;
  glGetShaderiv(*dest, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char log[512];
    glGetShaderInfoLog(*dest, sizeof(log), NULL, log);
    PANIC("Shader error:\n%s\n", log);
  }
}
struct ChunkMesh {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLsizei indexCount;
};

void chunkMeshCreate(f32* mesh,countT meshLength,struct ChunkMesh* dest){
    ASSERT(mesh != NULL && dest != NULL, "Invalid arguments");

    glGenVertexArrays(1,&dest->vao);
    glGenBuffers(1,&dest->vbo);
    // glGenBuffers(1,&dest->ebo);

    glBindVertexArray(dest->vao);
    glBindBuffer(GL_ARRAY_BUFFER,dest->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(f32) * meshLength,mesh,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(f32) * 3,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

struct ChunkPipeline {
  GLuint shader;
  struct ChunkMesh chunkMeshes[MAX_CHUNK_TO_RENDER];
};

void chunkPipelineCreate(struct ChunkPipeline* dest){
    char* vertShaderCode = NULL;
    fileRead("shaders/chunk.vert",&vertShaderCode);
    char* fragShaderCode = NULL;
    fileRead("shader/chunk.frag",&fragShaderCode);
    GLuint vertShader , fragShader;
    shaderCompile(GL_VERTEX_SHADER,vertShaderCode,&vertShader);
    shaderCompile(GL_FRAGMENT_SHADER,fragShaderCode,&fragShader);

    GLuint shader = glCreateProgram();
    glAttachShader(shader,vertShader);
    glAttachShader(shader,fragShader);
    glLinkProgram(shader);

    GLint ok;
    glGetProgramiv(shader,GL_LINK_STATUS,&ok);
    if(!ok){
        char log[512];
        glGetProgramInfoLog(shader,sizeof(log),NULL,log);
        PANIC("Link error:\n%s\n",log);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    free(vertShaderCode);
    free(fragShaderCode);
}
void chunkPipelineBeginDraw(){}
void chunkPipelineEndDraw(){}

void chunkPipelineDestroy(struct ChunkPipeline* self){}

struct Renderer {};
void rendererCreate() {}

void rendererDestroy() {}

void rendererSetup();
void rendererShutdown();
void rendererBeginFrame();
void rendererEndFrame();
void rendererSubmit();
