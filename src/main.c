#include "core/core.h"
#include "engine/window.h"
#include "core/log.h"
#include "core/result.h"
#include "engine/shader.h"
#include "camera.h"
#include "world/world.h"
#include "world/chunk_generator.h"
#include "engine/engine.h"
#include "core/worker/job.h"
#include "core/worker/worker.h"
#include "core/worker/boss.h"
#include "input.h"
#include <math.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

volatile struct Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 lightPos = {0.f, 50.0f, 0.0f};

#define cubeSize 0.4f
#define LOAD_RADIUS 5

int main()
{
    struct Boss boss = {0};
    bossCreate(&boss);
    bossHireWorker(&boss);
    // bossHireWorker(&boss);

    // TODO stack overflow oluyo arraylar cok buyuk tum buyuk arraylari heapden olustur
    struct Engine engine = {0};
    engineCreate(&engine);

    struct World world = {0};
    worldCreate(&world);

    struct ChunkGenerator chunkGenerator = {0};
    chunkGeneratorCreate(&chunkGenerator);

    struct Chunk chunk = {0};
    chunkCreate((i4[2]){0, 0}, &chunk);
    chunkTableInsert(world.chunkTable, (i4[2]){0, 0}, chunk);
    chunkGeneratorGenerateMesh(&chunkGenerator, &world, (i4[2]){0, 0});

    // for (index_t x = -LOAD_DISTANCE; x < LOAD_DISTANCE * 2 + 1; x++)
    // {
    //     for (index_t z = -LOAD_DISTANCE; z < LOAD_DISTANCE * 2 + 1; z++)
    //     {
    //         bossAssignJob(&boss, (struct Job){.func = staticWorldLoadChunk, .data = (void *)(i4[2]){x, z}, .type = LOAD_CHUNK});
    //     }
    // }

    // for (index_t x = -LOAD_DISTANCE; x < LOAD_DISTANCE * 2 + 1; x++)
    // {
    //     for (index_t z = -LOAD_DISTANCE; z < LOAD_DISTANCE * 2 + 1; z++)
    //     {
    //         bossAssignJob(&boss, (struct Job){.func = NULL, .data = (void *)(i4[2]){0, 0}, .type = GENERATE_MESH});
    //     }
    // }

    CACHE_RESULT(mCameraCreate(&camera));

    mDebug("World loaded\n");

    while (!windowShouldClose(&engine.window))
    {
        struct Chunk *chunk = chunkTableGet(world.chunkTable, (i4[2]){0, 0});
        RaycastHit hit = RaycastBlock((vec3){camera.position[0] + 0.00f, camera.position[1] + 0.4f, camera.position[2]}, camera.front, chunk);

        if (inputGetKeyPressedOnce(&engine.window, MOUSE_LEFT) && engine.window.isMouseLocked)
        {
            if (hit.hit)
            {
                mDebug("hit to %d\n", hit.index);
                chunk->blockTypeArr[hit.index] = 0;
                bossAssignJob(&boss, (struct Job){
                                         .data = (i4[2]){0, 0},
                                         .type = GENERATE_MESH});
                // worldGenerateChunkMesh(&world, (i4[2]){0, 0});
            }
        }

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        glm_perspective(glm_rad(45.0f), (float)engine.window.width / (float)engine.window.height, 0.1f, 300.0f, projection);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        CACHE_RESULT(cameraLookAt(camera, view));

        inputProcess(&engine.window, engine.deltaTime, &camera);
        engineUpdate(&engine, projection, view);

        glDisable(GL_DEPTH_TEST);

        if (hit.hit)
        {
            cubeHighlightRendererUpdate(&engine.cubeHighlightRenderer, projection, view, (i4[2]){0, 0}, (i4[3]){hit.blockPos.x, hit.blockPos.y, hit.blockPos.z});
        }

        shaderUse(&engine.crosshair);
        glBindVertexArray(engine.crosshair.vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        windowSwapBuffers(&engine.window);
    }

    // mDebug("World destroy");
    // worldDestroy(&world);

    // mDebug("Engine destroy");
    // engineDestroy(&engine);

    return 0;
}

// float tangents[12 * 3] = {0};
// float bitangents[12 * 3] = {0};

// for (int i = 0; i < 12; i++)
// {
//     vec3 v1 = {vertices[i * 24], vertices[i * 24 + 1], vertices[i * 24 + 2]};
//     vec3 v2 = {vertices[i * 24 + 8], vertices[i * 24 + 8 + 1], vertices[i * 24 + 8 + 2]};
//     vec3 v3 = {vertices[i * 24 + 16], vertices[i * 24 + 16 + 1], vertices[i * 24 + 16 + 2]};

//     vec2 uv1 = {vertices[i * 24 + 6], vertices[i * 24 + 6 + 1]};
//     vec2 uv2 = {vertices[i * 24 + 8 + 6], vertices[i * 24 + 8 + 6 + 1]};
//     vec2 uv3 = {vertices[i * 24 + 16 + 6], vertices[i * 24 + 16 + 6 + 1]};

//     vec3 edge1 = {0.0f, 0.0f, 0.0f};
//     glm_vec3_sub(v2, v1, edge1);

//     vec3 edge2 = {0.0f, 0.0f, 0.0f};
//     glm_vec3_sub(v3, v1, edge2);

//     vec2 deltaUV1 = {0.0f, 0.0f};
//     glm_vec2_sub(uv2, uv1, deltaUV1);

//     vec2 deltaUV2 = {0.0f, 0.0f};
//     glm_vec2_sub(uv3, uv1, deltaUV2);

//     float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

//     vec3 tempTangent = {
//         f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]),
//         f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]),
//         f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2])};

//     glm_vec3_copy(tempTangent, tangents + i * 3);
//     bitangents[i * 3] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
//     bitangents[i * 3 + 1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
//     bitangents[i * 3 + 2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
// }