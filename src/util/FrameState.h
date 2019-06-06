#pragma once

#include <cstdint>
#include "InputState.h"

namespace E4 {

    struct FrameState {
        FrameState(uint64_t frameIndex, uint16_t deltatime, InputState& inputStateCurr, InputState& inputStatePrev);
        uint64_t frameIndex;
        uint16_t deltatime;
        InputState& inputStateCurr;
        InputState& inputStatePrev;
    };

}
