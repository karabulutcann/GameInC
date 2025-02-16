#pragma once
#include "core/core.h"
#include "core/result.h"
#include "core/log.h"


struct Result imageLoad(const char *path, Bool hasAlpha,unsigned int *texture);