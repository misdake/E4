#include "ScriptRunner.h"

#include "ScriptFile.h"
#include "../../core/Lua.h"
#include "../../util/FrameState.h"
#include "../../components/Script.h"
#include "../../util/File.h"

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::bit32,
        sol::lib::table,
//        sol::lib::debug,
        sol::lib::string
    );
    scriptIndex = 0;
}

E4::ScriptRunner::~ScriptRunner() {
    delete state;
}

void E4::ScriptRunner::run(const char* script) {
    state->script(script);
}

void E4::ScriptRunner::run(Ecs& ecs, const E4::FrameState& frameState) {
    sol::state& lua = *state;

    ecs.fortypes<Script>([&](Entity& entity, Script& script) {
        lua["prepareEntity"](entity.index);

        //if script functions are not loaded => load script and extract functions
        if (!script.file->scriptLoaded) {
            script.file->scriptLoaded = true;
            std::string content = readFile(script.file->folder, script.file->name);
            Log::debug("script load %s", script.file->name.c_str());
            state->script(content, script.file->name);
            uint32_t index = script.file->scriptIndex = (script.file->scriptIndex > 0) ? script.file->scriptIndex : ++scriptIndex;
            lua["scripts"][index] = lua.create_table();
            lua["scripts"][index]["load"] = lua["load"];
            lua["scripts"][index]["update"] = lua["update"];
        }

        if (!script.loaded) {
            //call load
            script.loaded = true;
            lua["runLoad"](script.file->scriptIndex);
        }

        lua["runUpdate"](script.file->scriptIndex);
    });

//    size_t m1 = lua.memory_used();
//    lua.collect_garbage();
//    size_t m2 = lua.memory_used();
//    Log::debug("memory: %d bytes => %d bytes", m1, m2);
}
