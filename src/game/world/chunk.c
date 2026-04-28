#include "game/world/chunk.h"
#include "FastNoiseLight/FastNoiseLite.h"
#include "core/assert.h"
#include "core/types.h"
#include "defines.h"
#include "game/world/block.h"
#include <cglm/cglm.h>
#include <stdlib.h>
#include "engine/renderer.h"



void chunkCreate(i32 chunkPos[2], fnl_state *noiseState, struct Chunk *dest) {
  dest->chunkPos[0] = chunkPos[0];
  dest->chunkPos[1] = chunkPos[1];
  dest->isDirty = true;
  dest->blocks =
      malloc(sizeof(u8) * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
  ASSERT(dest->blocks != NULL, "Failed to allocate memory!");

  for (u16 z = 0; z < CHUNK_SIZE_Z; z++) {
    for (u16 x = 0; x < CHUNK_SIZE_X; x++) {
      f32 worldX = (chunkPos[0] * CHUNK_SIZE_X + x);
      f32 worldY = (chunkPos[1] * CHUNK_SIZE_Z + z);

      f32 heightSample = fnlGetNoise2D(noiseState, worldX, worldY);
      u16 terrainHeight = 20 + (int)((heightSample * heightSample) * 200);
      for (u16 y = 0; y < CHUNK_SIZE_Y; y++) {
        u8 blockID = 0;
        if (y <= terrainHeight) {
          blockID = 2; // Stone
          if (y > terrainHeight - 3) {
            blockID = 1; // Dirt near surface
            if (y > terrainHeight - 1) {
              blockID = 3; // Grass
            }
          }
        }

        dest->blocks[BLOCK(x, y, z)] = blockID;
      }
    }
  }
}

u8 chunkGetBlock(struct Chunk *self, u16 x, u16 y, u16 z) {
  ASSERT(x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z,
         "Chunk coordinates exceeds chunk borders x: %u y: %u z: %u", x, y, z);
  return self->blocks[BLOCK(x, y, z)];
}

void chunkSetBlock(struct Chunk *self, u16 x, u16 y, u16 z, u8 blockID) {
  ASSERT(x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z,
         "Chunk coordinates exceeds chunk borders x: %u y: %u z: %u", x, y, z);
  self->blocks[BLOCK(x, y, z)] = blockID;
}

void chunkUpdate() {}

void chunkBuildMesh(struct Chunk *self,struct ChunkPipeline* chunkPipeline) {
  f32 *tempMesh =
      malloc(sizeof(f32) * 108 * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
      
  sizeT totalWritten = 0;
  for (u16 y = 0; y < CHUNK_SIZE_Y; y++) {
    for (u16 z = 0; z < CHUNK_SIZE_Z; z++) {
      for (u16 x = 0; x < CHUNK_SIZE_X; x++) {
        if (self->blocks[BLOCK(x, y, z)] != 0) {
          blockBuildMesh(tempMesh + totalWritten,&totalWritten,(i16[]){x,y,z});
        }
      }
    }
  }
  ASSERT(totalWritten % sizeof(f32) == 0,"TotalWritten should be a multiply of f32");
  chunkPipelineCreateChunkMesh(chunkPipeline, tempMesh, totalWritten / sizeof(f32),&self->chunkMeshIndex);
  free(tempMesh);
}

void chunkDestroy(struct Chunk *self) { free(self->blocks); }
