#include "Transform.h"

void update(E4::ECS& ecs, E4::Position& position, uint64_t frameIndex) {
    if (position.lastFrame == frameIndex) return;

    if (position.parent > 0) {
        E4::Position& parentPosition = ecs.get<E4::Position>(position.parent);
        update(ecs, parentPosition, frameIndex);
        position.worldX = position.x + parentPosition.x;
        position.worldY = position.y + parentPosition.y;
    } else {
        position.worldX = position.x;
        position.worldY = position.y;
    }
    position.lastFrame = frameIndex;
}

void E4::Transform::run(E4::ECS& ecs, E4::FrameState frameState) {
    ecs.view<Position>().each([&](Position& position) {
        update(ecs, position, frameState.frameIndex);
    });
}
