#pragma once

#include <cstdint>
#include <string>
#include "FloatBuffer.h"

namespace E4 {

    enum class DataType {
        FLOAT,
        SHORT,
        TEXTURE,
        MATRIX,
    };

    class Attribute {
    public:
        uint32_t attributeIndex;
        DataType dataType;
        std::string name;

        Attribute(DataType dataType, std::string name);

        void bind(FloatBuffer& buffer);
        void unbind();
    };

}