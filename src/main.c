#include "core/core.h"
#include "engine/window.h"
#include <math.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/log.h"
#include "core/result.h"
#include "engine/shader.h"
#include "camera.h"
#include "world/world.h"
#include "engine/engine.h"
#include "input.h"
#include "core/thread.h"

volatile struct Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 lightPos = {0.f, 50.0f, 0.0f};

#define cubeSize 0.4f

volatile Bool threadShouldClose = FALSE;

#define LOAD_RADIUS 20

void getPlayerChunkCoords(struct Camera *camera, i4 chunkCoords[2])
{
    chunkCoords[0] = (int)(camera->position[0] / (cubeSize * CHUNK_SIZE_X));
    chunkCoords[1] = (int)(camera->position[2] / (cubeSize * CHUNK_SIZE_Z));
}

void loadChunksAroundPlayer(struct World *world, struct Camera *camera)
{
    i4 playerChunk[2];
    getPlayerChunkCoords(camera, playerChunk);

    // Define the loading radius (for example, a 3x3 grid of chunks)
    int loadRadius = LOAD_RADIUS;

    // TODO tüm chunkları tek tek gezme farklı bir yöntem bul
    for (int x = (-LOAD_RADIUS / 2); x <= (LOAD_RADIUS / 2); x++)
    {
        for (int z = (-LOAD_RADIUS / 2); z <= (LOAD_RADIUS / 2); z++)
        {
            i4 chunkCoords[2] = {playerChunk[0] + x, playerChunk[1] + z};

            // Check if the chunk is already loaded
            struct Chunk *chunk = chunkTableGet(world->chunkTable, chunkCoords);
            if (chunk == NULL)
            {
                // Load the chunk
                worldLoadChunk(world, chunkCoords);
                worldGenerateChunkMesh(world, chunkCoords);
            }
        }
    }
}

void unloadChunks(struct World *world, struct Camera *camera)
{
    i4 playerChunk[2];
    getPlayerChunkCoords(camera, playerChunk);

    // Define the unloading radius (you may want to keep a few extra chunks loaded)
    int unloadRadius = LOAD_RADIUS;

    // Loop through all chunks and unload those outside the radius
    for (index_t i = 0; i < WORLD_SIZE_X * WORLD_SIZE_X; i++)
    {
        // TODO bunu worldUnloadChunk ta zaten yapıyosun bunu iptal et
        struct Chunk chunk = world->chunkTable[i];
        if (chunk.blockTypeArr != NULL && abs(chunk.position[0] - playerChunk[0]) > unloadRadius || abs(chunk.position[1] - playerChunk[1]) > unloadRadius)
        {
            worldUnloadChunk(world, chunk.position);
        }
    }
}

mThreadCreateFunc(threadFunction, threadData, {
    struct Mutex *mutex = threadData->mutex;
    struct World *world = (struct World*)mutex->sharedState;

    while (!threadShouldClose)
    {
        loadChunksAroundPlayer(world, &camera);
        unloadChunks(world, &camera);
    }

    return 0;
})

int main()
{
    struct ThreadManager threadManager = {0};
    threadManagerCreate(&threadManager);

    struct Engine engine = {0};
    engineCreate(&engine);

    struct Shader chunkShader = {0};
    shaderCreate("shaders/vertex.glsl", "shaders/fragment.glsl", NULL, &chunkShader);
    shaderCreateTexture(&chunkShader, "assets/textures/texture_atlas.png", "textureAtlas");

    struct World world = {0};
    worldCreate(&world);

    struct Mutex worldMutex = {0};
    InitializeCriticalSection(&worldMutex.critSection);
    worldMutex.sharedState = &world;

    threadManagerSpawnThread(&threadManager, &worldMutex, threadFunction);

    CACHE_RESULT(mCameraCreate(&camera));

    mDebug("World loaded\n");

    while (!windowShouldClose(&engine.window))
    {

        engineUpdate(&engine);
        inputProcess(&engine.window, engine.deltaTime, &camera);
        shaderPrepareForDraw(&chunkShader);

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        glm_perspective(glm_rad(45.0f), (float)engine.window.width / (float)engine.window.height, 0.1f, 300.0f, projection);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        CACHE_RESULT(cameraLookAt(camera, view));

        shaderSetUniformMat4(&chunkShader, "projection", projection);
        shaderSetUniformMat4(&chunkShader, "view", view);
        shaderSetUniformVec3(&chunkShader, "viewPos", camera.position);
        shaderSetUniformVec3(&chunkShader, "lightPos", lightPos);
        shaderSetUniformVec3(&chunkShader, "lightColor", (vec3){1.0f, 1.0f, 1.0f});

        shaderSetUniformVec3(&chunkShader, "objectColor", (vec3){1.0f, 1.0f, 1.0f});
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        shaderSetUniformMat4(&chunkShader, "model", model);

        i4 playerChunk[2];
        getPlayerChunkCoords(&camera, playerChunk);

        for (i4 x = (i4)(-LOAD_RADIUS / 2); x <= (i4)(LOAD_RADIUS / 2); x++)
        {
            for (i4 z = (i4)(-LOAD_RADIUS / 2); z <= (i4)(LOAD_RADIUS / 2); z++)
            {
                struct Chunk *chunk = chunkTableGet(world.chunkTable, (i4[2]){playerChunk[0] + x, playerChunk[1] + z});
                if (chunk == NULL || chunk->isLoading == TRUE || chunk->mesh == NULL)
                {
                    continue;
                }

                if (chunk->isVboCreated == FALSE)
                {
                    chunkInitVbo(chunk);
                    chunk->isVboCreated = TRUE;
                }
                shaderSetUniformVec3(&chunkShader, "objectColor", (vec3){1.0f, 1.0f, 1.0f});
                mat4 model = GLM_MAT4_IDENTITY_INIT;
                glm_translate(model, (vec3){chunk->position[0] * cubeSize * CHUNK_SIZE_X, 0.0f, chunk->position[1] * cubeSize * CHUNK_SIZE_Z});
                shaderSetUniformMat4(&chunkShader, "model", model);
                // TODO bunu burda yapma vertex buffer object baglanmadan calismiyor
                // glNamedBuffer foksiyonlarini kullan
                glBindVertexArray(chunkShader.vertexArrayObject);
                glBindBuffer(GL_ARRAY_BUFFER, chunk->vertexBufferObject);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);

                glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
            }
        }

        windowSwapBuffers(&engine.window);
    }

    threadShouldClose = TRUE;

    threadManagerWaitForAll(&threadManager, INFINITE);

    threadManagerDestroy(&threadManager);

    mDebug("World destroy");
    worldDestroy(&world);

    mDebug("Shader destroy");
    shaderDestroy(&chunkShader);

    mDebug("Engine destroy");
    engineDestroy(&engine);
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