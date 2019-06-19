#include "Transformer.h"

#include "../../components/Transform.h"

void update(E4::Ecs& ecs, E4::Transform& transform, uint64_t frameIndex) {
    if (transform.lastFrame == frameIndex) return;

    E4::Mat4& mat4 = transform.worldTransform.mat4.get();
    transform.worldTransform.mat4->setTRS(
        transform.x, transform.y, transform.z,
        transform.rx, transform.ry, transform.rz,
        transform.sx, transform.sy, transform.sz
    );
    if (transform.parent > 0) {
        auto& parentTransform = ecs.get<E4::Transform>(ecs.getEntityById(transform.parent));
        update(ecs, parentTransform, frameIndex); //TODO check resursion
        E4::Mat4::multiply(parentTransform.worldTransform.mat4.get(), mat4, mat4);
    }
    transform.lastFrame = frameIndex;
}

void E4::Transformer::run(E4::Ecs& ecs, E4::FrameState frameState) {
    ecs.fortype<Transform>([&](Entity& entity, Transform& position) {
        update(ecs, position, frameState.frameIndex);
    });
}
