#pragma once

#include <cmath>

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

        void init() {
            enabled = true;
            type = CameraType::PROJ;
            fov = M_PI / 4;
            znear = 0.1;
            zfar = 100;
        }
        void action(Mat4& world, uint16_t width, uint16_t height) {
            const Vec3& p = world.transformPoint(Vec3(0, 0, 0));
            pos.set(p.x, p.y, p.z);
            float aspect = 1.0f * width / height;
            switch (type) {
                case CameraType::ORTHO: {
                    Mat4 ortho{};
                    Mat4 invWorld{};
                    world.invert(invWorld);
                    ortho.setOrtho(-aspect * fov, aspect * fov, fov, -fov, znear, zfar);
                    Mat4::multiply(ortho, invWorld, vp);
                    break;
                }
                case CameraType::PROJ: {
                    Mat4 perspective{};
                    Mat4 invWorld{};
                    world.invert(invWorld);
                    perspective.setPerspective(fov, aspect, znear, zfar);
                    Mat4::multiply(perspective, invWorld, vp);
                    break;
                }
            }
        }

        ShaderData pos;
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