#include "window.h"
#include "core/log.h"
#include "core/result.h"
// TODO avoid using string.h
#include "string.h"
#include <stdlib.h>
// TODO change this to handle multiple windows
static struct EngineWindow *staticWindow;

// TODO move input logic into its on file
static struct InputKeyState staticKeyState;

// always use else if to avoid multiple windowing libraries
#ifdef USE_GLFW
// TODO dont use calbacks instead use a error getter and use it in mErr
void glfwErrorCallback(int error, const char *description)
{
    ERROR("GLFW Error [%d]:  %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    staticWindow->width = width;
    staticWindow->height = height;
    glViewport(0, 0, width, height);
}

void mouseCallbackGLFW(GLFWwindow *window, double xpos, double ypos)
{
    staticWindow->mouseEventCallbackF(xpos, ypos);
}
#endif

struct Result windowCreate(int width, int height, const char *title, mouseEventCallback mouseCallbackF, struct EngineWindow *windowDest)
{
    staticWindow = windowDest;
    windowDest->width = width;
    windowDest->height = height;
    windowDest->title = (char *)malloc(strlen(title) + 1);
    windowDest->mouseEventCallbackF = mouseCallbackF;
    windowDest->isMouseLocked = FALSE;
    strcpy(windowDest->title, title);
#ifdef USE_GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        // Dont forget to terminate glfw and free the title before using mErr
        return mErr("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *test = glfwCreateWindow(width, height, title, NULL, NULL);
    windowDest->windowHandle = test;
    if (windowDest->windowHandle == NULL)
    {
        // Dont forget to terminate glfw and free the title before using mErr
        return mErr("Failed to create GLFW window");
    }
    DEBUG("windows is after %p", windowDest->windowHandle);
    glfwMakeContextCurrent(windowDest->windowHandle);
    glfwSetFramebufferSizeCallback(windowDest->windowHandle, framebuffer_size_callback);
    windowToggleMouseLock(windowDest);
    glfwSetCursorPosCallback(windowDest->windowHandle, mouseCallbackGLFW);

    DEBUG("w close %d", glfwWindowShouldClose(windowDest->windowHandle));

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

void windowToggleMouseLock(struct EngineWindow *window)
{
    window->isMouseLocked = !window->isMouseLocked;
#ifdef USE_GLFW
    glfwSetInputMode(window->windowHandle, GLFW_CURSOR, window->isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
#endif
}

// TODO change this return value
Bool windowShouldClose(struct EngineWindow *window)
{
#ifdef USE_GLFW
    return glfwWindowShouldClose(window->windowHandle);
#endif
}

struct Result windowHandleEvents(struct EngineWindow *window)
{
#ifdef USE_GLFW
    glfwPollEvents();
#endif
    return ok();
}

Bool windowGetKey(struct EngineWindow *window, enum InputKey key)
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
    default:
        return FALSE;
    }
#endif
}

// TODO make a macro to create a switch case for every key
Bool windowGetKeyPressedOnce(struct EngineWindow *window, enum InputKey key)
{
    switch (key)
    {
    case KEY_W:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyW)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyW = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyW = FALSE;
            return FALSE;
        }
        break;
    case KEY_S:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyS)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyS = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyS = FALSE;
            return FALSE;
        }
        break;
    case KEY_A:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyA)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyA = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyA = FALSE;
            return FALSE;
        }
        break;
    case KEY_D:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyD)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyD = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyD = FALSE;
            return FALSE;
        }
        break;
    case KEY_SPACE:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keySpace)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keySpace = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keySpace = FALSE;
            return FALSE;
        }
        break;
    case KEY_ESC:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyESC)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyESC = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyESC = FALSE;
            return FALSE;
        }
        break;
    case KEY_LEFT_SHIFT:
        if (windowGetKey(window, key))
        {
            if (staticKeyState.keyLeftShift)
            {
                return FALSE;
            }
            else
            {
                staticKeyState.keyLeftShift = TRUE;
                return TRUE;
            }
        }
        else
        {
            staticKeyState.keyLeftShift = FALSE;
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
}

struct Result windowSwapBuffers(struct EngineWindow *window)
{
#ifdef USE_GLFW
    glfwSwapBuffers(window->windowHandle);
#endif
    return ok();
}

struct Result windowDestroy(struct EngineWindow *window)
{
    free(window->title);
#ifdef USE_GLFW
    glfwTerminate();
#endif
}