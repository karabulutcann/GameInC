#pragma once
#include "core/result.h"
#include "engine/window.h"
#include "engine/shader.h"

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

struct CubeHighlightRenderer {
    struct Shader highlightPass;
    GlVertexBuffer VBO;
    GlIndexBuffer EBO;
};

struct Result cubeHighlightRendererUpdate(struct CubeHighlightRenderer *self, mat4 projection, mat4 view, i4 chunkPos[2] ,i4 blockPos[3]);

struct Engine{
    struct Window window;
    struct CubeDefferedRenderer cubeDefferedRenderer;
    struct CubeHighlightRenderer cubeHighlightRenderer;
    struct Shader crosshair;
    f4 deltaTime;
    f4 lastFrame;
};
struct Engine *engineGet();

struct Result engineCreate(struct Engine * dest);

struct Result engineUpdate(struct Engine * self, mat4 projection, mat4 view);

struct Result engineDraw(struct Engine * self);

struct Result engineDestroy(struct Engine * self);
