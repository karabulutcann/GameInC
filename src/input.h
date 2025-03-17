#pragma once
#include "core/core.h"
#include "engine/window.h"
#include "camera.h"
#include <cglm/cglm.h> 

void inputProcess(struct Window *window, f4 deltaTime, struct Camera *camera);

Bool inputGetKeyPressedOnce(struct Window *window, enum InputKey key);

void mouseCallback(struct Window* window,struct Camera camera, double xpos, double ypos);
