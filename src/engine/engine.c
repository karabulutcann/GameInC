#include "engine.h"
#include "core/log.h"
#include "camera.h"

const float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

float quadVertices[] = {
    // positions        // texture Coords
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
};

#define sizeVertex 0.2f

const float cubeHighlightVertices[] = {
    // Back face
    -sizeVertex, -sizeVertex, -sizeVertex, // bottom left
    sizeVertex, -sizeVertex, -sizeVertex,  // bottom right
    sizeVertex, sizeVertex, -sizeVertex,   // top right
    -sizeVertex, sizeVertex, -sizeVertex,  // top left

    // Front face
    -sizeVertex, -sizeVertex, sizeVertex, // bottom left
    sizeVertex, -sizeVertex, sizeVertex,  // bottom right
    sizeVertex, sizeVertex, sizeVertex,   // top right
    -sizeVertex, sizeVertex, sizeVertex,  // top left

    // Left face
    -sizeVertex, sizeVertex, sizeVertex,   // top-right
    -sizeVertex, sizeVertex, -sizeVertex,  // top-left
    -sizeVertex, -sizeVertex, -sizeVertex, // bottom-left
    -sizeVertex, -sizeVertex, sizeVertex,  // bottom-right

    // Right face
    sizeVertex, sizeVertex, sizeVertex,   // top-left
    sizeVertex, -sizeVertex, sizeVertex,  // bottom-left
    sizeVertex, -sizeVertex, -sizeVertex, // bottom-right
    sizeVertex, sizeVertex, -sizeVertex,  // top-right

    // Bottom face
    -sizeVertex, -sizeVertex, -sizeVertex, // top-right
    sizeVertex, -sizeVertex, -sizeVertex,  // top-left
    sizeVertex, -sizeVertex, sizeVertex,   // bottom-left
    -sizeVertex, -sizeVertex, sizeVertex,  // bottom-right

    // Top face
    -sizeVertex, sizeVertex, -sizeVertex, // top-left
    sizeVertex, sizeVertex, -sizeVertex,  // top-right
    sizeVertex, sizeVertex, sizeVertex,   // bottom-right
    -sizeVertex, sizeVertex, sizeVertex,  // bottom-left
};

GLuint cubeHighlightIndices[] = {
    // Back face
    0, 1,
    1, 2,
    2, 3,
    3, 0,

    // Front face
    4, 5,
    5, 6,
    6, 7,
    7, 4,

    // Left face
    0, 4,
    1, 5,
    2, 6,
    3, 7};

#define crosshairSize 0.02f

GLfloat crosshairVertices[] = {
    -crosshairSize, -crosshairSize, 0.0f, // bottom left
    crosshairSize, -crosshairSize, 0.0f,  // bottom right
    crosshairSize, crosshairSize, 0.0f,   // top right
    crosshairSize, crosshairSize, 0.0f,   // top right
    -crosshairSize, crosshairSize, 0.0f,  // top left
    -crosshairSize, -crosshairSize, 0.0f, // bottom left
};

static struct Engine *staticEngine;

struct Engine *engineGet()
{
    if (staticEngine == NULL)
    {
        mError("Engine not initialized");
        exit(1);
    }
    return staticEngine;
}

struct Result cubeDefferedRendererCreate(struct Engine engine, struct CubeDefferedRenderer *dest)
{
    shaderCreate("shaders/g_buffer_vertex.glsl", "shaders/g_buffer_fragment.glsl", NULL, &dest->geometryPass);
    shaderCreateTexture(&dest->geometryPass, "assets/textures/texture_atlas.png", "textureAtlas");
    shaderCreate("shaders/deffered_vertex.glsl", "shaders/deffered_fragment.glsl", NULL, &dest->lightingPass);

    dest->geometryPass.meshes = dynamicArrayCreate(DATA_TYPE_MESH, MAX_MESH_COUNT, sizeof(struct Mesh));
    dest->geometryPass.meshCount = MAX_MESH_COUNT;
    for (index_t i = 0; i < MAX_MESH_COUNT; i++)
    {
        meshCreate(dest->geometryPass.uniformCount,
                   dest->geometryPass.uniforms,
                   3, (struct ShaderBufferBinding[]){{.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 8, .offset = 0}, 
                   {.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 8, .offset = (GLvoid*)(sizeof(float) * 3)},
                   {.valueCount = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 8, .offset = (GLvoid*)(sizeof(float) * 6)}},
                   dynamicArrayGet(dest->geometryPass.meshes, i));
    }

