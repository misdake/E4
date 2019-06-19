#include "Renderer.h"

#include "../../core/ECS.h"
#include "../../components/Transform.h"
#include "../../components/Drawable.h"

void E4::Renderer::init() {
    glRenderer.init();
}
void E4::Renderer::resize(int w, int h) {
    glRenderer.resize(w, h);
}
void E4::Renderer::run(Ecs& ecs, const E4::FrameState& state) {
    glRenderer.clear();
    ecs.fortype<E4::Transform, E4::Drawable>([&](Entity& entity, Transform& transform, Drawable& drawable) {
        glRenderer.draw(transform, drawable);
    });
    glRenderer.checkError();
}
