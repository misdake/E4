#include "MeshLoader_obj.h"

#include <map>
#include <string>

#include "ObjFileLoader.h"
#include "../core/App.h"

const E4::ObjFile& E4::MeshLoader_obj::getObjFile(const std::string& folder, const std::string& name) {
    auto iter = objCache.find(name);
    if (iter == objCache.end()) {
        objCache[name] = ObjFile();
        ObjFile& objFile = objCache[name];
        ObjFileLoader::loadObjFile(folder, name, objFile);
        return objFile;
    } else {
        return iter->second;
    }
}
const E4::MtlFile& E4::MeshLoader_obj::getMtlFile(const std::string& folder, const std::string& name) {
    auto iter = mtlCache.find(name);
    if (iter == mtlCache.end()) {
        mtlCache[name] = MtlFile();
        MtlFile& mtlFile = mtlCache[name];
        ObjFileLoader::loadMtlFile(folder, name, mtlFile);
        return mtlFile;
    } else {
        return iter->second;
    }
}

uint64_t E4::MeshLoader_obj::create(const std::string& folder, const std::string& filename, App& app) {
    Entity& root = app.scene.newEntity();
    app.scene.createTransform(root);

    const ObjFile& objFile = getObjFile(folder, filename);
    const MtlFile& mtlFile = getMtlFile(folder, objFile.mtlFileName);

    std::map<std::string, const Mtl*> mtlMap;
    for (const Mtl& mtl : mtlFile.mtls) {
        mtlMap.insert(std::make_pair(mtl.name, &mtl));
    }

    for (const Obj& obj : objFile.objs) {
        const Mtl& mtl = *mtlMap[obj.mtlName];

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
