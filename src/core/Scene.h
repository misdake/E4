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
        void deleteEntity(Entity& entity);
        Entity& newEntityFromFile(const std::string& modelName);
        std::reference_wrapper<Transform> createTransform(Entity& entity);
        void removeTransform(Entity& entity);
        std::reference_wrapper<Drawable> createDrawable(Entity& entity);
        void removeDrawable(Entity& entity);
        std::reference_wrapper<Script> createScript(Entity& entity, const std::string& scriptName);
        void removeScript(Entity& entity);
        std::reference_wrapper<Env> enableLight(Entity& entity, LightType lightType, const std::string& ambient, const std::string& diffuse, const std::string& specular);
        void disableLight(Entity& entity);
        std::reference_wrapper<Env> enableCamera(Entity& entity, CameraType type, float fov);
        void disableCamera(Entity& entity);
        Asset<Material> newMaterialTexture(const std::string& textureName);
        Asset<Material> newMaterialColor(const std::string& color);
        Asset<Material> newMaterialLight(const std::string& ambient, const std::string& diffuse);
        Asset<Mesh> newMesh(const std::string& meshName);
    };

}
