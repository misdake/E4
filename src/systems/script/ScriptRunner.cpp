#include "ScriptRunner.h"

#define SOL_PRINT_ERRORS 1
#define SOL_CHECK_ARGUMENTS 1

#include <sol/sol.hpp>

#include "../../components/Script.h"
#include "ScriptFile.h"
#include "ScriptFunctions.h"
#include "../../util/FrameState.h"
#include "../../components/Transform.h"

void registerTypes(sol::state& lua) {
    lua["ComponentTransform"] = 1;

    lua.new_usertype<E4::Transform>(
        "Transform",
        "x", &E4::Transform::x,
        "y", &E4::Transform::y,
        "z", &E4::Transform::z
    );
}

void requireComponent(int index, uint64_t entity, E4::ECS& ecs, sol::state& lua) {
    switch (index) {
        case 1: //transform
            lua["transform"] = std::ref(ecs.get<E4::Transform>(entity));
            break;
        default:
            break;
    }
}

void writeFrameState(sol::state& lua, const E4::FrameState& frameState) {
    lua["inputStatePrev"] = lua.create_table_with(
        "wheel", frameState.inputStatePrev.wheel,
        "mouseX", frameState.inputStatePrev.mouseX,
        "mouseY", frameState.inputStatePrev.mouseY,
        "mouseButton1", frameState.inputStatePrev.mouseButton1,
        "mouseButton2", frameState.inputStatePrev.mouseButton2,
        "mouseButton3", frameState.inputStatePrev.mouseButton3,
        "keys", &frameState.inputStatePrev.keys
    );
    lua["inputStateCurr"] = lua.create_table_with(
        "wheel", frameState.inputStateCurr.wheel,
        "mouseX", frameState.inputStateCurr.mouseX,
        "mouseY", frameState.inputStateCurr.mouseY,
        "mouseButton1", frameState.inputStateCurr.mouseButton1,
        "mouseButton2", frameState.inputStateCurr.mouseButton2,
        "mouseButton3", frameState.inputStateCurr.mouseButton3,
        "keys", &frameState.inputStateCurr.keys
    );
}

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(sol::lib::base, sol::lib::package);
    registerTypes(*state);
}

E4::ScriptRunner::~ScriptRunner() {
    delete state;
}

void E4::ScriptRunner::run(ECS& ecs, const E4::FrameState& frameState) {
    sol::state& lua = *state;

    writeFrameState(lua, frameState);

    uint64_t entityId = 0;

    lua.set_function("requireComponent", [&](int componentIndex) { requireComponent(componentIndex, entityId, ecs, lua); });

    auto view = ecs.view<E4::Script>();
    for (auto entity: view) {
        E4::Script& script = view.get(entity);
        entityId = entity;

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
    }
}
