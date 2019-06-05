#include <SDL.h>
#include <cmath>

#include "core/App.h"
#include "systems/render/Texture.h"

int main(int argc, char* argv[]) {
    E4::App app;

    E4::Asset<E4::Mesh> mesh;

    app.load([&](E4::App& app) {
        std::vector<float> position = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };
        std::vector<float> color = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
        };
        std::vector<float> texcoord = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
        };

        mesh = app.meshes.alloc();
        E4::Asset<E4::Material> material = app.materials.alloc();
        E4::Asset<E4::Drawable> drawable = app.drawables.alloc();
        mesh->position.set(position, 3, 6).upload();
        mesh->texcoord.set(texcoord, 2, 6).upload();
        mesh->color.set(color, 3, 6).upload();
        mesh->offset.set(0, 0, 0);

        material->texture.name = "street.jpg";
        material->texture.load();

        mesh->vertexCount = 6;

        material->shader = &app.renderer.shaderTexture;

        drawable->mesh = mesh;
        drawable->material = material;

        app.scene.push_back(drawable);
    });

    app.enterLoop([&mesh](E4::App& app, const E4::FrameState& frameState) {
        static uint32_t totalTime = 0;
        totalTime += frameState.deltatime;
        mesh->offset.numbers.x = std::sin(totalTime / 1000.0f);
    });

    return 0;
}