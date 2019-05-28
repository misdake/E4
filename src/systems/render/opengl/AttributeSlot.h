#pragma once

#include <string>
#include "DataType.h"
#include "FloatBuffer.h"

namespace E4 {

    class AttributeSlot {
    public:
        const char* const name;
        const ShaderDataType dataType;
        AttributeSlot(const char* name, ShaderDataType dataType);
        AttributeSlot(const AttributeSlot&) = delete;

        uint32_t location;

        void bind(FloatBuffer& buffer) const;
        void unbind() const;
    };

    class AttributeSlots {
    public:
        AttributeSlot POSITION = AttributeSlot("aPosition", ShaderDataType::VEC4);
        AttributeSlot COLOR = AttributeSlot("aColor", ShaderDataType::VEC4);
    };

}
