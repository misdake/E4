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

        E4::Asset<E4::Material> material1 = app.materials.alloc();
        material1->texture = app.textures.get("street.jpg");
        E4::Asset<E4::Material> material2 = app.materials.alloc();
        material2->texture = app.textures.get("favicon.png");

//        mesh->vertexCount = 6;
        mesh->index.set(index, 6).upload();

        material1->shader = &app.renderer.shaderTexture;
        material2->shader = &app.renderer.shaderTexture;

        entt::entity entity1 = app.ecs.create();
        app.ecs.assign<E4::Transform>(entity1);
        auto& drawable1 = app.ecs.assign<E4::Drawable>(entity1);
        drawable1.mesh = mesh;
        drawable1.material = material1;

        entt::entity entity2 = app.ecs.create();
        auto& transform2 = app.ecs.assign<E4::Transform>(entity2);
        transform2.x = 0.1f;
        transform2.y = 0.1f;
        transform2.z = -0.1f;
        transform2.parent = entity1;
        auto& drawable2 = app.ecs.assign<E4::Drawable>(entity2);
        drawable2.mesh = mesh;
        drawable2.material = material2;
        auto& script2 = app.ecs.assign<E4::Script>(entity2);
        script2.loaded = false;
        script2.file = app.scripts.get("script.lua");
    });

    app.enterLoop([](E4::App& app, const E4::FrameState& frameState) {
        static uint32_t totalTime = 0;
        totalTime += frameState.deltatime;

        float distance = frameState.deltatime * 0.001f;
        float dx = 0;
        float dy = 0;
        dx += frameState.inputStateCurr.keys[SDL_SCANCODE_D] ? 1 : 0;
        dx -= frameState.inputStateCurr.keys[SDL_SCANCODE_A] ? 1 : 0;
        dy += frameState.inputStateCurr.keys[SDL_SCANCODE_W] ? 1 : 0;
        dy -= frameState.inputStateCurr.keys[SDL_SCANCODE_S] ? 1 : 0;
//        std::cout << frameState.frameIndex << std::endl;

        app.ecs.view<E4::Transform>().each([&](E4::Transform& position) {
            if (position.parent == 0) {
                position.x += distance * dx;
                position.y += distance * dy;
            }
        });

//    app.textures.freeLoaded("favicon.png"); //how to delete texture
    });

    return 0;
}