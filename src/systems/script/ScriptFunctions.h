#pragma once

#define SOL_CHECK_ARGUMENTS 1

#include <sol/sol.hpp>

namespace E4 {

    class ScriptFunctions {
    public:
        uint32_t prefix;
        sol::function load;
        sol::function update;

        explicit ScriptFunctions(sol::state& state);
    };

}
