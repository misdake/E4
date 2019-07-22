#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class Recycler {
    public:
        void run(Ecs& ecs, const E4::FrameState& state);
    };

}