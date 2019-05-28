#pragma once

#include <cstdint>
#include <string>
#include "FloatBuffer.h"
#include "DataType.h"

namespace E4 {

    class Uniform {
    public:
        uint32_t location;
        ShaderDataType dataType;
        std::string name;

        Uniform(ShaderDataType dataType, std::string name);

        void bind(ShaderData shaderData);
    };

}