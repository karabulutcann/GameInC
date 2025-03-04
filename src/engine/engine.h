#pragma once
#include "core/result.h"
#include "window.h"
#include "shader.h"

struct CubeDefferedRenderer {
    struct Shader geometryPass;
    struct Shader lightingPass;
    GlFrameBuffer fbo;
    GlRenderBuffer depthBuffer;
    GlTexture position;
    GlTexture normal;
    GlTexture albedoSpec;
    GlVertexBuffer quadVBO;
};

struct Engine{
    struct Window window;
    struct CubeDefferedRenderer cubeDefferedRenderer;
    f4 deltaTime;
    f4 lastFrame;
};

struct Result engineCreate(struct Engine * dest);

struct Result engineUpdate(struct Engine * self, mat4 projection, mat4 view);

struct Result engineDraw(struct Engine * self);

struct Result engineDestroy(struct Engine * self);
