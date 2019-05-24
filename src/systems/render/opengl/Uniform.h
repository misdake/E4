#pragma once

#include <cstdint>
#include <string>
#include "FloatBuffer.h"
#include "DataType.h"

namespace E4 {

    class Uniform {
    public:
        uint32_t location;
        DataType dataType;
        std::string name;

        Uniform(DataType dataType, std::string name);

        void bind(float x, float y, float z);
    };

}