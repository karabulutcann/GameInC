#include "engine.h"
#include "core/log.h"

const float clearColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

float quadVertices[] = {
    // positions        // texture Coords
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
};

struct Result cubeDefferedRendererCreate(struct Engine engine, struct CubeDefferedRenderer *dest)
{
    shaderCreate("shaders/g_buffer_vertex.glsl", "shaders/g_buffer_fragment.glsl", NULL, &dest->geometryPass);
    shaderCreate("shaders/deffered_vertex.glsl", "shaders/deffered_fragment.glsl", NULL, &dest->lightingPass);
    shaderCreateTexture(&dest->geometryPass, "assets/textures/texture_atlas.png", "textureAtlas");

    glCreateFramebuffers(1, &dest->fbo);
    // glGenFramebuffers(1, &dest->gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, dest->fbo);

    // position color buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &dest->position);
    glTextureStorage2D(dest->position, 1, GL_RGBA16F, engine.window.width, engine.window.height);
    glTextureParameteri(dest->position, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->position, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT0, dest->position, 0);
    shaderSetInt(&dest->geometryPass, "gPosition", 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &dest->normal);
    glTextureStorage2D(dest->normal, 1, GL_RGBA16F, engine.window.width, engine.window.height);
    glTextureParameteri(dest->normal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->normal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT1, dest->normal, 0);
    shaderSetInt(&dest->geometryPass, "gNormal", 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &dest->albedoSpec);
    glTextureStorage2D(dest->albedoSpec, 1, GL_RGBA, engine.window.width, engine.window.height);
    glTextureParameteri(dest->albedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(dest->albedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(dest->fbo, GL_COLOR_ATTACHMENT2, dest->albedoSpec, 0);
    shaderSetInt(&dest->geometryPass, "gAlbedoSpec", 2);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glNamedFramebufferDrawBuffers(dest->fbo, 3, attachments);

    // create and attach depth buffer (renderbuffer)
    glCreateRenderbuffers(1, &dest->depthBuffer);
    glNamedRenderbufferStorage(dest->depthBuffer, GL_DEPTH_COMPONENT, engine.window.width, engine.window.height);
    glNamedFramebufferRenderbuffer(dest->fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dest->depthBuffer);

    // finally check if framebuffer is complete
    // TODO clean the deffered renderer before returning error
    if (glCheckNamedFramebufferStatus(dest->fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        glDeleteFramebuffers(1, &dest->fbo);

    glDeleteTextures(1, &dest->position);
    glDeleteTextures(1, &dest->normal);
    glDeleteTextures(1, &dest->albedoSpec);
    glDeleteRenderbuffers(1, &dest->depthBuffer);
    return mErr("Framebuffer is not complete!\n");

    glCreateBuffers(1, &dest->quadVBO);
    glNamedBufferData(dest->quadVBO, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    shaderBindBuffers(&dest->lightingPass, 2, (struct ShaderBufferBinding[]){{.valueCount = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 5, .offset = 0}, {.valueCount = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(float) * 5, .offset = sizeof(float) * 3}},
                      dest->quadVBO);

    return ok();
}

struct Result cubeDefferedRendererUpdate(struct CubeDefferedRenderer *self, mat4 projection, mat4 view)
{
    glClearNamedFramebufferfv(self->fbo, GL_COLOR, 0, clearColor);
    glClearNamedFramebufferfi(self->fbo, GL_DEPTH, 0, 1.0f, 0);

    shaderSetMat4(&self->geometryPass, "projection", projection);
    shaderSetMat4(&self->geometryPass, "view", view);

    shaderPrepareForDraw(&self->geometryPass);

    // geometry pass
    for (index_t i = 0; i < self->geometryPass.meshCount; i++)
    {
        shaderSetMat4(&self->geometryPass, "model", self->geometryPass.meshes[i].model);

        glBindVertexArray(self->geometryPass.vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, self->geometryPass.meshes[i].vertexBuffer);

        for (index_t j = 0; j < self->geometryPass.meshes[i].bindingCount; j++)
        {
            glEnableVertexArrayAttrib(self->geometryPass.vertexArrayObject, j);
            glVertexArrayAttribFormat(self->geometryPass.vertexArrayObject, j, self->geometryPass.meshes[i].bufferBindings[j].valueCount, self->geometryPass.meshes[i].bufferBindings[j].type, self->geometryPass.meshes[i].bufferBindings[j].normalized, self->geometryPass.meshes[i].bufferBindings[j].offset);
            glVertexArrayVertexBuffer(self->geometryPass.vertexArrayObject, j, self->geometryPass.meshes[i].vertexBuffer, self->geometryPass.meshes[i].bufferBindings[j].offset, self->geometryPass.meshes[i].bufferBindings[j].stride);
        }

        if (self->geometryPass.meshes[i].isIndexed)
        {
            glDrawElements(GL_TRIANGLES, self->geometryPass.meshes[i].indexCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, self->geometryPass.meshes[i].vertexCount);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lighting pass
    shaderUse(&self->lightingPass);
    glBindTextureUnit(0, self->position);
    glBindTextureUnit(1, self->normal);
    glBindTextureUnit(2, self->albedoSpec);

    glBindVertexArray(self->quadVBO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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

struct Result engineCreate(struct Engine *dest)
{
    windowCreate(1200, 800, "Demo", &dest->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return mErr("Failed to initialize GLAD");
    }
    glViewport(0, 0, dest->window.width, dest->window.height);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // cubeDefferedRendererCreate(*dest, &dest->cubeDefferedRenderer);

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

    // cubeDefferedRendererUpdate(&self->cubeDefferedRenderer, projection, view);

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
