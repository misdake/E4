#include "Transformer.h"

#include "../../components/Transform.h"
#include "../env/Environment.h"

void update(E4::Ecs& ecs, E4::Entity& entity, E4::Transform& transform, uint64_t frameIndex, const E4::Environment& environment) {
    if (transform.lastFrame == frameIndex) return;

    E4::Mat4& mat4 = transform.world.mat4.get();
    mat4.setTRS(
        transform.x, transform.y, transform.z,
        transform.rx, transform.ry, transform.rz,
        transform.sx, transform.sy, transform.sz
    );
    if (entity.parent > 0) {
        E4::Entity& parentEntity = ecs.getEntityByIndex(entity.parent);
        auto& parentTransform = ecs.get<E4::Transform>(parentEntity);
        update(ecs, parentEntity, parentTransform, frameIndex, E4::Environment()); //TODO check resursion
        E4::Mat4::multiply(parentTransform.world.mat4.get(), mat4, mat4);
    }
    if (environment.camera.second != nullptr) {
        E4::Mat4& vp = environment.camera.second->vp;
        E4::Mat4::multiply(vp, mat4, transform.wvp.mat4.get());
    } else {
        transform.wvp.mat4.get().set(mat4);
    }
    transform.lastFrame = frameIndex;
}

void E4::Transformer::run(Ecs& ecs, const FrameState& frameState, const E4::Environment& environment) {
    if (environment.camera.second != nullptr) {
        auto& entity = ecs.getEntityByIndex(environment.camera.first);
        auto& transform = ecs.get<Transform>(entity);
        update(ecs, entity, transform, frameState.frameIndex, environment);
        environment.camera.second->action(transform.world.mat4.get(), frameState.width, frameState.height);
    }

    ecs.fortypes<Transform>([&](Entity& entity, Transform& position) {
        update(ecs, entity, position, frameState.frameIndex, environment);
    });
}
