#pragma once

#include <cstdint>
#include <string>
#include "FloatBuffer.h"

namespace E4 {

    enum class DataType {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        SHORT,
        TEXTURE,
        MATRIX,
    };

    class Attribute {
    public:
        uint32_t location;
        DataType dataType;
        std::string name;

        Attribute(DataType dataType, std::string name);

        void bind(FloatBuffer& buffer);
        void unbind();
    };

}