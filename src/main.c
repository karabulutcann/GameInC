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
#include "core/worker/boss.h"
#include "input.h"
#include "gui/gui.h"
#include <math.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 lightPos = {-2.0f, 50.0f, -1.0f};

#define LOAD_RADIUS 5

int main()
{
    struct Boss boss = {0};
    bossCreate(&boss);
    bossHireWorker(&boss);
    bossHireWorker(&boss);

    // TODO stack overflow oluyo arraylar cok buyuk tum buyuk arraylari heapden olustur
    struct Engine engine = {0};
    engineCreate(&engine);

    struct World world = {0};
    worldCreate(&world);

    struct ChunkGenerator chunkGenerator = {0};
    chunkGeneratorCreate(&chunkGenerator);

    guiCreate(&engine);

    for (i8 x = -LOAD_DISTANCE; x < LOAD_DISTANCE; x++)
    {
        for (i8 z = -LOAD_DISTANCE; z < LOAD_DISTANCE; z++)
        {

            i4 *data = malloc(sizeof(i4[2]));
            data[0] = x;
            data[1] = z;
            // staticWorldLoadChunk((Allocated)data);
            struct Job *job = jobCreate(staticWorldLoadChunk, (Allocated)data, FALSE, LOAD_CHUNK);
            bossAssignJob(&boss, job);
        }
    }

    bossWaitForAllJobs(&boss);

    for (i8 x = -LOAD_DISTANCE; x < LOAD_DISTANCE; x++)
    {
        for (i8 z = -LOAD_DISTANCE; z < LOAD_DISTANCE; z++)
        {
            i4 *data = malloc(sizeof(i4[2]));
            data[0] = x;
            data[1] = z;
            // chunkGeneratorGenerateMesh(&chunkGenerator, &world, data);
            // meshCopyData(chunkTableGet(world.chunkTable, data)->mesh);
            struct Job *job = jobCreate(NULL, (Allocated)data, FALSE, GENERATE_MESH);
            bossAssignJob(&boss, job);
        }
    }

    CACHE_RESULT(mCameraCreate(&camera));

    mDebug("World loaded\n");

    i4 cursorPos[3] = {0}; 
    Bool isButtonClicked = FALSE;
    int selectedBlock = OAK_LOG;

    while (!windowShouldClose(&engine.window))
    {

        RaycastHit hit = {0};
        hit = RaycastBlock((vec3){camera.position[0], camera.position[1], camera.position[2]}, camera.front);

        if (inputGetKeyPressedOnce(&engine.window, MOUSE_LEFT) && engine.window.isMouseLocked)
        {
            if (hit.hit)
            {
                //FIXME chunk yenilendigi zaman kirilan block chunk sinirinda ise diger chunk yenilenmedigi icin bosluk gozukuyor
                staticWorldSetBlock((i4[3]){hit.blockPos.x,hit.blockPos.y,hit.blockPos.z}, 0);
                i4 *data = malloc(sizeof(i4[2]));
                getChunkPos((i4[3]){hit.blockPos.x,hit.blockPos.y,hit.blockPos.z},data);
                struct Job *job = jobCreate(staticWorldLoadChunk, (Allocated)data, FALSE, GENERATE_MESH);
                bossAssignJob(&boss, job);
            }
        }
        
        if (isButtonClicked)
        {
            staticWorldSetBlock(cursorPos, selectedBlock);
            i4 *data = malloc(sizeof(i4[2]));
            getChunkPos((i4[3]){hit.blockPos.x,hit.blockPos.y,hit.blockPos.z},data);
            struct Job *job = jobCreate(staticWorldLoadChunk, (Allocated)data, FALSE, GENERATE_MESH);
            bossAssignJob(&boss, job);
            isButtonClicked = FALSE;
        }

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        glm_perspective(glm_rad(50.0f), (float)engine.window.width / (float)engine.window.height, 0.1f, 3000.0f, projection);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        CACHE_RESULT(cameraLookAt(camera, view));

        inputProcess(&engine.window, engine.deltaTime, &camera);
        engineUpdate(&engine, projection, view);

        glDisable(GL_DEPTH_TEST);

        if (hit.hit)
        {
            cubeHighlightRendererUpdate(&engine.cubeHighlightRenderer, projection, view, (i4[2]){0, 0}, (i4[3]){hit.blockPos.x, hit.blockPos.y, hit.blockPos.z},(vec3){1,0,0});
        }

        cubeHighlightRendererUpdate(&engine.cubeHighlightRenderer, projection, view, (i4[2]){0, 0}, cursorPos,(vec3){1,0,0});

        shaderUse(&engine.debugShader);
        shaderSetMat4(&engine.debugShader, "projection", projection);
        shaderSetMat4(&engine.debugShader, "view", view);
        glBindVertexArray(engine.debugShader.vertexArrayObject);
        glDrawArrays(GL_LINES, 0, 24);

        shaderUse(&engine.crosshair);
        glBindVertexArray(engine.crosshair.vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        guiUpdate(cursorPos, &isButtonClicked, &selectedBlock, engine.deltaTime);

        windowSwapBuffers(&engine.window);
    }

    // mDebug("World destroy");
    // worldDestroy(&world);

    // mDebug("Engine destroy");
    // engineDestroy(&engine);

    guiDestroy();

    return 0;
}
