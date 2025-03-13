#pragma once
#include "core/core.h"

enum DataType
{
    DATA_TYPE_U1,
    DATA_TYPE_I1,
    DATA_TYPE_U2,
    DATA_TYPE_I2,
    DATA_TYPE_U4,
    DATA_TYPE_I4,
    DATA_TYPE_F4,
    DATA_TYPE_F8,
    DATA_TYPE_F10,
    DATA_TYPE_MESH,
};

size_t dataTypeGetSize(enum DataType type);