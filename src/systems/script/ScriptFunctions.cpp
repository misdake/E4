#include "ScriptFunctions.h"

E4::ScriptFunctions::ScriptFunctions(sol::state& state) :
    load(state["load"]),
    update(state["update"]) {
}
