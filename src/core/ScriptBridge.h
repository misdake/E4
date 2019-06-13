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
        static void bind(App& app, sol::state& lua, EcsCore& ecs);
    };

}