    glCreateFramebuffers(1, &dest->fbo);
    // position color buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &dest->position);
    glTextureStorage2D(dest->position, 1, GL_RGBA16F, engine.window.width, engine.window.height);
    glTextureParameteri(dest->position, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->position, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT0, dest->position, 0);
    shaderSetInt(&dest->lightingPass, "gPosition", 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &dest->normal);
    glTextureStorage2D(dest->normal, 1, GL_RGBA16F, engine.window.width, engine.window.height);
    glTextureParameteri(dest->normal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->normal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT1, dest->normal, 0);
    shaderSetInt(&dest->lightingPass, "gNormal", 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &dest->albedoSpec);
    glTextureStorage2D(dest->albedoSpec, 1, GL_RGBA32F, engine.window.width, engine.window.height);
    glTextureParameteri(dest->albedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->albedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT2, dest->albedoSpec, 0);
    shaderSetInt(&dest->lightingPass, "gAlbedoSpec", 2);

    // // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glNamedFramebufferDrawBuffers(dest->fbo, 3, attachments);

    // // create and attach depth buffer (renderbuffer)
    glCreateRenderbuffers(1, &dest->depthBuffer);
    glNamedRenderbufferStorage(dest->depthBuffer, GL_DEPTH_COMPONENT, engine.window.width, engine.window.height);
    glNamedFramebufferRenderbuffer(dest->fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dest->depthBuffer);

    // finally check if framebuffer is complete
    // TODO clean the deffered renderer before returning error
    if (glCheckNamedFramebufferStatus(dest->fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteFramebuffers(1, &dest->fbo);
        glDeleteTextures(1, &dest->position);
        glDeleteTextures(1, &dest->normal);
        glDeleteTextures(1, &dest->albedoSpec);
        glDeleteRenderbuffers(1, &dest->depthBuffer);
        mError("failed to create framebuffer");
        exit(1);
    }

    glCreateBuffers(1, &dest->quadVBO);
    glNamedBufferData(dest->quadVBO, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    shaderBindBuffers(&dest->lightingPass, 2, 
        (struct ShaderBufferBinding[]){
            {.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 5, .offset = 0},
             {.valueCount = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 5, .offset = (GLvoid*)(sizeof(float) * 3)}},
                      dest->quadVBO);

    return ok();
}

struct Result cubeDefferedRendererUpdate(struct CubeDefferedRenderer *self, mat4 projection, mat4 view)
{
    glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderPrepareForDraw(&self->geometryPass);
    shaderSetMat4(&self->geometryPass, "projection", projection);
    shaderSetMat4(&self->geometryPass, "view", view);

    struct Mesh *mesh = NULL;
    
    // geometry pass
    for (index_t i = 0; i < self->geometryPass.meshCount; i++)
    {
        mesh = dynamicArrayGet(self->geometryPass.meshes, i);
        if(mesh == NULL || !mesh->isInUse || mesh->vertexCount == 0){
            continue;
        }

        if (!mesh->isCopied && !mesh->isLoading)
        {
            meshCopyData(mesh);
        }

        shaderBindBuffers(&self->geometryPass, mesh->bindingCount, mesh->bufferBindings, mesh->vertexBuffer);
        shaderBindUniforms(&self->geometryPass, mesh->uniformCount, mesh->uniforms);

        if (mesh->isIndexed)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferObject);
            glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // lighting pass
    shaderUse(&self->lightingPass);
    glBindTextureUnit(0, self->position);
    glBindTextureUnit(1, self->normal);
    glBindTextureUnit(2, self->albedoSpec);

    shaderSetVec3(&self->lightingPass, "viewPos", cameraGet()->position);
    shaderSetVec3(&self->lightingPass, "lights[0].Position", (vec3){-0.2f, -1.0f, -0.3f});
    shaderSetVec3(&self->lightingPass, "lights[0].Color", (vec3){1.0f, 1.0f, 1.0f});
    shaderSetFloat(&self->lightingPass, "lights[0].Linear", 0.7f);
    shaderSetFloat(&self->lightingPass, "lights[0].Quadratic", 1.8f);
    shaderSetInt(&self->lightingPass, "lights[0].type", 1);

    glBindVertexArray(self->lightingPass.vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, self->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).

    // this is used for copying the depth buffer to the default framebuffer for rendering with forward rendering but it doesn't work fix it
    //  glBlitFramebuffer(0, 0, engine.window.width, engine.window.height, 0, 0, engine.window.width, engine.window.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return ok();
}

struct Result cubeDefferedRendererDestroy(struct CubeDefferedRenderer *self)
{

    glDeleteFramebuffers(1, &self->fbo);
    glDeleteRenderbuffers(1, &self->depthBuffer);
    glDeleteTextures(1, &self->position);
    glDeleteTextures(1, &self->normal);
    glDeleteTextures(1, &self->albedoSpec);

    // clear quad
    glDeleteBuffers(1, &self->quadVBO);

    shaderDestroy(&self->geometryPass);
    shaderDestroy(&self->lightingPass);
    return ok();
}

struct Result cubeHighlightRendererCreate(struct Engine engine, struct CubeHighlightRenderer *dest)
{
    shaderCreate("shaders/highlight_vertex.glsl", "shaders/highlight_fragment.glsl", NULL, &dest->highlightPass);
    glCreateBuffers(1, &dest->VBO);
    glCreateBuffers(1, &dest->EBO);
    glNamedBufferData(dest->EBO, sizeof(cubeHighlightIndices), cubeHighlightIndices, GL_STATIC_DRAW);
    glNamedBufferData(dest->VBO, sizeof(cubeHighlightVertices), cubeHighlightVertices, GL_STATIC_DRAW);
    shaderBindBuffers(&dest->highlightPass, 1, (struct ShaderBufferBinding[]){{.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 3, .offset = 0}},
                      dest->VBO);
    glLineWidth(2);
    return ok();
}

struct Result cubeHighlightRendererUpdate(struct CubeHighlightRenderer *self, mat4 projection, mat4 view, i4 chunkPos[2], i4 blockPos[3])
{

    shaderUse(&self->highlightPass);
    shaderSetMat4(&self->highlightPass, "projection", projection);
    shaderSetMat4(&self->highlightPass, "view", view);
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){(chunkPos[0] * cubeSize * CHUNK_SIZE_X) + (blockPos[0] * cubeSize), (blockPos[1] * cubeSize), chunkPos[1] * cubeSize * CHUNK_SIZE_Z + (blockPos[2] * cubeSize)});
    shaderSetMat4(&self->highlightPass, "model", model);

