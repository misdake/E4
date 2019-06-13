#include "ECS.h"

#include "Lua.h"

E4::Entity E4::EcsCore::createEntity() {
    E4::Entity id = ecs.create();
    sol::state& lua = *state;
    lua["entities"][id] = lua.create_table_with("id", id);
    return id;
}

E4::Transform& E4::EcsCore::createTransform(Entity entity) {
    return ecs.assign<E4::Transform>(entity);
}

E4::Drawable& E4::EcsCore::createDrawable(Entity entity) {
    return ecs.assign<E4::Drawable>(entity);
}

E4::Script& E4::EcsCore::createScript(Entity entity) {
    return ecs.assign<E4::Script>(entity);
}
