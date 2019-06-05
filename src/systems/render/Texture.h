#pragma once

#include <string>
#include "opengl/DataType.h"

namespace E4 {

    class Texture {
    public:
        std::string name;
        ShaderData shaderData;
        int w, h;
        bool loaded;

        void load();
        void unload();
    };

}