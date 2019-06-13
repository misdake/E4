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

        Transform() :
            x(0), y(0), z(0), rx(0), ry(0), rz(0), sx(1), sy(1), sz(1),
            lastFrame(0), parent(0),
            worldTransform() {
        }
    };

}