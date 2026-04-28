#include "window.h"
#include "core/assert.h"
#include "glad/glad.h"
// TODO avoid using string.h

// TODO dont use calbacks instead use a error getter and use it in mErr
void glfwErrorCallback(int error, const char *description)
{
    PANIC("GLFW Error [%d]:  %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *self, int width, int height)
{
    glViewport(0, 0, width, height);
}


void windowCreate(int width, int height, const char *title, struct Window *dest)
{
    dest->width = width;
    dest->height = height;
    dest->title = title;

    glfwSetErrorCallback(glfwErrorCallback);

    ASSERT(glfwInit(), "Failed to init GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    dest->windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
    ASSERT(dest->windowHandle,"Failed to open window");

    glfwMakeContextCurrent(dest->windowHandle);
    glfwSetFramebufferSizeCallback(dest->windowHandle, framebuffer_size_callback);
    //glfwSetCursorPosCallback(dest->windowHandle, mouseCallbackGLFW);
    windowToggleMouseLock(dest);
    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load GLAD");
}

// TODO change this return value
bool windowShouldClose(struct Window *self)
{
    return glfwWindowShouldClose(self->windowHandle);
}

void windowSetShouldClose(struct Window *self, bool shouldClose)
{
    glfwSetWindowShouldClose(self->windowHandle, shouldClose);
}

void windowPollEvents(struct Window *self)
{
    glfwPollEvents();
    self->shouldClose = windowShouldClose(self);
}

// TODO make a macro to create a switch case for every key
void windowSwapBuffers(struct Window *self)
{
    glfwSwapBuffers(self->windowHandle);
}

void windowToggleMouseLock(struct Window *self)
{
    int current = glfwGetInputMode(self->windowHandle, GLFW_CURSOR);
    int next = (current == GLFW_CURSOR_DISABLED) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
    glfwSetInputMode(self->windowHandle, GLFW_CURSOR, next);
}

void windowDestroy()
{
    glfwTerminate();
}
