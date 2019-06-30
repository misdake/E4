#pragma once

#include "DataType.h"

namespace E4 {

    class UniformSlot {
    public:
        const char* const name;
        ShaderDataType dataType;
        UniformSlot(const char* name, ShaderDataType dataType);
        UniformSlot(const UniformSlot&) = delete;

        uint32_t location;

        void bind(const ShaderData& shaderData);
    };

    class UniformSlots {
    public:
        UniformSlot world = UniformSlot("uWorld", ShaderDataType::MAT4);
        UniformSlot wvp = UniformSlot("uWVP", ShaderDataType::MAT4);
        UniformSlot texture = UniformSlot("uTexture", ShaderDataType::TEXTURE);
        UniformSlot ambient = UniformSlot("uAmbient", ShaderDataType::VEC4);
        UniformSlot diffuse = UniformSlot("uDiffuse", ShaderDataType::VEC4);
        UniformSlot light = UniformSlot("uLight", ShaderDataType::VEC4);
        UniformSlot lightAmbient = UniformSlot("uLightAmbient", ShaderDataType::VEC4);
        UniformSlot lightDiffuse = UniformSlot("uLightDiffuse", ShaderDataType::VEC4);
    };

}