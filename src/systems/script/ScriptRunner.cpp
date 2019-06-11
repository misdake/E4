#include "ScriptRunner.h"

#define SOL_CHECK_ARGUMENTS 1

#include <sol/sol.hpp>

#include "../../components/Script.h"
#include "ScriptFile.h"
#include "ScriptFunctions.h"
#include "../../util/FrameState.h"

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(sol::lib::base, sol::lib::package);

    state->new_usertype<E4::InputState>(
        "InputState",
        "wheel", &E4::InputState::wheel,
        "mouseX", &E4::InputState::mouseX,
        "mouseY", &E4::InputState::mouseY,
        "mouseButton1", &E4::InputState::mouseButton1,
        "mouseButton2", &E4::InputState::mouseButton2,
        "mouseButton3", &E4::InputState::mouseButton3,
//        "keys", sol::property([](E4::InputState& self) { return self.keys; })
    );

    state->new_usertype<E4::FrameState>(
        "FrameState",
        "frameIndex", &E4::FrameState::frameIndex,
        "deltatime", &E4::FrameState::deltatime,
        "inputStateCurr", &E4::FrameState::inputStateCurr,
        "inputStatePrev", &E4::FrameState::inputStatePrev
    );
}
E4::ScriptRunner::~ScriptRunner() {
    delete state;
}

void E4::ScriptRunner::run(ECS& ecs, const E4::FrameState& frameState) {
    (*state)["frameState"] = frameState;

    ecs.view<E4::Script>().each([&](E4::Script& script) {
        if (!script.loaded) {
            //if script functions are not loaded => load script and extract functions
            if (!script.file->functions) {
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
