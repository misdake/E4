#pragma once

#include "UniformSlot.h"
#include "AttributeSlot.h"

namespace E4 {

    class Transform;
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

        void draw(const Transform& transform, const Drawable& drawable);

        void checkError();
    };

}
