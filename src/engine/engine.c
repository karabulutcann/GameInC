#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderer.h"
#include <cglm/cglm.h>
#include <math.h>
#include "core/log.h"
void engineSetup(struct Engine* dest){
    windowCreate(800,600,"MINECRSFT",&dest->window);
    rendererCreate(&dest->renderer);
};

void engineBeginUpdate(struct Engine* self){
    static vec3 pos       = {3.0f, 0.0f, 0.0f};
    static vec3 front     = {0.0f, 0.0f, -1.0f};
    static float yaw      = -90.0f;
    static float pitch    = 0.0f;
    static double lastX   = 400.0, lastY = 300.0;
    static int firstMouse = 1;

    float speed = 5.0f * 0.016f; // approx dt
    GLFWwindow* win = self->window.windowHandle;

    // keyboard movement
    vec3 right;
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_vec3_cross(front, up, right);
    glm_vec3_normalize(right);

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        INFO("window should close");
         windowSetShouldClose(&self->window,true);
    };


    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) glm_vec3_muladds(front, speed, pos);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) glm_vec3_muladds(front, -speed, pos);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) glm_vec3_muladds(right, -speed, pos);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) glm_vec3_muladds(right,  speed, pos);

    // mouse look
    double mx, my;
    glfwGetCursorPos(win, &mx, &my);
    if (firstMouse) { lastX = mx; lastY = my; firstMouse = 0; }
    float dx = (float)(mx - lastX) * 0.1f;
    float dy = (float)(lastY - my) * 0.1f;
    lastX = mx; lastY = my;
    yaw   += dx;
    pitch += dy;
    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    front[0] = cosf(glm_rad(yaw)) * cosf(glm_rad(pitch));
    front[1] = sinf(glm_rad(pitch));
    front[2] = sinf(glm_rad(yaw)) * cosf(glm_rad(pitch));
    glm_vec3_normalize(front);

    // view-projection
    mat4 view, proj, viewProj;
    vec3 center;
    glm_vec3_add(pos, front, center);
    glm_lookat(pos, center, up, view);
    glm_perspective(glm_rad(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f, proj);
    glm_mat4_mul(proj, view, viewProj);

    rendererBeginFrame(&self->renderer, viewProj);
    windowSwapBuffers(&self->window);
    windowPollEvents(&self->window);
};
void engineEndUpdate();

void engineShutdown(struct Engine *self){
    windowDestroy();
};