    // geometry pass
    glBindVertexArray(self->highlightPass.vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->EBO);

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    return ok();
}

struct Result cubeHighlightRendererDestroy(struct CubeHighlightRenderer *self)
{
    glDeleteBuffers(1, &self->VBO);
    shaderDestroy(&self->highlightPass);
    return ok();
}

struct Result engineCreate(struct Engine *dest)
{
    staticEngine = dest;
    windowCreate(1280, 720, "Engine", &dest->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return mErr("Failed to initialize GLAD");
    }
    glViewport(0, 0, dest->window.width, dest->window.height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);

    cubeDefferedRendererCreate(*dest, &dest->cubeDefferedRenderer);
    cubeHighlightRendererCreate(*dest, &dest->cubeHighlightRenderer);

    shaderCreate(NULL, NULL, NULL, &dest->crosshair);
    dest->crosshair.meshes = dynamicArrayCreate(DATA_TYPE_MESH, 1, sizeof(struct Mesh));
    dest->crosshair.meshCount = 1;
    meshCreate(
        dest->crosshair.uniformCount,
        dest->crosshair.uniforms, 1,
        (struct ShaderBufferBinding[]){
            {.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 3, .offset = 0}},
        dynamicArrayGet(dest->crosshair.meshes, 0));

    glNamedBufferData(dest->crosshair.meshes[0].vertexBuffer, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
    shaderUse(&dest->crosshair);
    shaderSetVec3(&dest->crosshair, "color", (vec3){1.0f, 1.0f, 1.0f});
    mat4 projection2 = GLM_MAT4_IDENTITY_INIT;

    // TODO pencere boyutu degistiginde projection u tekrar hesapla
    //  Assuming windowWidth and windowHeight represent your window size
    float aspectRatio = (float)dest->window.width / (float)dest->window.height;

    // Define the projection based on the aspect ratio
    float left = -2.0f;
    float right = 2.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    // Adjust the orthographic projection based on the aspect ratio
    if (aspectRatio > 1.0f)
    {
        // Wider than tall, scale the vertical axis
        bottom = -2.0f / aspectRatio;
        top = 2.0f / aspectRatio;
    }
    else
    {
        // Taller than wide, scale the horizontal axis
        left = -2.0f * aspectRatio;
        right = 2.0f * aspectRatio;
    }

    // Apply the projection
    glm_ortho(left, right, bottom, top, 0.0f, 100.0f, projection2);

    shaderSetMat4(&dest->crosshair, "projection", projection2);
    shaderBindBuffers(
        &dest->crosshair,
        dest->crosshair.meshes[0].bindingCount,
        dest->crosshair.meshes[0].bufferBindings,
        dest->crosshair.meshes[0].vertexBuffer);

    return ok();
}

struct Result engineUpdate(struct Engine *self, mat4 projection, mat4 view)
{
    // TODO fix this so it shows what error it
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        mError("OpenGL Error: %d\n", err);
    }

    f4 currentFrame = windowGetDeltaTime(&self->window);
    self->deltaTime = currentFrame - self->lastFrame;
    self->lastFrame = currentFrame;
    windowUpdateEvents(&self->window);

    // glClearColor(0.5568f, 0.7019f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    cubeDefferedRendererUpdate(&self->cubeDefferedRenderer, projection, view);

    return ok();
}

struct Result engineDraw(struct Engine *self)
{
    return ok();
}

struct Result engineDestroy(struct Engine *self)
{

    cubeDefferedRendererDestroy(&self->cubeDefferedRenderer);
    windowDestroy(&self->window);
    return ok();
}
