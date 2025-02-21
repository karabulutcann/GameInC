#include "input.h"
#include <cglm/cglm.h>

struct InputKeyState
{
    Bool keyW;
    Bool keyS;
    Bool keyA;
    Bool keyD;
    Bool keySpace;
    Bool keyESC;
    Bool keyLeftShift;
};

static struct InputKeyState staticKeyState;

static Bool firstMouse = TRUE;
static float lastX = 0, lastY = 0;

void inputProcess(struct Window *window, f4 deltaTime, struct Camera *camera)
{
    if (inputGetKeyPressedOnce(window, KEY_ESC))
        windowToggleMouseLock(window);

    if (window->isMouseLocked)
    {
        f4 xpos = windowGetMouseX(window);
        f4 ypos = windowGetMouseY(window);
        if (firstMouse && xpos != 0 && ypos != 0) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = FALSE;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;
        cameraUpdateDirection(camera, xoffset, yoffset);
    }

    if (windowGetKey(window, KEY_LEFT_SHIFT) == KEY_DOWN)
    {
        camera->movementSpeed = 50.0f;
    }
    else
    {
        camera->movementSpeed = 5.0f;
    }

    const f4 cameraSpeed = camera->movementSpeed * deltaTime; // adjust accordingly
    if (windowGetKey(window, KEY_SPACE) == KEY_DOWN)
    {
        windowSetShouldClose(window, TRUE);
        // vec3 tmp = {0.0f, 0.0f, 0.0f};
        // glm_vec3_scale(camera->up, cameraSpeed, tmp);
        // glm_vec3_add(camera->position, tmp, camera->position);
    }
    if (windowGetKey(window, KEY_W) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_scale(camera->front, cameraSpeed, tmp);
        glm_vec3_add(camera->position, tmp, camera->position);
    }

    if (windowGetKey(window, KEY_S) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_scale(camera->front, cameraSpeed, tmp);
        glm_vec3_sub(camera->position, tmp, camera->position);
    }
    if (windowGetKey(window, KEY_A) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_cross(camera->front, camera->up, tmp);
        glm_normalize(tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_sub(camera->position, tmp, camera->position);
    }
    if (windowGetKey(window, KEY_D) == KEY_DOWN)
    {
        vec3 tmp = {0.0f, 0.0f, 0.0f};
        glm_vec3_cross(camera->front, camera->up, tmp);
        glm_normalize(tmp);
        glm_vec3_scale(tmp, cameraSpeed, tmp);
        glm_vec3_add(camera->position, tmp, camera->position);
    }
}

Bool inputGetKeyPressedOnce(struct Window *window, enum InputKey key)
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