#include "Scene.h"

#include "Lua.h"
#include "App.h"

static bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

void E4::Scene::init(E4::App* _app, sol::state* _lua, E4::Ecs* _ecs) {
    app = _app;
    state = _lua;
    ecs = _ecs;
}

E4::Entity& E4::Scene::newEntity() {
    sol::state& lua = *state;
    Entity& e = ecs->newEntity();
    lua["entities"][e.index] = lua.create_table_with("index", e.index);
    return e;
}
void E4::Scene::deleteEntity(E4::Entity& entity) {
    sol::state& lua = *state;
    lua["entities"][entity.index] = lua.create_table_with("index", entity.index);
    ecs->deleteEntity(entity);
}
E4::Entity& E4::Scene::newEntityFromFile(const std::string& modelName) {
    if (endsWith(modelName, ".obj")) {
        uint64_t entityId = app->meshLoader_obj.create(app->folder, modelName, *app);
        return ecs->getEntityById(entityId);
    }
    Log::error("newEntityFromFile: unknown model format, %s", modelName.c_str());
    return ecs->getEntityById(0);
}

std::reference_wrapper<E4::Transform> E4::Scene::createTransform(E4::Entity& entity) {
    sol::state& lua = *state;
    auto& transform = ecs->create<Transform>(entity);
    transform.sx = transform.sy = transform.sz = 1;
    transform.world.mat4 = app->mat4.alloc();
    transform.wvp.mat4 = app->mat4.alloc();
    lua["entities"][entity.index]["transform"] = std::ref<Transform>(transform);
    return std::ref<Transform>(transform);
}

std::reference_wrapper<E4::Drawable> E4::Scene::createDrawable(Entity& entity) {
    sol::state& lua = *state;
    auto& drawable = ecs->create<Drawable>(entity);
    lua["entities"][entity.index]["drawable"] = std::ref<Drawable>(drawable);
    return std::ref<Drawable>(drawable);
}

std::reference_wrapper<E4::Script> E4::Scene::createScript(Entity& entity, const std::string& scriptName) {
    sol::state& lua = *state;
    auto& script = ecs->create<Script>(entity);
    script.loaded = false;
    script.file = app->scripts.get(scriptName);
    lua["entities"][entity.index]["script"] = std::ref<Script>(script);
    return std::ref<Script>(script);
}

std::reference_wrapper<E4::Env> E4::Scene::enableLight(E4::Entity& entity, LightType lightType, const std::string& ambient, const std::string& diffuse, const std::string& specular) {
    sol::state& lua = *state;
    bool created = false;
    auto& env = ecs->getOrCreate<Env>(entity, created);
    if (created) lua["entities"][entity.index]["env"] = std::ref<Env>(env);
    env.light.enabled = true;
    env.light.type = lightType;
    env.light.ambient.color.set(ambient);
    env.light.diffuse.color.set(diffuse);
    env.light.specular.color.set(specular);
    return std::ref<Env>(env);
}

void E4::Scene::disableLight(E4::Entity& entity) {
    sol::state& lua = *state;
    if (ecs->has<Env>(entity)) {
        ecs->get<Env>(entity).light.enabled = false;
    }
}

std::reference_wrapper<E4::Env> E4::Scene::enableCamera(E4::Entity& entity, CameraType cameraType, float fov) {
    sol::state& lua = *state;
    bool created = false;
    auto& env = ecs->getOrCreate<Env>(entity, created);
    if (created) lua["entities"][entity.index]["env"] = std::ref<Env>(env);
    env.camera.enabled = true;
    env.camera.init();
    env.camera.type = cameraType;
    env.camera.fov = fov;
    return std::ref<Env>(env);
}

void E4::Scene::disableCamera(E4::Entity& entity) {
    sol::state& lua = *state;
    if (ecs->has<Env>(entity)) {
        ecs->get<Env>(entity).camera.enabled = false;
    }
}

E4::Asset<E4::Material> E4::Scene::newMaterialTexture(const std::string& textureName) {
    Asset<Material> material = app->materials.alloc();
    material->texture = app->textures.get(textureName);
    material->shader = &app->renderer.shaderTexture;
    return material;
}
E4::Asset<E4::Material> E4::Scene::newMaterialColor(const std::string& color) {
    Asset<Material> material = app->materials.alloc();
    material->ambient.color.set(color);
    material->shader = &app->renderer.shaderBasic;
    return material;
}
E4::Asset<E4::Material> E4::Scene::newMaterialLight(const std::string& ambient, const std::string& diffuse) {
    Asset<Material> material = app->materials.alloc();
    material->ambient.color.set(ambient);
    material->diffuse.color.set(diffuse);
    material->specular.color.set(diffuse); //TODO
    material->specularExp.set(10); //TODO
    material->shader = &app->renderer.shaderLight;
    return material;
}

E4::Asset<E4::Mesh> E4::Scene::newMesh(const std::string& meshName) {
    static const std::string prefix_builtin = "builtin:";
    static const uint64_t prefix_builtin_len = prefix_builtin.length();
    if (startsWith(meshName, prefix_builtin)) {
        return app->meshLoader_builtin.create(meshName.substr(prefix_builtin_len), app->meshes);
    }
    Log::error("newMesh: unknown mesh, %s", meshName.c_str());
    return Asset<Mesh>();
}
