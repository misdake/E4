#pragma once

#include <string>

namespace E4 {

    struct Texture {
        std::string name;
        uint32_t textureId;
        int w, h;
        bool loaded;

        void load();
        void unload();
    };

}