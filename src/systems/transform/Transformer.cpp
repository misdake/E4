#include "Transformer.h"

#include "../../components/Transform.h"
#include "../env/Environment.h"

void update(E4::Ecs& ecs, E4::Transform& transform, uint64_t frameIndex, const E4::Environment& environment) {
    if (transform.lastFrame == frameIndex) return;

    E4::Mat4& mat4 = transform.world.mat4.get();
    mat4.setTRS(
        transform.x, transform.y, transform.z,
        transform.rx, transform.ry, transform.rz,
        transform.sx, transform.sy, transform.sz
    );
    if (transform.parent > 0) {
        auto& parentTransform = ecs.get<E4::Transform>(ecs.getEntityByIndex(transform.parent));
        update(ecs, parentTransform, frameIndex, E4::Environment()); //TODO check resursion
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
        environment.camera.second->action(frameState.width, frameState.height);
    }

    ecs.fortype<Transform>([&](Entity& entity, Transform& position) {
        update(ecs, position, frameState.frameIndex, environment);
    });
}
