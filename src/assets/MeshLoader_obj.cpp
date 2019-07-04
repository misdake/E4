#include "MeshLoader_obj.h"

#include <map>
#include <string>

#include "../util/Log.h"
#include "ObjFileLoader.h"
#include "../core/App.h"

uint64_t E4::MeshLoader_obj::create(const std::string& folder, const std::string& filename, App& app) {
    Entity& root = app.scene.newEntity();
    app.scene.createTransform(root);

    ObjFile objFile;
    MtlFile mtlFile;
    ObjFileLoader::loadObjFile(folder, filename, objFile);
    ObjFileLoader::loadMtlFile(folder, objFile.mtlFileName, mtlFile);

    std::map<std::string, Mtl*> mtlMap;
    for (Mtl& mtl : mtlFile.mtls) {
        mtlMap.insert(std::make_pair(mtl.name, &mtl));
    }

    for (Obj& obj : objFile.objs) {
        Mtl& mtl = *mtlMap[obj.mtlName];

        Asset<Mesh> mesh = app.meshes.alloc();
        uint64_t vertexCount = obj.position.size() / 3;
        mesh->position.set(obj.position, 3, vertexCount).upload();
        if (!obj.texcoord.empty()) mesh->texcoord.set(obj.texcoord, 2, obj.texcoord.size() / 2).upload();
        if (!obj.normal.empty()) mesh->normal.set(obj.normal, 3, obj.normal.size() / 3).upload();
        mesh->vertexCount = vertexCount;

        Entity& child = app.scene.newEntity();
        Transform& transform = app.scene.createTransform(child);
        transform.parent = root.index;
        Drawable& drawable = app.scene.createDrawable(child);
        drawable.mesh = mesh;
        drawable.material = app.scene.newMaterialLight("FF000000", "FFFFFFFF");
        drawable.material->ambient.set(mtl.Ka[0], mtl.Ka[1], mtl.Ka[2], 1);
        drawable.material->diffuse.set(mtl.Kd[0], mtl.Kd[1], mtl.Kd[2], 1);
        drawable.material->specular.set(mtl.Ks[0], mtl.Ks[1], mtl.Ks[2], 1);
        drawable.material->specularExp.set(mtl.Ns);
    }

    return root.id;
}
