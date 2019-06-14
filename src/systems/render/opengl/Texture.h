#pragma once

#include <string>
#include "DataType.h"

namespace E4 {

    class Texture {
    public:
        std::string folder;
        std::string name;
        ShaderData shaderData;
        int w, h;
        bool loading;
        bool loaded;

        Texture& load(std::string folder, std::string name);
        void unload();
    };

}