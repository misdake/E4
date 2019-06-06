#include <SDL.h>
#include <cmath>

#include "core/App.h"
#include "components/Position.h"
#include "systems/render/opengl/Texture.h"

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

        E4::Asset<E4::Mesh> mesh1 = app.meshes.alloc();
        E4::Asset<E4::Mesh> mesh2 = app.meshes.alloc();
        mesh1->position.set(position, 3, 4).upload();
        mesh1->texcoord.set(texcoord, 2, 4).upload();
        mesh1->color.set(color, 3, 4).upload();
        mesh1->offset.set(0, 0, 0);
        mesh2->position.set(position, 3, 4).upload();
        mesh2->texcoord.set(texcoord, 2, 4).upload();
        mesh2->color.set(color, 3, 4).upload();
        mesh2->offset.set(0, 0, 0);

        E4::Asset<E4::Material> material1 = app.materials.alloc();
        material1->texture.name = "street.jpg";
        material1->texture.load();
        E4::Asset<E4::Material> material2 = app.materials.alloc();
        material2->texture.name = "favicon.png";
        material2->texture.load();

//        mesh1->vertexCount = 6;
        mesh1->index.set(index, 6).upload();
        mesh2->index.set(index, 6).upload();

        material1->shader = &app.renderer.shaderTexture;
        material2->shader = &app.renderer.shaderTexture;

        entt::entity entity1 = app.ecs.create();
        app.ecs.assign<E4::Position>(entity1, 0.0f, 0.0f, 0.0f, 0ul, 0ul, 0.0f, 0.0f, 0.0f);
        app.ecs.assign<E4::Drawable>(entity1, mesh1, material1);
        entt::entity entity2 = app.ecs.create();
        app.ecs.assign<E4::Position>(entity2, 0.0f, 0.0f, 0.0f, 0ul, entity1, 0.0f, 0.0f, 0.0f);
        app.ecs.assign<E4::Drawable>(entity2, mesh2, material2);
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

        app.ecs.view<E4::Position>().each([&](E4::Position& position) {
            if (position.parent == 0) {
                position.x += distance * dx;
                position.y += distance * dy;
            }
        });
        app.ecs.view<const E4::Position, E4::Drawable>().each([&](const E4::Position& position, E4::Drawable& drawable) {
            drawable.mesh->offset.numbers.x = position.worldX;
            drawable.mesh->offset.numbers.y = position.worldY;
        });
    });

    return 0;
}