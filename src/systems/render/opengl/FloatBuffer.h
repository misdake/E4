#pragma once

#include <vector>
#include <cstdint>

namespace E4 {

    class FloatBuffer {
    public:
        std::vector<float> array;
        uint32_t bufferId;
        uint32_t floatPerVertex;
        uint32_t countVertex;

        FloatBuffer(std::vector<float> array, uint32_t floatPerVertex, uint32_t countVertex);
        void upload();
        void bind(uint32_t attributeIndex) const;
    };

}