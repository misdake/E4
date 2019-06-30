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
        float fov;
        float znear;
        float zfar;

        void action(Mat4& world, uint16_t width, uint16_t height) {
            const Vec3& target = world.transformPoint(Vec3(0, 0, 0));
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
        ShaderData ambient;
        ShaderData diffuse;
        ShaderData specular;

        void transform(Mat4& tworld) {
            switch (type) {
                case LightType::POINT: {
                    Vec3 pos = tworld.transformPoint(Vec3(0, 0, 0));
                    world.set(pos.x, pos.y, pos.z, 1);
                    break;
                }
                case LightType::DIRECTIONAL: {
                    Vec3 dir = tworld.transformNormal(Vec3(0, 0, -1));
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