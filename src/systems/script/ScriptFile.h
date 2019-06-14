#pragma once

#include <string>

namespace E4 {

    class ScriptFunctions;

    class ScriptFile {
    public:
        std::string folder;
        std::string name;

        bool scriptLoaded;
        uint32_t scriptIndex;

        ScriptFile& load(std::string folder, std::string name);
        void unload();
    };

}
