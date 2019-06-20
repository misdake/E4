#include "Renderer.h"

#include "../../core/ECS.h"
#include "../../components/Transform.h"
#include "../../components/Drawable.h"
#include "Environment.h"

void E4::Renderer::init() {
    glRenderer.init();
}
void E4::Renderer::resize(int w, int h) {
    glRenderer.resize(w, h);
}
void E4::Renderer::run(Ecs& ecs, const E4::FrameState& state) {
    Environment environment;
    ecs.fortype<E4::Transform, E4::Env>([&](Entity& entity, Transform& transform, Env& env) {
        if (env.light.enabled) {
            environment.lights.push_back(&env.light);
        }
    });

    glRenderer.clear();
    ecs.fortype<E4::Transform, E4::Drawable>([&](Entity& entity, Transform& transform, Drawable& drawable) {
        glRenderer.draw(transform, drawable, environment);
    });
    glRenderer.checkError();
}
