#include "engine.h"
#include "core/log.h"
#include <stb_image.h>

struct Result engineCreate(struct Engine * dest){
    windowCreate(1280, 720, "ENGINE", &dest->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return mErr("Failed to initialize GLAD");
    }
    
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    return ok();
}


struct Result engineUpdate(struct Engine * self){

    //TODO fix this so it shows what error it
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

    return ok();
}

struct Result engineDraw(struct Engine * self){
    
    return ok();
}

struct Result engineDestroy(struct Engine * self){
    return ok();
}
