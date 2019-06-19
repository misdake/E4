#pragma once

#include "../systems/render/opengl/DataType.h"

namespace E4 {

    struct Transform {
        float x, y, z;
        float rx, ry, rz;
        float sx, sy, sz;

        uint64_t lastFrame;
        uint32_t parent;
        ShaderData worldTransform;
    };

}