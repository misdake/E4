#include "ScriptRunner.h"

#define SOL_CHECK_ARGUMENTS 1

#include <sol/sol.hpp>

#include "../../components/Script.h"
#include "ScriptFile.h"
#include "ScriptFunctions.h"

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
            if (!script.file->functions) { //if script functions are not loaded
                state->script(script.file->content);
                script.file->functions = new ScriptFunctions(*state);
            }

            //call load
            sol::function& load = script.file->functions->load;
            if (load != sol::nil) {
                load();
            }
            script.loaded = true;
        }

        //call update
        sol::function& update = script.file->functions->update;
        if (update != sol::nil) {
            update();
        }

    });
}
