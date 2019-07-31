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

        bool mouse1down() const {
            return inputStateCurr.mouseButton1 && !inputStatePrev.mouseButton1;
        }
        bool mouse1up() const {
            return !inputStateCurr.mouseButton1 && inputStatePrev.mouseButton1;
        }
        bool mouse2down() const {
            return inputStateCurr.mouseButton2 && !inputStatePrev.mouseButton2;
        }
        bool mouse2up() const {
            return !inputStateCurr.mouseButton2 && inputStatePrev.mouseButton2;
        }
        bool mouse3down() const {
            return inputStateCurr.mouseButton3 && !inputStatePrev.mouseButton3;
        }
        bool mouse3up() const {
            return !inputStateCurr.mouseButton3 && inputStatePrev.mouseButton3;
        }
        bool keydown(uint16_t key) const {
            return inputStateCurr.keys[key] && !inputStatePrev.keys[key];
        }
        bool keyup(uint16_t key) const {
            return !inputStateCurr.keys[key] && inputStatePrev.keys[key];
        }
    };

}
