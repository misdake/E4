#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class Transformer {
    public:
        void run(ECS& ecs, FrameState frameState);
    };

}
