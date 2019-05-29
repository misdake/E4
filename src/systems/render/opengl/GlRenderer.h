#pragma once

#include <iostream>
#include <glbinding/gl/gl.h>
#include "../../../components/Drawable.h"

using namespace gl;

namespace E4 {

    class GlRenderer {
    public:
        int width = 0;
        int height = 0;

        void resize(int w, int h);

        void init();

        void clear();

        void draw(const Mesh& mesh);

        void checkError();

    };

}
