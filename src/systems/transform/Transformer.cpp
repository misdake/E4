#include "Transformer.h"

#include "../../components/Transform.h"

void update(E4::ECS& ecs, E4::Transform& position, uint64_t frameIndex) {
    if (position.lastFrame == frameIndex) return;

    if (position.parent > 0) {
        E4::Transform& parentPosition = ecs.get<E4::Transform>(position.parent);
        update(ecs, parentPosition, frameIndex);
        position.worldTransform.set(
            position.x + parentPosition.x,
            position.y + parentPosition.y,
            position.z + parentPosition.z
        );
    } else {
        position.worldTransform.set(
            position.x,
            position.y,
            position.z
        );
    }
    position.lastFrame = frameIndex;
}

void E4::Transformer::run(E4::ECS& ecs, E4::FrameState frameState) {
    ecs.view<Transform>().each([&](Transform& position) {
        update(ecs, position, frameState.frameIndex);
    });
}
