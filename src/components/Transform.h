#pragma once

#include "../systems/render/opengl/DataType.h"

namespace E4 {

    struct Transform {
        float x, y, z;

        uint64_t lastFrame;
        uint64_t parent;
        ShaderData worldTransform;
    };

}