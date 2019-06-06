#include "FrameState.h"

E4::FrameState::FrameState(uint64_t frameIndex, uint16_t deltatime, E4::InputState& inputStateCurr, E4::InputState& inputStatePrev) :
    frameIndex(frameIndex),
    deltatime(deltatime),
    inputStateCurr(inputStateCurr),
    inputStatePrev(inputStatePrev) {
}