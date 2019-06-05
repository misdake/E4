#pragma once

#include <iostream>
#include <glbinding/gl/gl.h>
using namespace gl;

#include "UniformSlot.h"
#include "AttributeSlot.h"

namespace E4 {

    class Drawable;

    class GlRenderer {
    public:
        UniformSlots uniformSlots;
        AttributeSlots attributeSlots;

        int width = 0;
        int height = 0;

        void resize(int w, int h);

        void init();

        void clear();

        void draw(const Drawable& drawable);

        void checkError();
    };

}
