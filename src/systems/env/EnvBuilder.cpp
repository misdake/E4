#include "EnvBuilder.h"

void E4::EnvBuilder::run(E4::Ecs& ecs, const E4::FrameState& state) {
    environment.camera.first = nullptr;
    environment.camera.second = nullptr;
    environment.lights.clear();

    ecs.fortype<E4::Transform, E4::Env>([&](Entity& entity, Transform& transform, Env& env) {
        if (env.camera.enabled) {
            environment.camera.first = &transform;
            environment.camera.second = &env.camera;
        }
        if (env.light.enabled) {
            environment.lights.emplace_back(&transform, &env.light);
        }
    });
}
