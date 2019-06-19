#include "ScriptRunner.h"

#include "ScriptFile.h"
#include "../../core/Lua.h"
#include "../../util/FrameState.h"
#include "../../components/Transform.h"
#include "../../components/Script.h"
#include "../../util/File.h"

E4::ScriptRunner::ScriptRunner() {
    state = new sol::state;
    state->open_libraries(sol::lib::base, sol::lib::math);
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

    ecs.fortype<Script>([&](Entity& entity, Script& script) {
        lua["prepareEntity"](entity.id);

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
        }

        lua["runUpdate"](script.file->scriptIndex);
    });
}
