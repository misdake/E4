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
        UniformSlot OFFSET = UniformSlot("uOffset", ShaderDataType::VEC3);
    };

}