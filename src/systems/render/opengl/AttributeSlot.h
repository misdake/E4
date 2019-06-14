#pragma once

#include "DataType.h"

namespace E4 {

    class FloatBuffer;

    class AttributeSlot {
    public:
        const char* const name;
        const ShaderDataType dataType;
        AttributeSlot(const char* name, ShaderDataType dataType);
        AttributeSlot(const AttributeSlot&) = delete;

        uint32_t location;

        void bind(const FloatBuffer& buffer) const;
        void unbind() const;
    };

    class AttributeSlots {
    public:
        AttributeSlot position = AttributeSlot("aPosition", ShaderDataType::VEC3);
        AttributeSlot texcoord = AttributeSlot("aTexcoord", ShaderDataType::VEC2);
        AttributeSlot color = AttributeSlot("aColor", ShaderDataType::VEC4);
    };

}
