#include "core/types.h"

#define BLOCK_SIZE 0.2f

const float CUBE_VERTICES[] = {
    // Back face
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE, // bottom left
    BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,    // top right
    BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,  // bottom right
    BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,   // top right
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE, // bottom left
    -BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,   // top left

    // front face
    -BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,  // bottom left
    BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,   // bottom right
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,   // top right
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,   // top right
    -BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,  // top left
    -BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,  // bottom left

    // left face
    -BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,   // top-right
    -BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE, // top-left
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,  // bottom-left
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,  // bottom-left
    -BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,  // bottom-right
    -BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,    // top-right

    // right face
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,  // top-left
    BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE, // bottom-right
    BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,   // top-right
    BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE, // bottom-right
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,   // top-left
    BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,   // bottom-left

    // bottom face
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,  // top-right
    BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,   // top-left
    BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,  // bottom-left
    BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,    // bottom-left
    -BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE,   // bottom-right
    -BLOCK_SIZE, -BLOCK_SIZE, -BLOCK_SIZE,  // top-right

    // top face
    -BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE, // top-left
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,    // bottom-right
    BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,   // top-right
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,   // bottom-right
    -BLOCK_SIZE, BLOCK_SIZE, -BLOCK_SIZE,  // top-left
    -BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, // bottom-left
};

void blockBuildMesh(f32* mesh,countT* totalWritten){

}
