#include "ScriptFunctions.h"

E4::ScriptFunctions::ScriptFunctions(const sol::state& state) :
    prefix(0),
    load(std::move(state["load"])),
    update(std::move(state["update"])) {
}
