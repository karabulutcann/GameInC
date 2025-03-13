#include "data_types.h"
#include "core/core.h"
#include "engine/mesh.h"

size_t dataTypeGetSize(enum DataType type)
{
    switch (type)
    {
    case DATA_TYPE_U1:
        return sizeof(u1);
    case DATA_TYPE_I1:
        return sizeof(i1);
    case DATA_TYPE_U2:
        return sizeof(u2);
    case DATA_TYPE_I2:
        return sizeof(i2);
    case DATA_TYPE_U4:
        return sizeof(u4);
    case DATA_TYPE_I4:
        return sizeof(i4);
    case DATA_TYPE_F4:
        return sizeof(f4);
    case DATA_TYPE_F8:
        return sizeof(f8);
    case DATA_TYPE_F10:
        return sizeof(f10);
    case DATA_TYPE_MESH:
        return sizeof(struct Mesh);
    default:
        return 0;
    }
}