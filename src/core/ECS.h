#pragma once

#include <entt/entt.hpp>

namespace E4 {

    using Entity = entt::entity;

    class EcsCore {

    private:
        entt::registry ecs;

    public:
        Entity createEntity() {
            return ecs.create();
        }

        template<typename T>
        T& createComponent(Entity entity) {
            return ecs.assign<T>(entity);
        }

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