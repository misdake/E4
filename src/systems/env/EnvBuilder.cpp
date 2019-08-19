#include "EnvBuilder.h"

void E4::EnvBuilder::run(E4::Ecs& ecs, const E4::FrameState& state) {
    environment.camera.first = 0;
    environment.camera.second = nullptr;
    environment.lights.clear();

    ecs.fortypes<E4::Transform, E4::Env>([&](Entity& entity, Transform& transform, Env& env) {
        if (env.camera.enabled) {
            if (state.activeCamera == 0 || entity.index == state.activeCamera) {
                environment.camera.first = entity.index;
                environment.camera.second = &env.camera;
            }
        }
        if (env.light.enabled) {
            environment.lights.emplace_back(&transform, &env.light);
        }
    });
}
