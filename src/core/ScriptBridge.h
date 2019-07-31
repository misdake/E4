#pragma once

#include "../components/Components.h"
#include "ECS.h"
#include "../util/FrameState.h"

namespace sol {
    class state;
}

namespace E4 {

    class App;

    class ScriptBridge {
    public:
        static void load(App& app, sol::state& lua, Ecs& ecs);
        static void update(App& app, sol::state& lua, Ecs& ecs, const E4::FrameState& frameState);
    };

}
