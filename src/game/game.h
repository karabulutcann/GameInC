#include "engine/engine.h";
#include "game/world/chunk.h"
#include "FastNoiseLight/FastNoiseLite.h"

struct Game{
    struct Engine engine;
};

void gameSetup(struct Game* dest){
    engineSetup(&dest->engine);
};

void gameRun(struct Game* self){
    fnl_state noise_state = fnlCreateState();
    noise_state.seed = 45435;                  // You can change the seed for different terrain generation
    noise_state.frequency = 0.009f;            // Adjust frequency for terrain detail
    noise_state.noise_type = FNL_NOISE_PERLIN; // You can experiment with different noise types
    noise_state.fractal_type = FNL_FRACTAL_FBM;
    // 4 den sonrası değişiklik yaratmıyo
    noise_state.octaves = 5;
    noise_state.gain = 0.3f;
    struct Chunk test;
    chunkCreate((i32[2]){0,0},&noise_state,&test);
    chunkBuildMesh(&test,&self->engine.renderer.chunkPipeline);
    while(!windowShouldClose(&self->engine.window)){
        engineBeginUpdate(&self->engine);
    }
}

void gameShutdown(struct Game* self);
