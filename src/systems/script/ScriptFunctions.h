#pragma once

#include "../../core/Lua.h"

namespace E4 {

    class ScriptFunctions {
    public:
        uint32_t prefix;
        sol::function load;
        sol::function update;

        explicit ScriptFunctions(sol::state& state);
    };

}
