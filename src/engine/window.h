#pragma once
#include "core/core.h"

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#endif


typedef void (* mouseEventCallback)(double xpos, double ypos);

struct EngineWindow
{
    int width;
    int height;
    char *title;
    Bool isMouseLocked;
    mouseEventCallback mouseEventCallbackF;
    GLFWwindow *windowHandle;
};

enum InputKey
{
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_SPACE,
    KEY_ESC,
    KEY_LEFT_SHIFT,
};

struct InputKeyState{
    Bool keyW;
    Bool keyS;
    Bool keyA;
    Bool keyD;
    Bool keySpace;
    Bool keyESC;
    Bool keyLeftShift;
};

#define KEY_DOWN TRUE
#define KEY_UP FALSE

struct Result windowCreate(int width, int height, const char *title, mouseEventCallback mouseCallback, struct EngineWindow *windowDest);

void windowToggleMouseLock(struct EngineWindow * window);

Bool windowShouldClose(struct EngineWindow *window);

struct Result windowHandleEvents(struct EngineWindow *window);

Bool windowGetKey(struct EngineWindow *window, enum InputKey key);

Bool windowGetKeyPressedOnce(struct EngineWindow *window, enum InputKey key);

struct Result windowSwapBuffers(struct EngineWindow *window);

struct Result windowDestroy(struct EngineWindow *window);