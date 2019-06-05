#pragma once

#include <vector>
#include <cstdint>

namespace E4 {

    class ShortBuffer {
    public:
        std::vector<uint16_t> array;
        uint32_t bufferId;
        uint32_t countIndices;

        ShortBuffer(std::vector<uint16_t> array, uint32_t countIndices);
        ShortBuffer();
        ShortBuffer& set(std::vector<uint16_t> array, uint32_t countIndices);
        void upload();
        void bind() const;
    };

}