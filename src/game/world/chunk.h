#pragma once
#include "FastNoiseLight/FastNoiseLite.h"
#include "core/types.h"
#include "engine/renderer.h"

struct Chunk {
  u8 *blocks;
  i32 chunkPos[2];
  bool isDirty;
  indexT chunkMeshIndex;
};

void chunkCreate(i32 chunkPos[2], fnl_state *noiseState, struct Chunk *dest);
u8 chunkGetBlock(struct Chunk *self, u16 x, u16 y, u16 z);

void chunkSetBlock(struct Chunk *self, u16 x, u16 y, u16 z, u8 blockID);

void chunkUpdate();

void chunkBuildMesh(struct Chunk *self,struct ChunkPipeline* chunkPipeline);
void chunkDestroy(struct Chunk *self);
