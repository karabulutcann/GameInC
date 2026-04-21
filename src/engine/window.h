#pragma once
#include "core/types.h"
#include "GLFW/glfw3.h"

struct Window
{
    int width;
    int height;
    const char *title;
    GLFWwindow *windowHandle;
    bool shouldClose;
};

void windowCreate(int width, int height, const char *title, struct Window *dest);

void windowToggleMouseLock(struct Window * self);

bool windowShouldClose(struct Window *self);

void windowSetShouldClose(struct Window *self, bool shouldClose);

void windowPollEvents(struct Window *self);

void windowSwapBuffers(struct Window *self);

void windowDestroy();
