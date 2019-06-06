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

        E4::Asset<E4::Mesh> mesh = app.meshes.alloc();
        E4::Asset<E4::Material> material = app.materials.alloc();
        mesh->position.set(position, 3, 4).upload();
        mesh->texcoord.set(texcoord, 2, 4).upload();
        mesh->color.set(color, 3, 4).upload();
        mesh->offset.set(0, 0, 0);

        material->texture.name = "street.jpg";
        material->texture.load();

//        mesh->vertexCount = 6;
        mesh->index.set(index, 6).upload();

        material->shader = &app.renderer.shaderTexture;

        entt::entity entity = app.ecs.create();
        app.ecs.assign<E4::Position>(entity, 0.0f, 0.0f);
        app.ecs.assign<E4::Drawable>(entity, mesh, material);
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
            position.x += distance * dx;
            position.y += distance * dy;
        });
        app.ecs.view<const E4::Position, E4::Drawable>().each([&](const E4::Position& position, E4::Drawable& drawable) {
            drawable.mesh->offset.numbers.x = position.x;
            drawable.mesh->offset.numbers.y = position.y;
        });
    });

    return 0;
}