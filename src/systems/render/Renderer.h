#pragma once

#include "opengl/UniformSlot.h"
#include "opengl/AttributeSlot.h"

namespace E4 {

    class Renderer {
    public:
        Renderer() = default;

        UniformSlots uniformSlots;
        AttributeSlots attributeSlots;
    };

}