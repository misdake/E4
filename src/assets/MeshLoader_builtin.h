#pragma once

#include "../core/Asset.h"
#include "../components/Drawable.h"

namespace E4 {

    class MeshLoader_builtin {
    public:
        Asset<Mesh> create(const std::string& meshName, AssetPool<Mesh>& pool);

        Asset<Mesh> createPlane(AssetPool<Mesh>& pool, bool invertY);
    };

}
