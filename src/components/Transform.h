#pragma once

#include "../systems/render/opengl/DataType.h"

namespace E4 {

    struct Transform {
        float x, y, z;

        uint64_t lastFrame;
        uint32_t parent;
        ShaderData worldTransform;

        Transform() :
            x(0), y(0), z(0),
            lastFrame(0), parent(0),
            worldTransform() {
        }
    };

}