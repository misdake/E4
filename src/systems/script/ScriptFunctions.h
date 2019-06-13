#pragma once

#include "../../core/Lua.h"

namespace E4 {

    class ScriptFunctions {
    public:
        uint32_t index;
        sol::function load;
        sol::function update;

        explicit ScriptFunctions(sol::state& lua, uint32_t index);
    };

}
