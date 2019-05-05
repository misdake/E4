#pragma once

#include <string>
#include "../systems/render/Texture.h"

namespace E4 {

    struct Drawable {
        std::string name;
        Texture* texture;
    };

}