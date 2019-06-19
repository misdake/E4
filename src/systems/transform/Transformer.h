#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class Transformer {
    public:
        void run(Ecs& ecs, FrameState frameState);
    };

}
