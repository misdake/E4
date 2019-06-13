#include "ScriptFunctions.h"

E4::ScriptFunctions::ScriptFunctions(sol::state& lua, uint32_t index) :
    index(index),
    load(sol::nil),
    update(sol::nil) {
    load = lua["load"];
    update = lua["update"];
    lua["scripts"][index] = lua.create_table();
    lua["scripts"][index]["load"] = lua["load"];
    lua["scripts"][index]["update"] = lua["update"];
}
