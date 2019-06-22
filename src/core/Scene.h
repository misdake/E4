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
        Entity& newEntityFromFile(const std::string& modelName);
        std::reference_wrapper<Transform> createTransform(Entity& entity);
        std::reference_wrapper<Transform> createTransform(uint32_t index);
        std::reference_wrapper<Drawable> createDrawable(Entity& entity);
        std::reference_wrapper<Drawable> createDrawable(uint32_t index);
        std::reference_wrapper<Script> createScript(Entity& entity, const std::string& scriptName);
        std::reference_wrapper<Script> createScript(uint32_t index, const std::string& scriptName);
        std::reference_wrapper<Env> createEnv(Entity& entity, const std::string& color);
        std::reference_wrapper<Env> createEnv(uint32_t index, const std::string& color);
        Asset<Material> newMaterialTexture(const std::string& textureName);
        Asset<Material> newMaterialColor(const std::string& color);
        Asset<Material> newMaterialLight(const std::string& color);
        Asset<Mesh> newMesh(const std::string& meshName);
    };

}
