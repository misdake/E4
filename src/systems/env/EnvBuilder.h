#pragma once

#include "Environment.h"
#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class EnvBuilder {
    public:
        Environment environment;

        void run(Ecs& ecs, const E4::FrameState& state);
    };

}