#include "Renderer.h"

#include "../../components/Transform.h"
#include "../../components/Drawable.h"
#include "../env/Environment.h"
#include "../../core/ECS.h"

void E4::Renderer::init() {
    glRenderer.init();
}
void E4::Renderer::resize(int w, int h) {
    glRenderer.resize(w, h);
}
void E4::Renderer::run(Ecs& ecs, const E4::FrameState& state, const E4::Environment& environment) {
    for(auto&[transform, light] : environment.lights) {
        Mat4& world = transform->world.mat4.get();
        light->transform(world);
    }

    glRenderer.clear();
    ecs.fortypes<E4::Transform, E4::Drawable>([&](Entity& entity, Transform& transform, Drawable& drawable) {
        if (drawable.visible) {
            glRenderer.draw(transform, drawable, environment);
        }
    });
    glRenderer.checkError();
}
