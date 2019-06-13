#pragma once

#include <cstdint>
#include "../../../math/Mat4.h"
#include "../../../core/Asset.h"

namespace E4 {
    enum class ShaderDataType {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        TEXTURE,
        MAT4,
    };

    const char* const dataTypeName(ShaderDataType type);

    union ShaderData {

        struct Numbers { //TODO replace with Vec4
            float x;
            float y;
            float z;
            float w;
        } numbers;

        struct Colors { //TODO replace with Color
            float r;
            float g;
            float b;
            float a;
        } colors;

        uint32_t textureId;

        Asset<Mat4> mat4;

        ShaderData() :
            numbers{0, 0, 0, 0} {
        }

        explicit ShaderData(uint32_t textureId) :
            textureId{textureId} {
        }

        explicit ShaderData(float x) :
            numbers{x, 0, 0, 0} {
        }

        explicit ShaderData(const Asset<Mat4>& mat4) :
            mat4{mat4} {
        }

        ShaderData(float x, float y) :
            numbers{x, y, 0, 0} {
        }

        ShaderData(float x, float y, float z) :
            numbers{x, y, z, 0} {
        }

        ShaderData(float x, float y, float z, float w) :
            numbers{x, y, z, w} {
        }

        void set(float x) {
            numbers.x = x;
        }
        void set(float x, float y) {
            numbers.x = x;
            numbers.y = y;
        }
        void set(float x, float y, float z) {
            numbers.x = x;
            numbers.y = y;
            numbers.z = z;
        }
        void set(float x, float y, float z, float w) {
            numbers.x = x;
            numbers.y = y;
            numbers.z = z;
            numbers.w = w;
        }

    };
}