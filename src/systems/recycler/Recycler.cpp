#include "Recycler.h"

#include "../../core/App.h"

#include <iostream>

void E4::Material::unload() {

}

void E4::Mesh::unload() {
    position.unload();
    texcoord.unload();
    normal.unload();
    index.unload();
}

void E4::Recycler::run(E4::App& app, E4::Ecs& ecs, const E4::FrameState& state) {
    AssetPool<Mesh>& meshes = app.meshes;
    AssetPool<Material>& materials = app.materials;
    AssetLoader<Texture>& textures = app.textures;

    meshes.checkBegin();
    materials.checkBegin();
    textures.checkBegin();

    ecs.fortype<E4::Drawable>([&](E4::Drawable& drawable) {
        meshes.check(drawable.mesh);
        materials.check(drawable.material);
        if (drawable.material.valid()) {
            Asset<Texture>& texture = drawable.material->texture;
            if (texture.valid()) {
                textures.check(texture);
            }
        }
    });

    meshes.checkEnd([](Asset<E4::Mesh>& mesh) {
        mesh->unload();
        mesh.free();
    });
    materials.checkEnd([](Asset<E4::Material>& material) {
        material->unload();
        material.free();
    });
    textures.checkEnd([&](Asset<E4::Texture>& texture) {
        textures.freeLoaded(texture->name);
    });
}
