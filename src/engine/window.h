#pragma once
#include "core/core.h"
#include "input.h"

#define USE_GLFW

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#endif

struct Window
{
    int width;
    int height;
    char *title;
    Bool isMouseLocked;
    GLFWwindow *windowHandle;
};

#define KEY_DOWN TRUE
#define KEY_UP FALSE

struct Result windowCreate(int width, int height, const char *title, struct Window *dest);

void windowToggleMouseLock(struct Window * window);

Bool windowShouldClose(struct Window *window);

void windowSetShouldClose(struct Window *window, Bool shouldClose);

Bool windowGetKey(struct Window *window, enum InputKey key);

f4 windowGetMouseX(struct Window* window);

f4 windowGetMouseY(struct Window* window);

struct Result windowUpdateEvents(struct Window *window);

struct Result windowSwapBuffers(struct Window *window);

f4 windowGetDeltaTime(struct Window* window);

struct Result windowDestroy(struct Window *window);