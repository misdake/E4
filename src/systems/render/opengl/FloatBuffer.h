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
        FloatBuffer();
        FloatBuffer& set(std::vector<float> narray, uint32_t nfloatPerVertex, uint32_t ncountVertex);
        void upload();
        void bind(uint32_t attributeIndex) const;
    };

}