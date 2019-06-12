#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace sol {
    class state;
}

namespace E4 {

    class ScriptRunner {
    private:
        sol::state* state;
    public:
        ScriptRunner();
        ~ScriptRunner();
        void run(EcsCore& ecs, const E4::FrameState& frameState);
    };

}
