#include <SDL.h>

#include "core/App.h"
#include "components/Transform.h"
#include "components/Script.h"

int main(int argc, char* argv[]) {
    E4::App app;

    app.load([](E4::App& app) {
        std::vector<float> position = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };
        std::vector<float> color = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
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

        E4::Asset<E4::Mesh> mesh = app.meshes.alloc();
        mesh->position.set(position, 3, 4).upload();
        mesh->texcoord.set(texcoord, 2, 4).upload();
        mesh->color.set(color, 3, 4).upload();
//        mesh->vertexCount = 6;
        mesh->index.set(index, 6).upload();

        E4::Asset<E4::Material> material1 = app.materials.alloc();
        material1->texture = app.textures.get("street.jpg");
        material1->shader = &app.renderer.shaderTexture;
        E4::Asset<E4::Material> material2 = app.materials.alloc();
        material2->texture = app.textures.get("favicon.png");
        material2->shader = &app.renderer.shaderTexture;

        entt::entity entity1 = app.ecs.createEntity();
        entt::entity entity2 = app.ecs.createEntity();

        auto& transform1 = app.ecs.createTransform(entity1);
        transform1.x = 0.1f;
        transform1.y = -0.5f;
        transform1.z = -0.1f;
        auto& transform2 = app.ecs.createTransform(entity2);
        transform2.x = 0.1f;
        transform2.y = 0.1f;
        transform2.z = -0.1f;
        transform2.parent = entity1;

        auto& drawable1 = app.ecs.createDrawable(entity1);
        drawable1.mesh = mesh;
        drawable1.material = material1;
        auto& drawable2 = app.ecs.createDrawable(entity2);
        drawable2.mesh = mesh;
        drawable2.material = material2;

        auto& script1 = app.ecs.createScript(entity1);
        script1.loaded = false;
        script1.file = app.scripts.get("script.lua");
        auto& script2 = app.ecs.createScript(entity2);
        script2.loaded = false;
        script2.file = app.scripts.get("script2.lua");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
//        static uint32_t totalTime = 0;
//        totalTime += frameState.deltatime;
//
//        float distance = frameState.deltatime * 0.001f;
//        float dx = 0;
//        float dy = 0;
//        dx += frameState.inputStateCurr.keys[SDL_SCANCODE_RIGHT] ? 1 : 0;
//        dx -= frameState.inputStateCurr.keys[SDL_SCANCODE_LEFT] ? 1 : 0;
//        dy += frameState.inputStateCurr.keys[SDL_SCANCODE_UP] ? 1 : 0;
//        dy -= frameState.inputStateCurr.keys[SDL_SCANCODE_DOWN] ? 1 : 0;
//
//        app.ecs.view<E4::Transform>().each([&](E4::Transform& transform) {
//            if (transform.parent > 0) {
//                transform.x += distance * dx;
//                transform.y += distance * dy;
//                transform.ry = std::fmod(totalTime * 0.001, 2 * M_PI);
//            }
//        });

//    app.textures.freeLoaded("favicon.png"); //how to delete texture
    });

    return 0;
}