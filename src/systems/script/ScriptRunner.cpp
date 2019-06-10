#include "ScriptRunner.h"

#define SOL_ALL_SAFETIES_ON 1

#include <sol/sol.hpp>

#include "../../components/Script.h"

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(sol::lib::base, sol::lib::package);
}
E4::ScriptRunner::~ScriptRunner() {
    delete state;
}

void E4::ScriptRunner::run(ECS& ecs) {
    ecs.view<E4::Script>().each([&](E4::Script& script) {
        if (!script.loaded) {
            if (!script.load.empty()) {
                state->script(script.load);
            }
            script.loaded = true;
        }
        if (!script.update.empty()) {
            state->script(script.update);
        }
    });
}
