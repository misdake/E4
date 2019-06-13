#include "ScriptBridge.h"

#include "Lua.h"
#include "App.h"

void E4::ScriptBridge::bind(App& app, sol::state& lua, EcsCore& ecs) {
    lua["requestTransform"] = [&lua, &ecs](Entity entity) {
        lua["entities"][entity]["transform"] = std::ref(ecs.getComponent<Transform>(entity));
    };
    lua["requestDrawable"] = [&lua, &ecs](Entity entity) {
        lua["entities"][entity]["drawable"] = std::ref(ecs.getComponent<Drawable>(entity));
    };
    lua["requestScript"] = [&lua, &ecs](Entity entity) {
        lua["entities"][entity]["script"] = std::ref(ecs.getComponent<Script>(entity));
    };

    lua["createEntity"] = [&lua, &ecs]() {
        return ecs.createEntity();
    };
    lua["createTransform"] = [&](Entity entity) {
        Transform& transform = ecs.createTransform(entity);
        return std::ref<Transform>(transform);
    };

    lua["newMaterial"] = [&](std::string textureName) {
        Asset<Material> material = app.materials.alloc();
        material->texture = app.textures.get(textureName);
        material->shader = &app.renderer.shaderTexture;
        return material;
    };
    lua["newMesh"] = [&]() { //TODO load built-in mesh or file
        std::vector<float> position = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };
        std::vector<float> color = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
        };
        std::vector<float> texcoord = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
        };
        std::vector<uint16_t> index = {
            0, 1, 2,
            0, 2, 3,
        };

        Asset<Mesh> mesh = app.meshes.alloc();
        mesh->position.set(position, 3, 4).upload();
        mesh->texcoord.set(texcoord, 2, 4).upload();
        mesh->color.set(color, 3, 4).upload();
        mesh->index.set(index, 6).upload();
        return mesh;
    };

    lua["createDrawable"] = [&](Entity entity) {
        Drawable& drawable = ecs.createDrawable(entity);
        return std::ref<Drawable>(drawable);
    };

    lua["createScript"] = [&](Entity entity, std::string scriptName) {
        Script& script = ecs.createScript(entity);
        script.loaded = false;
        script.file = app.scripts.get(scriptName);
        return std::ref<Script>(script);
    };
}
