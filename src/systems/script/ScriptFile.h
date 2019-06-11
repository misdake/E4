#pragma once

#include <string>

namespace E4 {

    class ScriptFunctions;

    class ScriptFile {
    public:
        std::string name;
        std::string content;

        ScriptFunctions* functions;

        ScriptFile& load(std::string name);
        void unload();
    };

}
