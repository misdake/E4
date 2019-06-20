#include "MeshLoader_obj.h"

#include "../util/Log.h"
#include "ObjFileLoader.h"

E4::Asset<E4::Mesh> E4::MeshLoader_obj::create(const std::string& folder, const std::string& filename, E4::AssetPool<E4::Mesh>& pool) {
    std::vector<Asset<Mesh>> meshList = ObjFileLoader::objFileToMeshes(folder, filename, pool);
    Asset<Mesh>& mesh = meshList[0];
//    mesh->print();
    return mesh;
}
