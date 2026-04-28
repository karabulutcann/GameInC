#include "engine/renderer.h"
#include "core/assert.h"
#include "core/file.h"
#include "core/types.h"
#include "defines.h"

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

void chunkMeshCreate(f32* mesh,countT meshLength,struct ChunkMesh* dest){
    ASSERT(mesh != NULL && dest != NULL, "Invalid arguments");

    dest->indexCount = meshLength;
    dest->ebo = 0;
    glm_mat4_identity(dest->model);
    glGenVertexArrays(1,&dest->vao);
    glGenBuffers(1,&dest->vbo);
    // glGenBuffers(1,&dest->ebo);

    glBindVertexArray(dest->vao);
    glBindBuffer(GL_ARRAY_BUFFER,dest->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(f32) * meshLength, mesh, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(f32) * 3,(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}


void chunkPipelineCreate(struct ChunkPipeline* dest){
    dest->chunkMeshCount = 0;
    char* vertShaderCode = NULL;
    fileRead("shaders/chunk.vert",&vertShaderCode);
    char* fragShaderCode = NULL;
    fileRead("shaders/chunk.frag",&fragShaderCode);
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
    dest->shader = shader;
    dest->modelViewProjUniformLoc = glGetUniformLocation(shader,"modelViewProj");
    // ASSERT(dest->modelViewProjUniformLoc != -1,"")
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    free(vertShaderCode);
    free(fragShaderCode);
}

void chunkPipelineCreateChunkMesh(struct ChunkPipeline* self,f32* mesh,countT meshLength,indexT* chunkMeshID){
    ASSERT(self->chunkMeshCount < MAX_CHUNK_TO_RENDER,"Max mesh count is exceed");
    chunkMeshCreate(mesh, meshLength, self->chunkMeshes + self->chunkMeshCount);
    *chunkMeshID = self->chunkMeshCount;
    self->chunkMeshCount++;

}
void chunkPipelineBeginDraw(struct ChunkPipeline* self,mat4 viewProj){
    glUseProgram(self->shader);
    for(int i=0;i< self->chunkMeshCount;i++){
          mat4 modelViewProj = GLM_MAT4_IDENTITY_INIT;
          glm_mat4_mul(viewProj,self->chunkMeshes[i].model,modelViewProj);
          glUniformMatrix4fv(self->modelViewProjUniformLoc,1,false,(f32*) modelViewProj);

          glBindVertexArray(self->chunkMeshes[i].vao);

          glDrawArrays(GL_TRIANGLES,0,self->chunkMeshes[i].indexCount);
    }
    
}
void chunkPipelineEndDraw(){}

void chunkPipelineDestroy(struct ChunkPipeline* self){}

void rendererCreate(struct Renderer* dest) {
  chunkPipelineCreate(&dest->chunkPipeline);
}

void rendererDestroy() {}

void rendererBeginFrame(struct Renderer* self, mat4 viewProj){
  glClearColor(0.5f,0.8f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  chunkPipelineBeginDraw(&self->chunkPipeline,viewProj);
}
void rendererEndFrame();
void rendererSubmit();
