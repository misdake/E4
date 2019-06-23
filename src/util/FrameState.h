#pragma once

#include <cstdint>
#include "InputState.h"

namespace E4 {

    struct FrameState {
        FrameState() = default;
        uint16_t width;
        uint16_t height;
        uint64_t frameIndex;
        uint16_t deltatime;
        InputState inputStateCurr;
        InputState inputStatePrev;
    };

}
