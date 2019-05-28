#pragma once

#include <cstdint>
#include <string>
#include "FloatBuffer.h"
#include "DataType.h"

namespace E4 {

    class Attribute {
    public:
        uint32_t location;
        ShaderDataType dataType;
        std::string name;

        Attribute(ShaderDataType dataType, std::string name);

        void bind(FloatBuffer& buffer);
        void unbind();
    };

}