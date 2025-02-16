#include "core/core.h"
#include "window.h"
#include <math.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/log.h"
#include "core/result.h"
#include "shader.h"
#include "image.h"
#include "camera.h"
#include "world/world.h"

struct Camera camera;
struct EngineWindow window;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

void processInput(struct EngineWindow *window)
{
    if (windowGetKeyPressedOnce(window, KEY_ESC))
        windowToggleMouseLock(window);

    if (windowGetKey(window, KEY_LEFT_SHIFT) == KEY_DOWN)
    {
        camera.movementSpeed = 20.0f;
    }
    else
    {
        camera.movementSpeed = 5.0f;
    }

    const float cameraSpeed = camera.movementSpeed * deltaTime; // adjust accordingly
    if (windowGetKey(window, KEY_SPACE) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_scale(camera.up, cameraSpeed, tmp);
        glm_vec3_add(camera.position, tmp, camera.position);
    }
    if (windowGetKey(window, KEY_W) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_scale(camera.front, cameraSpeed, tmp);
        glm_vec3_add(camera.position, tmp, camera.position);
    }

    if (windowGetKey(window, KEY_S) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_scale(camera.front, cameraSpeed, tmp);
        glm_vec3_sub(camera.position, tmp, camera.position);
    }
    if (windowGetKey(window, KEY_A) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_cross(camera.front, camera.up, tmp);
        glm_normalize(tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_sub(camera.position, tmp, camera.position);
    }
    if (windowGetKey(window, KEY_D) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_cross(camera.front, camera.up, tmp);
        glm_normalize(tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_add(camera.position, tmp, camera.position);
    }
}

float lastX = 400, lastY = 300;
float pitch, yaw = -90.0f;
Bool firstMouse = TRUE;

void mouseCallback(double xpos, double ypos)
{
    if (!window.isMouseLocked)
    {
        if (firstMouse) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        cameraUpdateDirection(&camera, xoffset, yoffset);
    }
}

vec3 lightPos = {0.f, 50.0f, 0.0f};

int main()
{

    CACHE_RESULT(windowCreate(WINDOW_WIDTH, WINDOW_HEIGHT, "ENGINE", mouseCallback, &window), FATAL);

    // TODO make this platform independent and move to its own file
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ERROR("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    struct Shader shader = shaderCreate("shaders/vertex.glsl", "shaders/fragment.glsl", NULL);

    unsigned int texture, normalMap;

    unsigned int cobbleStoneT, cobbleStoneN;

    CACHE_RESULT(imageLoad("assets/textures/dirt.png", FALSE, &texture));
    CACHE_RESULT(imageLoad("assets/textures/dirt_n.png", TRUE, &normalMap));

    CACHE_RESULT(imageLoad("assets/textures/cobblestone_1.png", TRUE, &cobbleStoneT));
    CACHE_RESULT(imageLoad("assets/textures/cobblestone_1_n.png", TRUE, &cobbleStoneN));

    struct World world = {0};
    worldCreate(&world);

    float blockSize = 0.2f;
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // u4 ChunkVertexBuffers[WORLD_SIZE_X * WORLD_SIZE_X];
    // glGenBuffers(WORLD_SIZE_X * WORLD_SIZE_X,ChunkVertexBuffers);
    // for(int i = 0; i < WORLD_SIZE_X * WORLD_SIZE_X; i++){
    //     glBindBuffer(GL_ARRAY_BUFFER, ChunkVertexBuffers[i]);
    //     glBufferData(GL_ARRAY_BUFFER, world.chunkTable[i].vertexCount * sizeof(float), world.chunkTable[i].mesh, GL_STATIC_DRAW);
    // }

    INFO("vertex count %d", world.chunkTable[0].vertexCount);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, world.chunkTable[0].vertexCount * sizeof(float), world.chunkTable[0].mesh, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CACHE_RESULT(mCameraCreate(&camera));

    unsigned int transformLoc = 0;
    shaderUse(shader);
    shaderSetUniformInt(shader, "texture", 0);
    shaderSetUniformInt(shader, "normalMap", 1);

    shaderSetUniformInt(shader, "cobblestoneT", 2);
    shaderSetUniformInt(shader, "cobblestoneN", 3);

    while (!windowShouldClose(&window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        windowHandleEvents(&window);
        processInput(&window);

        glClearColor(0.5568f, 0.7019f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderUse(shader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cobbleStoneT);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cobbleStoneN);

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        glm_perspective(glm_rad(45.0f), (float)window.width / (float)window.height, 0.1f, 100.0f, projection);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        CACHE_RESULT(cameraLookAt(camera, view));

        shaderSetUniformMat4(shader, "projection", projection);
        shaderSetUniformMat4(shader, "view", view);
        shaderSetUniformVec3(shader, "viewPos", camera.position);
        shaderSetUniformVec3(shader, "lightPos", lightPos);
        shaderSetUniformVec3(shader, "lightColor", (vec3){1.0f, 1.0f, 1.0f});

        shaderSetUniformVec3(shader, "objectColor", (vec3){1.0f, 1.0f, 1.0f});
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        shaderSetUniformMat4(shader, "model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, world.chunkTable[0].vertexCount);

        // for(int i = 0; i < WORLD_SIZE_X * WORLD_SIZE_X; i++){
        //     shaderSetUniformVec3(shader, "objectColor", (vec3){1.0f, 1.0f, 1.0f});
        //     mat4 model = GLM_MAT4_IDENTITY_INIT;
        //     shaderSetUniformMat4(shader, "model", model);
        //     glBindVertexArray(VAO);
        //     glBindBuffer(GL_ARRAY_BUFFER, ChunkVertexBuffers[i]);
        //     glDrawArrays(GL_TRIANGLES, 0, world.chunkTable[i].vertexCount);
        // }

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            printf("OpenGL Error: %d\n", err);
        }

        windowSwapBuffers(&window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &normalMap);

    glDeleteTextures(1, &cobbleStoneT);
    glDeleteTextures(1, &cobbleStoneN);

    shaderDestroy(shader);
    windowDestroy(&window);
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