#include "MeshLoader_builtin.h"

#include "../util/Log.h"

E4::Asset<E4::Mesh> E4::MeshLoader_builtin::create(const std::string& meshName, E4::AssetPool<E4::Mesh>& pool) {
    if (meshName.rfind("plane_inverty", 0) == 0) {
        return createPlane(pool, true);
    } else if (meshName.rfind("plane", 0) == 0) {
        return createPlane(pool, false);
    } else {
        Log::error("create mesh with unknown name %s", meshName.c_str());
        return Asset<Mesh>();
    }
}

E4::Asset<E4::Mesh> E4::MeshLoader_builtin::createPlane(AssetPool<Mesh>& pool, bool invertY) {
    float sy = invertY ? -1 : 1;
    std::vector<float> position = {
        0.5f, sy * 0.5f, 0.0f,
        0.5f, sy * -0.5f, 0.0f,
        -0.5f, sy * -0.5f, 0.0f,
        -0.5f, sy * 0.5f, 0.0f,
    };
    std::vector<float> texcoord = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    };
    std::vector<uint16_t> index = {
        0, 1, 2,
        0, 2, 3,
    };

    Asset<Mesh> mesh = pool.alloc();
    mesh->position.set(position, 3, 4).upload();
    mesh->texcoord.set(texcoord, 2, 4).upload();
    mesh->index.set(index, 6).upload();
    return mesh;
}
