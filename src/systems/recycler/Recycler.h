#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace E4 {

    class App;

    class Recycler {
    public:
        void run(App& app, Ecs& ecs, const E4::FrameState& state);
    };

}