#include "ScriptBridge.h"

#include "Lua.h"
#include "App.h"

void E4::ScriptBridge::load(App& app, sol::state& lua, EcsCore& ecs) {
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

    lua["newMaterialTexture"] = [&](std::string textureName) {
        Asset<Material> material = app.materials.alloc();
        material->texture = app.textures.get(textureName);
        material->shader = &app.renderer.shaderTexture;
        return material;
    };
    lua["newMaterialColor"] = [&](std::string color) {
        Asset<Material> material = app.materials.alloc();
        material->color.color.set(color);
        material->shader = &app.renderer.shaderBasic;
        return material;
    };
    lua["newMesh"] = [&](std::string meshName) {
        if (meshName.rfind("builtin:", 0) == 0) {
            return app.meshLoader.create(meshName, app.meshes);
        } else {
            //TODO load from file
            return Asset<Mesh>();
        }
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

    lua.script(R"(
        function mouse1down()
            return (not inputStatePrev.mouseButton1) and (inputStateCurr.mouseButton1)
        end
    )");
}

void E4::ScriptBridge::update(App& app, sol::state& lua, EcsCore& ecs) {
    lua["screenWidth"] = app.window.getWidth();
    lua["screenHeight"] = app.window.getHeight();
}
