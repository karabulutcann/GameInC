#pragma once
#include "core/core.h"
#include "engine/window.h"
#include "camera.h"
#include <cglm/cglm.h>

enum InputKey
{
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_SPACE,
    KEY_ESC,
    KEY_LEFT_SHIFT,
    MOUSE_LEFT,
    MOUSE_RIGHT,
};

void inputProcess(struct Window* window, f4 deltaTime, struct Camera* camera);

Bool inputGetKeyPressedOnce(struct Window *window, enum InputKey key);

void mouseCallback(struct Window* window,struct Camera camera, double xpos, double ypos);