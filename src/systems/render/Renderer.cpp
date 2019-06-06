#include "Renderer.h"

#include "../../components/Drawable.h"

void E4::Renderer::init() {
    glRenderer.init();
}
void E4::Renderer::resize(int w, int h) {
    glRenderer.resize(w, h);
}
void E4::Renderer::run(ECS& ecs) {
    glRenderer.clear();
    ecs.view<E4::Transform, E4::Drawable>().each([&](E4::Transform transform, E4::Drawable& drawable) {
        glRenderer.draw(transform, drawable);
    });
    glRenderer.checkError();
}
