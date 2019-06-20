#include "Scene.h"

#include "Lua.h"
#include "App.h"

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
E4::Entity& E4::Scene::getEntityById(uint64_t id) {
    return ecs->getEntityById(id);
}
E4::Entity& E4::Scene::getEntityByIndex(uint32_t index) {
    return ecs->getEntityByIndex(index);
}

std::reference_wrapper<E4::Transform> E4::Scene::createTransform(E4::Entity& entity) {
    sol::state& lua = *state;
    auto& transform = ecs->create<Transform>(entity);
    transform.sx = transform.sy = transform.sz = 1;
    transform.worldTransform.mat4 = app->mat4.alloc();
    lua["entities"][entity.index]["transform"] = std::ref<Transform>(transform);
    return std::ref<Transform>(transform);
}
std::reference_wrapper<E4::Transform> E4::Scene::createTransform(uint32_t index) {
    return createTransform(ecs->getEntityByIndex(index));
}

std::reference_wrapper<E4::Drawable> E4::Scene::createDrawable(Entity& entity) {
    sol::state& lua = *state;
    auto& drawable = ecs->create<Drawable>(entity);
    lua["entities"][entity.index]["drawable"] = std::ref<Drawable>(drawable);
    return std::ref<Drawable>(drawable);
}
std::reference_wrapper<E4::Drawable> E4::Scene::createDrawable(uint32_t index) {
    return createDrawable(ecs->getEntityByIndex(index));
}

std::reference_wrapper<E4::Script> E4::Scene::createScript(Entity& entity, const std::string& scriptName) {
    sol::state& lua = *state;
    auto& script = ecs->create<Script>(entity);
    script.loaded = false;
    script.file = app->scripts.get(scriptName);
    lua["entities"][entity.index]["script"] = std::ref<Script>(script);
    return std::ref<Script>(script);
}
std::reference_wrapper<E4::Script> E4::Scene::createScript(uint32_t index, const std::string& scriptName) {
    return createScript(ecs->getEntityByIndex(index), scriptName);
}

E4::Asset<E4::Material> E4::Scene::newMaterialTexture(const std::string& textureName) {
    Asset<Material> material = app->materials.alloc();
    material->texture = app->textures.get(textureName);
    material->shader = &app->renderer.shaderTexture;
    return material;
}
E4::Asset<E4::Material> E4::Scene::newMaterialColor(const std::string& color) {
    Asset<Material> material = app->materials.alloc();
    material->color.color.set(color);
    material->shader = &app->renderer.shaderBasic;
    return material;
}

static bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

E4::Asset<E4::Mesh> E4::Scene::newMesh(std::string meshName) {
    static const std::string prefix_builtin = "builtin:";
    static const uint64_t prefix_builtin_len = prefix_builtin.length();
    static const std::string prefix_file = "file:";
    static const uint64_t prefix_file_len = prefix_file.length();
    if (startsWith(meshName, prefix_builtin)) {
        return app->meshLoader_builtin.create(meshName.substr(prefix_builtin_len), app->meshes);
    } else if (startsWith(meshName, prefix_file)) {
        if (endsWith(meshName, ".obj")) {
            return app->meshLoader_obj.create(app->folder, meshName.substr(prefix_file_len), app->meshes);
        }
    }
    return Asset<Mesh>();
}
