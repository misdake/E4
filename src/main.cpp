#include <SDL.h>

#include <cmath>

#include "systems/render/Texture.h"
#include "systems/render/Renderer.h"
#include "systems/render/opengl/Shader.h"
#include "systems/render/opengl/GlRenderer.h"
#include "util/Window.h"
#include "util/File.h"
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

    E4::Shader* shader;
    E4::Asset<E4::Mesh> mesh;

    app.load([&](E4::App& app) {
        positionBuffer.upload();
        colorBuffer.upload();

        std::string vsContent = E4::readFile("shader_basic_vs.txt");
        std::string psContent = E4::readFile("shader_basic_ps.txt");
        shader = new E4::Shader(vsContent, psContent);
        shader->addVertexAttribute(app.renderer.attributeSlots.POSITION);
        shader->addVertexAttribute(app.renderer.attributeSlots.COLOR);
        shader->addVertexUniform(app.renderer.uniformSlots.OFFSET);
        shader->compile();

        mesh = app.meshes.alloc();
        E4::Asset<E4::Material> material = app.materials.alloc();
        E4::Asset<E4::Drawable> drawable = app.drawables.alloc();
        mesh->addAttribute(app.renderer.attributeSlots.POSITION, positionBuffer);
        mesh->addAttribute(app.renderer.attributeSlots.COLOR, colorBuffer);
        mesh->addUniform(app.renderer.uniformSlots.OFFSET, offsetData);
        mesh->vertexCount = 3;

        material->program = shader;

        drawable->mesh = mesh;
        drawable->material = material;

        app.scene.push_back(drawable);
    });

    uint32_t totalTime = 0;

    app.enterLoop([&](E4::App& app, const E4::FrameState& frameState) {
        E4::ShaderData* data = mesh->uniforms[0].second;
        totalTime += frameState.deltatime;
        data->numbers.x = std::sin(totalTime / 1000.0f);
    });

    return 0;
}