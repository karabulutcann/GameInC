#include "core/assert.h"
#include "core/types.h"
#include <stdlib.h>

#define BLOCK_SIZE 0.2f
const f32 CUBE_VERTICES[] = {
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

void blockBuildMesh(f32* mesh,sizeT* totalWritten,i16 blockPos[3]){
    ASSERT(mesh != NULL && totalWritten != NULL,"Invalid parameters");
    for(indexT i=0;i<sizeof(CUBE_VERTICES) / sizeof(f32);i++){
        mesh[i] = CUBE_VERTICES[i] + blockPos[i % 3];
        *totalWritten += sizeof(CUBE_VERTICES[0]);
    }
}
