#include "MeshLoader_obj.h"

#include "../util/Log.h"
#include "ObjFileLoader.h"
#include "../core/App.h"

uint64_t E4::MeshLoader_obj::create(const std::string& folder, const std::string& filename, App& app) {
    std::vector<Asset<Mesh>> meshList = ObjFileLoader::objFileToMeshes(folder, filename, app.meshes);
    Entity& root = app.scene.newEntity();
    app.scene.createTransform(root);

    for (Asset<Mesh> mesh : meshList) {
        Entity& child = app.scene.newEntity();
        Transform& transform = app.scene.createTransform(root);
        transform.parent = root.index;
        Drawable& drawable = app.scene.createDrawable(root);
        drawable.mesh = mesh;
        drawable.material = app.scene.newMaterialColor("FFFFFFFF");
    }

    return root.id;
}
