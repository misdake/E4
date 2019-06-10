#pragma once

#include "../../core/ECS.h"

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
        void run(ECS& ecs);
    };

}
