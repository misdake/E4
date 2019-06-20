#pragma once

#include "../core/Asset.h"
#include "../components/Drawable.h"

namespace E4 {

    class MeshLoader_obj {
    public:
        Asset<Mesh> create(const std::string& folder, const std::string& filename, AssetPool<Mesh>& pool);
    };

}
