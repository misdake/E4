#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"
#include "../env/Environment.h"

namespace E4 {

    class Transformer {
    public:
        void run(Ecs& ecs, const FrameState& frameState, const E4::Environment& environment);
    };

}
