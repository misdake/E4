#include <SDL.h>

#include "core/App.h"
#include "components/Transform.h"
#include "components/Script.h"

int main(int argc, char* argv[]) {
    E4::App app;

    app.load([](E4::App& app) {
        E4::Entity e = app.ecs.createEntity();
        app.ecs.createTransform(e);
        E4::Script& script = app.ecs.createScript(e);
        script.loaded = false;
        script.file = app.scripts.get("main.lua");
        
//        E4::Asset<E4::Mesh> mesh = app.meshLoader.createPlane(app.meshes);
//
//        E4::Asset<E4::Material> material1 = app.materials.alloc();
//        material1->color.color.set("FFFF8800");
//        material1->shader = &app.renderer.shaderBasic;
//        E4::Asset<E4::Material> material2 = app.materials.alloc();
//        material2->texture = app.textures.get("favicon.png");
//        material2->shader = &app.renderer.shaderTexture;
//
//        entt::entity entity1 = app.ecs.createEntity();
//        entt::entity entity2 = app.ecs.createEntity();
//
//        auto& transform1 = app.ecs.createTransform(entity1);
//        transform1.x = 0.1f;
//        transform1.y = -0.5f;
//        transform1.z = -0.1f;
//        auto& transform2 = app.ecs.createTransform(entity2);
//        transform2.x = 0.1f;
//        transform2.y = 0.1f;
//        transform2.z = -0.1f;
//        transform2.parent = entity1;
//
//        auto& drawable1 = app.ecs.createDrawable(entity1);
//        drawable1.mesh = mesh;
//        drawable1.material = material1;
//        auto& drawable2 = app.ecs.createDrawable(entity2);
//        drawable2.mesh = mesh;
//        drawable2.material = material2;
//
//        auto& script1 = app.ecs.createScript(entity1);
//        script1.loaded = false;
//        script1.file = app.scripts.get("script.lua");
//        auto& script2 = app.ecs.createScript(entity2);
//        script2.loaded = false;
//        script2.file = app.scripts.get("script2.lua");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
    });

    return 0;
}