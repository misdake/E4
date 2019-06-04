#include <SDL.h>
#include <cmath>

#include "core/App.h"

int main(int argc, char* argv[]) {
    E4::App app;

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
    E4::FloatBuffer positionBuffer(position, 3, 3);
    E4::FloatBuffer colorBuffer(color, 3, 3);
    E4::ShaderData offsetData(0.2, 0, 0);

    E4::Asset<E4::Mesh> mesh;

    app.load([&mesh, &offsetData, &positionBuffer, &colorBuffer](E4::App& app) {
        positionBuffer.upload();
        colorBuffer.upload();

        mesh = app.meshes.alloc();
        E4::Asset<E4::Material> material = app.materials.alloc();
        E4::Asset<E4::Drawable> drawable = app.drawables.alloc();
        mesh->addAttribute(app.renderer.attributeSlots.POSITION, positionBuffer);
        mesh->addAttribute(app.renderer.attributeSlots.COLOR, colorBuffer);
        mesh->addUniform(app.renderer.uniformSlots.OFFSET, offsetData);
        mesh->vertexCount = 3;

        material->program = &app.renderer.shaderBasic;

        drawable->mesh = mesh;
        drawable->material = material;

        app.scene.push_back(drawable);
    });

    app.enterLoop([&offsetData](E4::App& app, const E4::FrameState& frameState) {
        static uint32_t totalTime = 0;
        totalTime += frameState.deltatime;
        offsetData.numbers.x = std::sin(totalTime / 1000.0f);
    });

    return 0;
}