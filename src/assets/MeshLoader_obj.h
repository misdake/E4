#pragma once

#include "../core/Asset.h"
#include "../components/Drawable.h"

namespace E4 {

    class App;

    class MeshLoader_obj {
    public:
        uint64_t create(const std::string& folder, const std::string& filename, App& app);
    };

}
