#pragma once

#include <string>

namespace E4 {

    struct Texture {
        std::string name;
        unsigned int textureId;
        int w, h;
        bool loaded;

        void load();
        void unload();
    };

}