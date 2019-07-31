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

void E4::Scene::start() {
    E4::Entity& e = newEntity();
    createTransform(e);
    createScript(e, "main.lua");
}
void E4::Scene::reloadAll() {
    clearEntity();
    clearAsset();
    reloadScript();
    start();
}
void E4::Scene::reloadScript() {
    ecs->fortype<E4::Script>([](E4::Script& script) {
        script.file->scriptLoaded = false;
    });
}

E4::Entity& E4::Scene::newEntity() {
    sol::state& lua = *state;
    Entity& e = ecs->newEntity();
    lua["entities"][e.index] = lua.create_table_with("index", e.index);
    Log::debug("scene: newEntity %d", e.index);
    return e;
}
void E4::Scene::deleteEntity(E4::Entity& entity) {
    Log::debug("scene: deleteEntity %d", entity.index);
    sol::state& lua = *state;
    removeTransform(entity);
    removeDrawable(entity);
    removeScript(entity);
    removeEnv(entity);
    lua["entities"][entity.index] = lua.create_table_with("index", entity.index);
    ecs->deleteEntity(entity);
}
void E4::Scene::clearEntity() {
    std::vector<uint32_t> indices{};
    ecs->foreach([&](E4::Entity& entity) {
        indices.push_back(entity.index);
        removeTransform(entity);
        removeDrawable(entity);
        removeScript(entity);
        removeEnv(entity);
    });
    for (uint32_t index : indices) {
        ecs->deleteEntity(ecs->getEntityByIndex(index));
    }
}
void E4::Scene::clearAsset() {
//    Log::debug("clearAsset mat4.size() => %d", app->mat4.size());
//    app->mat4.clear(); //Mat4 should have been fully recycled.
    //Mesh, Material, Texture will be dealt by Recycler. (and possibly)
    //Script will be reloaded.
}
E4::Entity& E4::Scene::newEntityFromFile(const std::string& modelName) {
    Log::debug("scene: newEntityFromFile %s", modelName.c_str());
    if (endsWith(modelName, ".obj")) {
        uint64_t entityId = app->meshLoader_obj.create(app->folder, modelName, *app);
        return ecs->getEntityById(entityId);
    }
    Log::error("newEntityFromFile: unknown model format, %s", modelName.c_str());
    return ecs->getEntityById(0);
}

std::reference_wrapper<E4::Transform> E4::Scene::createTransform(E4::Entity& entity) {
    Log::debug("scene: createTransform %d", entity.index);
    sol::state& lua = *state;
    auto& transform = ecs->create<Transform>(entity);
    transform.sx = transform.sy = transform.sz = 1;
    transform.world.mat4 = app->mat4.alloc();
    transform.wvp.mat4 = app->mat4.alloc();
    lua["entities"][entity.index]["transform"] = std::ref<Transform>(transform);
    return std::ref<Transform>(transform);
}
void E4::Scene::removeTransform(E4::Entity& entity) {
    if (ecs->has<Transform>(entity)) {
        Log::debug("scene: removeTransform %d", entity.index);
        auto& transform = ecs->get<Transform>(entity);
        transform.world.mat4.free();
        transform.wvp.mat4.free();
        sol::state& lua = *state;
        lua["entities"][entity.index]["transform"] = sol::nil;
    }
}

std::reference_wrapper<E4::Drawable> E4::Scene::createDrawable(Entity& entity) {
    Log::debug("scene: createDrawable %d", entity.index);
    sol::state& lua = *state;
    auto& drawable = ecs->create<Drawable>(entity);
    lua["entities"][entity.index]["drawable"] = std::ref<Drawable>(drawable);
    return std::ref<Drawable>(drawable);
}
void E4::Scene::removeDrawable(E4::Entity& entity) {
    if (ecs->has<Drawable>(entity)) {
        Log::debug("scene: removeDrawable %d", entity.index);
        auto& drawable = ecs->get<Drawable>(entity);
        sol::state& lua = *state;
        lua["entities"][entity.index]["drawable"] = sol::nil;
    }
}

std::reference_wrapper<E4::Script> E4::Scene::createScript(Entity& entity, const std::string& scriptName) {
    Log::debug("scene: createScript %d %s", entity.index, scriptName.c_str());
    sol::state& lua = *state;
    auto& script = ecs->create<Script>(entity);
    script.loaded = false;
    script.file = app->scripts.get(scriptName);
    lua["entities"][entity.index]["script"] = std::ref<Script>(script);
    return std::ref<Script>(script);
}
void E4::Scene::removeScript(E4::Entity& entity) {
    if (ecs->has<Script>(entity)) {
        Log::debug("scene: removeScript %d", entity.index);
        auto& script = ecs->get<Script>(entity);
        sol::state& lua = *state;
        lua["entities"][entity.index]["script"] = sol::nil;
    }
}

std::reference_wrapper<E4::Env> E4::Scene::enableLight(E4::Entity& entity, LightType lightType, const std::string& ambient, const std::string& diffuse, const std::string& specular) {
    Log::debug("scene: enableLight %d", entity.index);
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
        Log::debug("scene: disableLight %d", entity.index);
        ecs->get<Env>(entity).light.enabled = false;
    }
}
std::reference_wrapper<E4::Env> E4::Scene::enableCamera(E4::Entity& entity, CameraType cameraType, float fov) {
    Log::debug("scene: enableCamera %d", entity.index);
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
        Log::debug("scene: disableCamera %d", entity.index);
        ecs->get<Env>(entity).camera.enabled = false;
    }
}
void E4::Scene::removeEnv(E4::Entity& entity) {
    if (ecs->has<Env>(entity)) {
        Log::debug("scene: removeEnv %d", entity.index);
        auto& env = ecs->get<Env>(entity);
        sol::state& lua = *state;
        lua["entities"][entity.index]["env"] = sol::nil;
    }
}

E4::Asset<E4::Material> E4::Scene::newMaterialTexture(const std::string& textureName) {
    Log::debug("scene: newMaterialTexture %s", textureName.c_str());
    Asset <Material> material = app->materials.alloc();
    material->texture = app->textures.get(textureName);
    material->shader = &app->renderer.shaderTexture;
    return material;
}
E4::Asset<E4::Material> E4::Scene::newMaterialColor(const std::string& color) {
    Log::debug("scene: newMaterialColor %s", color.c_str());
    Asset <Material> material = app->materials.alloc();
    material->ambient.color.set(color);
    material->shader = &app->renderer.shaderBasic;
    return material;
}
E4::Asset<E4::Material> E4::Scene::newMaterialLight(const std::string& ambient, const std::string& diffuse) {
    Log::debug("scene: newMaterialLight %s %s", ambient.c_str(), diffuse.c_str());
    Asset <Material> material = app->materials.alloc();
    material->ambient.color.set(ambient);
    material->diffuse.color.set(diffuse);
    material->specular.color.set(diffuse); //TODO
    material->specularExp.set(10); //TODO
    material->shader = &app->renderer.shaderLight;
    return material;
}

E4::Asset<E4::Mesh> E4::Scene::newMesh(const std::string& meshName) {
    Log::debug("scene: newMesh %s", meshName.c_str());
    static const std::string prefix_builtin = "builtin:";
    static const uint64_t prefix_builtin_len = prefix_builtin.length();
    if (startsWith(meshName, prefix_builtin)) {
        return app->meshLoader_builtin.create(meshName.substr(prefix_builtin_len), app->meshes);
    }
    Log::error("newMesh: unknown mesh, %s", meshName.c_str());
    return Asset<Mesh>();
}
