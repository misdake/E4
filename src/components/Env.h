#pragma once

#include "../systems/render/opengl/DataType.h"

namespace E4 {

    enum class LightType {
        POINT,
        DIRECTIONAL,
    };
    struct Light {
        bool enabled;
        LightType lightType;

        ShaderData direction;
        ShaderData color;
    };

    struct Env {
        Light light;
    };

}