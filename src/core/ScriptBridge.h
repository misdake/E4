#pragma once

#include "../components/Components.h"
#include "ECS.h"

namespace sol {
    class state;
}

namespace E4 {

    class App;

    class ScriptBridge {
    public:
        static void load(App& app, sol::state& lua, EcsCore& ecs);
        static void update(App& app, sol::state& lua, EcsCore& ecs);
    };

}
