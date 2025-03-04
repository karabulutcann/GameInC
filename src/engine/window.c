#include "window.h"
#include "core/log.h"
#include "core/result.h"
// TODO avoid using string.h
#include "string.h"
#include <stdlib.h>

// TODO change this to handle multiple windows
static struct Window *staticWindow;

static f4 mousePosX = 0.0f;
static f4 mousePosY = 0.0f;

// TODO move input logic into its on file

// always use else if to avoid multiple windowing libraries
#ifdef USE_GLFW

// TODO dont use calbacks instead use a error getter and use it in mErr
void glfwErrorCallback(int error, const char *description)
{
    mError("GLFW Error [%d]:  %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    staticWindow->width = width;
    staticWindow->height = height;
    glViewport(0, 0, width, height);
}

void mouseCallbackGLFW(GLFWwindow *window, double xpos, double ypos)
{
    mousePosX = xpos;
    mousePosY = ypos;
}
#endif

struct Result windowCreate(int width, int height, const char *title, struct Window *dest)
{
    staticWindow = dest;
    dest->width = width;
    dest->height = height;
    dest->title = (char *)malloc(strlen(title) + 1);
    dest->isMouseLocked = FALSE;
    strcpy(dest->title, title);
    
#ifdef USE_GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        // Dont forget to terminate glfw and free the title before using mErr
        return mErr("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *test = glfwCreateWindow(width, height, title, NULL, NULL);
    dest->windowHandle = test;
    if (dest->windowHandle == NULL)
    {
        // Dont forget to terminate glfw and free the title before using mErr
        return mErr("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(dest->windowHandle);
    glfwSetFramebufferSizeCallback(dest->windowHandle, framebuffer_size_callback);
    windowToggleMouseLock(dest);
    glfwSetCursorPosCallback(dest->windowHandle, mouseCallbackGLFW);

    mDebug("w close %d", glfwWindowShouldClose(dest->windowHandle));

#elif defined(USE_SDL)
    ERROR("SDL not implemented yet");
    exit(1);
#elif defined(USE_WINAPI)
    ERROR("WinAPI not implemented yet");
    exit(1);
#else
    ERROR("No windowing library defined please define USE_GLFW, USE_SDL, or USE_WINAPI");
    exit(1);
#endif
    return ok();
}

void windowToggleMouseLock(struct Window *window)
{
    window->isMouseLocked = !window->isMouseLocked;
#ifdef USE_GLFW
    glfwSetInputMode(window->windowHandle, GLFW_CURSOR, window->isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
#endif
}

// TODO change this return value
Bool windowShouldClose(struct Window *window)
{
#ifdef USE_GLFW
    return glfwWindowShouldClose(window->windowHandle);
#endif
}

void windowSetShouldClose(struct Window *window, Bool shouldClose)
{
    glfwSetWindowShouldClose(window->windowHandle, shouldClose);
}

struct Result windowUpdateEvents(struct Window *window)
{
#ifdef USE_GLFW
    glfwPollEvents();
#endif
    return ok();
}

Bool windowGetKey(struct Window *window, enum InputKey key)
{
#ifdef USE_GLFW
    switch (key)
    {
    case KEY_W:
        return glfwGetKey(window->windowHandle, GLFW_KEY_W) == GLFW_PRESS;
    case KEY_S:
        return glfwGetKey(window->windowHandle, GLFW_KEY_S) == GLFW_PRESS;
    case KEY_A:
        return glfwGetKey(window->windowHandle, GLFW_KEY_A) == GLFW_PRESS;
    case KEY_D:
        return glfwGetKey(window->windowHandle, GLFW_KEY_D) == GLFW_PRESS;
    case KEY_SPACE:
        return glfwGetKey(window->windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS;
    case KEY_ESC:
        return glfwGetKey(window->windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    case KEY_LEFT_SHIFT:
        return glfwGetKey(window->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    case MOUSE_LEFT:
        return glfwGetMouseButton(window->windowHandle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    case MOUSE_RIGHT:
        return glfwGetMouseButton(window->windowHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    default:
        return FALSE;
    }
#endif
}

f4 windowGetMouseX(struct Window* window){
    return mousePosX;
}

f4 windowGetMouseY(struct Window* window){
    return mousePosY;
}

// TODO make a macro to create a switch case for every key
struct Result windowSwapBuffers(struct Window *window)
{
#ifdef USE_GLFW
    glfwSwapBuffers(window->windowHandle);
#endif
    return ok();
}
f4 windowGetDeltaTime(struct Window* window){
    return (f4)glfwGetTime();
}

struct Result windowDestroy(struct Window *window)
{
    free(window->title);
#ifdef USE_GLFW
    glfwTerminate();
#endif
}

