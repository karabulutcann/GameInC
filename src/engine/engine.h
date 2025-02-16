#pragma once
#include "core/result.h"
#include "window.h"

struct Engine{
    struct Window window;

    f4 deltaTime;
    f4 lastFrame;
};

struct Result engineCreate(struct Engine * dest);

struct Result engineUpdate(struct Engine * self);

struct Result engineDraw(struct Engine * self);

struct Result engineDestroy(struct Engine * self);
