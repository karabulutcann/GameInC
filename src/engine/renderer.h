#pragma once
#include "core/types.h"
#include "defines.h"
#include "glad/glad.h"
#include "cglm/cglm.h"

struct ChunkMesh {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLsizei indexCount;
  mat4 model;
};

struct ChunkPipeline {
  GLuint shader;
  GLint modelViewProjUniformLoc;
  struct ChunkMesh chunkMeshes[MAX_CHUNK_TO_RENDER];
  countT chunkMeshCount;
};

void chunkPipelineCreateChunkMesh(struct ChunkPipeline* self,f32* mesh,countT meshLength,indexT* chunkMeshID);



struct Renderer {
  struct ChunkPipeline chunkPipeline;
};


void rendererCreate(struct Renderer* dest);


void rendererBeginFrame(struct Renderer* self, mat4 viewProj);