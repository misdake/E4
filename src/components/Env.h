#pragma once

#include "../systems/render/opengl/DataType.h"

namespace E4 {

    enum class CameraType {
        ORTHO,
        PROJ,
    };

    struct Camera {
        bool enabled;
        CameraType type;
        Vec3 eye;
        Vec3 target;
        Vec3 up;
        float fov;
        float znear;
        float zfar;

        Mat4 vp;

        void action(uint16_t width, uint16_t height) {
            vp.setTRS( //TODO
                target.x, target.y, 0,
                0, 0, 0,
                1.0f * height / width, 1, 1
            );
        }
    };

    enum class LightType {
        POINT,
        DIRECTIONAL,
    };
    struct Light {
        bool enabled;
        LightType type;
        ShaderData color;

        Vec3 direction;
        Vec3 world;
    };

    struct Env {
        Camera camera;
        Light light;
    };

}