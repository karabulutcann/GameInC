#pragma once
#include "engine/window.h"
#include "engine/renderer.h"
#include <cglm/cglm.h>
#include <math.h>

struct Engine {
    struct Window window;
    int input;
    struct Renderer renderer;
    int resourse;
};

void engineSetup(struct Engine* dest);

void engineBeginUpdate(struct Engine* self);
void engineEndUpdate();

void engineShutdown(struct Engine *self);

struct Input{};

void inputSetup();
void inputShutdown();

struct Physic{};

void physicSetup();
void physicShutdown();
void physicsUpdate();
