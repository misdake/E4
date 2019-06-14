#pragma once

#include "../Asset.h"
#include "../../components/Drawable.h"

namespace E4 {

    class BuiltinMeshLoader {
    public:
        Asset<Mesh> create(const std::string& meshName, AssetPool<Mesh>& pool);

        Asset<Mesh> createPlane(AssetPool<Mesh>& pool);
    };

}
