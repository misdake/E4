#pragma once

#include "../../util/FrameState.h"

namespace E4 {

    class EcsCore;

    class Transformer {
    public:
        void run(EcsCore& ecs, FrameState frameState);
    };

}
