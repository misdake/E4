#pragma once

#include "ECS.h"
#include "../components/Components.h"

namespace sol {
    class state;
}

namespace E4 {

    class App;

    class Scene {
    private:
        App* app;
        sol::state* state;
        Ecs* ecs;

    public:
        void init(App* app, sol::state* lua, Ecs* ecs);

        Entity& newEntity();
        Entity& getEntityById(uint64_t id);
        Entity& getEntityByIndex(uint32_t index);
        std::reference_wrapper<Transform> createTransform(Entity& entity);
        std::reference_wrapper<Transform> createTransform(uint32_t index);
        std::reference_wrapper<Drawable> createDrawable(Entity& entity);
        std::reference_wrapper<Drawable> createDrawable(uint32_t index);
        std::reference_wrapper<Script> createScript(Entity& entity, const std::string& scriptName);
        std::reference_wrapper<Script> createScript(uint32_t index, const std::string& scriptName);
        Asset<Material> newMaterialTexture(const std::string& textureName);
        Asset<Material> newMaterialColor(const std::string& color);
        Asset<Mesh> newMesh(std::string meshName);
    };

}
