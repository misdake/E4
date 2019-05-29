#include "GlRenderer.h"

void E4::GlRenderer::resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void E4::GlRenderer::init() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //TODO move to glstate
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void E4::GlRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void E4::GlRenderer::draw(const E4::Drawable& drawable) {
    Material& material = *(drawable.material);
    material.program->use();

    Mesh& mesh = *(drawable.mesh);
    for (auto& pair: mesh.attributes) {
        AttributeSlot* slot = pair.first;
        FloatBuffer* buffer = pair.second;
        slot->bind(*buffer);
    }
    for (auto& pair: mesh.uniforms) {
        UniformSlot* slot = pair.first;
        ShaderData* buffer = pair.second;
        slot->bind(*buffer);
    }
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
}

void E4::GlRenderer::checkError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "error" << std::endl;
    }
}
