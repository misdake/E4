#include <SDL.h>
#include <cmath>

#include "core/App.h"

int main(int argc, char* argv[]) {
    E4::App app;

    E4::Asset<E4::Mesh> mesh;

    app.load([&mesh](E4::App& app) {

        std::vector<float> position = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };
        std::vector<float> color = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        mesh = app.meshes.alloc();
        E4::Asset<E4::Material> material = app.materials.alloc();
        E4::Asset<E4::Drawable> drawable = app.drawables.alloc();
        mesh->position.set(position, 3, 3);
        mesh->color.set(color, 3, 3);
        mesh->offset.set(0.2, 0, 0);
        mesh->vertexCount = 3;
        mesh->position.upload();
        mesh->color.upload();

        material->program = &app.renderer.shaderBasic;

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