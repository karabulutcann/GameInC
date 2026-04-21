#pragma once
#include "FastNoiseLight/FastNoiseLite.h"
#include "core/types.h"
#include "game/world/chunk-map.h"

struct World{
    struct ChunkMap chunkMap;
    fnl_state noiseState;
    i32 seed;
};

void worldCreate();

void worldBuildChunkMesh(){

}

void worldDestroy();
