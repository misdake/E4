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
            Mat4& world = transform.worldTransform.mat4.get();
            Light& light = env.light;
            Vec3 lightDirection = world * Vec3(light.direction.numbers.x, light.direction.numbers.y, light.direction.numbers.z);
            environment.lights.emplace_back(lightDirection, &light);
        }
    });

    glRenderer.clear();
    ecs.fortype<E4::Transform, E4::Drawable>([&](Entity& entity, Transform& transform, Drawable& drawable) {
        glRenderer.draw(transform, drawable, environment);
    });
    glRenderer.checkError();
}
