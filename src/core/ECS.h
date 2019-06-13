#pragma once

#include <entt/entt.hpp>
#include "../components/Components.h"

namespace sol {
    class state;
}

namespace E4 {

    using Entity = entt::entity;

    class EcsCore {

    private:
        friend class App;

        entt::registry ecs;
        sol::state* state;

    public:
        Entity createEntity();

        Transform& createTransform(Entity entity);
        Drawable& createDrawable(Entity entity);
        Script& createScript(Entity entity);

        template<typename T>
        T& getComponent(Entity entity) {
            return ecs.get<T>(entity);
        }

        //TODO delete component

        //TODO delete entity

        template<typename... Component>
        entt::basic_view<Entity, Component...> view() {
            return ecs.view<Component...>();
        }

    };

}