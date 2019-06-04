#include "Renderer.h"

void E4::Renderer::init() {
    glRenderer.init();
}
void E4::Renderer::resize(int w, int h) {
    glRenderer.resize(w, h);
}

void E4::Renderer::run(std::vector<E4::Asset<E4::Drawable>>& scene) {
    glRenderer.clear();

    for (auto& drawable: scene) {
        glRenderer.draw(drawable.get());
    }

    glRenderer.checkError();
}
