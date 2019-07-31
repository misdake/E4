#pragma once

#include "UniformSlot.h"
#include "AttributeSlot.h"
#include "../../env/Environment.h"

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

        void draw(const Transform& transform, const Drawable& drawable, const E4::Environment& environment);

        void checkError();
    };

}
