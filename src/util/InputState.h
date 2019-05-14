#pragma once

#include <stdint.h>

namespace E4 {

    struct InputState {
    public:
        InputState() = default;

//        int wheel;
        int mouseX;
        int mouseY;
        bool mouseButton1;
        bool mouseButton2;
        bool mouseButton3;

        uint8_t keys[512];
    };

}
