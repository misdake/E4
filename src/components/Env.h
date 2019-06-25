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

        void action(uint16_t width, uint16_t height) {
            vp.setTRS( //TODO
                target.x, target.y, 0,
                0, 0, 0,
                1.0f * height / width, 1, 1
            );
        }

        Mat4 vp;
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

        void transform(Mat4& tworld) {
            switch (type) {
                case LightType::POINT: {
                    Vec3 pos = tworld.transformPoint(Vec3(0, 0, 0));
                    world.set(pos.x, pos.y, pos.z, 1);
                    break;
                }
                case LightType::DIRECTIONAL: {
                    Vec3 dir = tworld.transformNormal(direction);
                    world.set(dir.x, dir.y, dir.z, 0);
                    break;
                }
            }
        }

        ShaderData world;
    };

    struct Env {
        Camera camera;
        Light light;
    };

}