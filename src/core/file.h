#pragma once
#include "result.h"

struct Result fileRead(const char *path, char **buffer);

struct Result fileWrite(const char *path, char *buffer);
