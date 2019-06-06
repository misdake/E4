#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class Transform {
    public:
        void run(ECS& ecs, FrameState frameState);
    };

}
