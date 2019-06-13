#include "ScriptFunctions.h"

E4::ScriptFunctions::ScriptFunctions(sol::state& state) :
    prefix(0),
    load(sol::nil),
    update(sol::nil) {
    load = state["load"];
    update = state["update"];
}
