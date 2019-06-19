#pragma once

#include "../core/Asset.h"
#include "../systems/script/ScriptFile.h"

namespace E4 {

    struct Script {
        bool loaded;
        Asset<ScriptFile> file;
        //TODO script attributes
    };

}