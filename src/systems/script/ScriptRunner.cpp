#include "ScriptRunner.h"

#include "ScriptFile.h"
#include "../../core/Lua.h"
#include "../../util/FrameState.h"
#include "../../components/Transform.h"
#include "../../components/Script.h"
#include "../../util/File.h"

void registerTypes(sol::state& lua) {
    lua.script(R"(
        function prepareEntity(id)
            entity = entities[id]
        end
        function runLoad(index)
            if scripts[index].load ~= nil then
                scripts[index].load()
            end
        end
        function runUpdate(index)
            if scripts[index].update ~= nil then
                scripts[index].update()
            end
        end
    )");

    lua["entities"] = lua.create_table();
    lua["scripts"] = lua.create_table();

    lua.new_usertype<E4::Transform>(
        "Transform",
        "x", &E4::Transform::x,
        "y", &E4::Transform::y,
        "z", &E4::Transform::z,
        "rx", &E4::Transform::rx,
        "ry", &E4::Transform::ry,
        "rz", &E4::Transform::rz,
        "sx", &E4::Transform::sx,
        "sy", &E4::Transform::sy,
        "sz", &E4::Transform::sz,
        "parent", &E4::Transform::parent
    );
    lua.new_usertype<E4::Drawable>(
        "Drawable",
        "mesh", &E4::Drawable::mesh,
        "material", &E4::Drawable::material
    );
    lua.new_usertype<E4::Script>(
        "Script"
    );
}

void firstFrame(sol::state& lua, const E4::FrameState& frameState, E4::EcsCore& ecs) {
    lua["inputStatePrev"] = lua.create_table_with(
        "wheel", 0,
        "mouseX", 0,
        "mouseY", 0,
        "mouseButton1", false,
        "mouseButton2", false,
        "mouseButton3", false,
        "keys", &frameState.inputStatePrev.keys
    );
    lua["inputStateCurr"] = lua.create_table_with(
        "wheel", 0,
        "mouseX", 0,
        "mouseY", 0,
        "mouseButton1", false,
        "mouseButton2", false,
        "mouseButton3", false,
        "keys", &frameState.inputStateCurr.keys
    );
}
void updateFrameState(sol::state& lua, const E4::FrameState& frameState) {
    lua["dt"] = frameState.deltatime * 0.001f;
    lua["frameIndex"] = frameState.frameIndex;
    lua["inputStatePrev"]["wheelX"] = frameState.inputStatePrev.wheelX;
    lua["inputStatePrev"]["wheelY"] = frameState.inputStatePrev.wheelY;
    lua["inputStatePrev"]["mouseX"] = frameState.inputStatePrev.mouseX;
    lua["inputStatePrev"]["mouseY"] = frameState.inputStatePrev.mouseY;
    lua["inputStatePrev"]["mouseButton1"] = frameState.inputStatePrev.mouseButton1;
    lua["inputStatePrev"]["mouseButton2"] = frameState.inputStatePrev.mouseButton2;
    lua["inputStatePrev"]["mouseButton3"] = frameState.inputStatePrev.mouseButton3;
    lua["inputStatePrev"]["keys"] = std::ref(frameState.inputStatePrev.keys);
    lua["inputStateCurr"]["wheelX"] = frameState.inputStateCurr.wheelX;
    lua["inputStateCurr"]["wheelY"] = frameState.inputStateCurr.wheelY;
    lua["inputStateCurr"]["mouseX"] = frameState.inputStateCurr.mouseX;
    lua["inputStateCurr"]["mouseY"] = frameState.inputStateCurr.mouseY;
    lua["inputStateCurr"]["mouseButton1"] = frameState.inputStateCurr.mouseButton1;
    lua["inputStateCurr"]["mouseButton2"] = frameState.inputStateCurr.mouseButton2;
    lua["inputStateCurr"]["mouseButton3"] = frameState.inputStateCurr.mouseButton3;
    lua["inputStateCurr"]["keys"] = std::ref(frameState.inputStateCurr.keys);
}

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(sol::lib::base, sol::lib::math);
    scriptIndex = 0;
    registerTypes(*state);
}

E4::ScriptRunner::~ScriptRunner() {
    delete state;
}

void E4::ScriptRunner::run(EcsCore& ecs, const E4::FrameState& frameState) {
    sol::state& lua = *state;

    static bool loaded = false;
    if (!loaded) {
        firstFrame(lua, frameState, ecs);
        loaded = true;
    }

    updateFrameState(lua, frameState);

    auto view = ecs.view<E4::Script>();
    for (auto entity: view) {
        lua["prepareEntity"](entity);

        E4::Script& script = view.get(entity);
        //if script functions are not loaded => load script and extract functions
        if (!script.file->scriptLoaded) {
            script.file->scriptLoaded = true;
            std::string content = readFile(script.file->folder + "/" + script.file->name);
            state->script(content);
            uint32_t index = script.file->scriptIndex = (script.file->scriptIndex > 0) ? script.file->scriptIndex : ++scriptIndex;
            lua["scripts"][index] = lua.create_table();
            lua["scripts"][index]["load"] = lua["load"];
            lua["scripts"][index]["update"] = lua["update"];
        }

        if (!script.loaded) {
            //call load
            script.loaded = true;
            lua["runLoad"](script.file->scriptIndex);

            //call update
            E4::Script& script2 = view.get(entity); //load again. load() might call createScript, invalidating script.
            lua["runUpdate"](script2.file->scriptIndex);
        } else {
            lua["runUpdate"](script.file->scriptIndex);
        }

    }
}
