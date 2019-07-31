#pragma once

#include "../../core/ECS.h"
#include "../../util/FrameState.h"

namespace sol {
    class state;
}

namespace E4 {

    class ScriptRunner {
    private:
        friend class App;
        sol::state* state;
        uint32_t scriptIndex;
    public:
        ScriptRunner();
        ~ScriptRunner();
        void run(const char* script);
        void run(Ecs& ecs, const E4::FrameState& frameState);
    };

}
