#include "ScriptBridge.h"

#include "Lua.h"
#include "App.h"
#include "../util/File.h"

void E4::ScriptBridge::load(App& app, sol::state& lua, Ecs& ecs) {
    lua.set_function("loadScriptFile", [&](const std::string& filename) {
        std::string content = readFile(app.folder, filename);
        Log::debug("script load %s", filename.c_str());
        lua.script(content, filename);
    });

    lua.set_function("newEntityNameParent", [&](const char* name, uint32_t parent) {
        return app.scene.newEntity(name, parent).index;
    });
    lua.set_function("deleteEntity", [&](uint32_t index) {
        return app.scene.deleteEntity(ecs.getEntityByIndex(index));
    });
    lua.set_function("newEntityFromFileParent", [&](const std::string& modelName, uint32_t parent) {
        return app.scene.newEntityFromFile(modelName, parent).index;
    });
    lua.set_function("findEntityByName", [&](const char* name) {
        return app.scene.findEntityByName(name).index;
    });
    lua.script(R"(
        function newEntity()
            return newEntityNameParent("", 0)
        end
        function newEntityName(name)
            return newEntityNameParent(name, 0)
        end
        function newEntityParent(parent)
            return newEntityNameParent("", parent)
        end
        function newEntityFromFile(modelName)
            return newEntityFromFileParent(modelName, parent)
        end
    )");

    lua.set_function("setActiveCamera", [&](uint32_t index) {
        return app.scene.setActiveCamera(ecs.getEntityByIndex(index));
    });
    lua.set_function("getActiveCamera", [&]() {
        return app.scene.getActiveCamera().index;
    });

    lua.set_function("createTransform", [&](uint32_t index) {
        return app.scene.createTransform(ecs.getEntityByIndex(index));
    });
    lua.set_function("removeTransform", [&](uint32_t index) {
        app.scene.removeTransform(ecs.getEntityByIndex(index));
    });

    lua.set_function("createDrawable", [&](uint32_t index) {
        return app.scene.createDrawable(ecs.getEntityByIndex(index));
    });
    lua.set_function("removeDrawable", [&](uint32_t index) {
        app.scene.removeDrawable(ecs.getEntityByIndex(index));
    });

    lua.set_function("createScript", [&](uint32_t index, const std::string& scriptName) {
        return app.scene.createScript(ecs.getEntityByIndex(index), scriptName);
    });
    lua.set_function("removeScript", [&](uint32_t index) {
        app.scene.removeScript(ecs.getEntityByIndex(index));
    });

    lua.set_function("enableLight", [&](uint32_t index, LightType lightType, const std::string& ambient, const std::string& diffuse, const std::string& specular) {
        return app.scene.enableLight(ecs.getEntityByIndex(index), lightType, ambient, diffuse, specular);
    });
    lua.set_function("disableLight", [&](uint32_t index) {
        return app.scene.disableLight(ecs.getEntityByIndex(index));
    });
    lua.set_function("enableCamera", [&](uint32_t index, CameraType type, float fov) {
        return app.scene.enableCamera(ecs.getEntityByIndex(index), type, fov);
    });
    lua.set_function("disableCamera", [&](uint32_t index) {
        return app.scene.disableCamera(ecs.getEntityByIndex(index));
    });

    lua.set_function("newMaterialTexture", [&](const std::string& textureName) {
        return app.scene.newMaterialTexture(textureName);
    });
    lua.set_function("newMaterialColor", [&](const std::string& color) {
        return app.scene.newMaterialColor(color);
    });
    lua.set_function("newMaterialLight", [&](const std::string& ambient, const std::string& diffuse) {
        return app.scene.newMaterialLight(ambient, diffuse);
    });
    lua.set_function("newMesh", [&](const std::string& meshName) {
        return app.scene.newMesh(meshName);
    });
    lua.set_function("setMaterialTextureTile", [&](Asset<Material>& material, const std::string& tilename) {
        app.scene.setMaterialTextureTile(material, tilename);
    });

    lua.set_function("setLogFilter", [&](const std::string& filter) {
        Log::setFilter(filter.c_str());
    });
    lua.set_function("setWindowTitle", [&](const std::string& title) {
        app.window.setTitle(title);
    });
    lua.set_function("readFile", [&](const std::string& fileName) {
        return readFile(app.folder, fileName);
    });
    lua.set_function("writeFile", [&](const std::string& fileName, const std::string& content) {
        writeFile(app.folder, fileName, content);
    });
    lua.script(R"(
        function mouse1down()
            return (not inputStatePrev.mouseButton1) and inputStateCurr.mouseButton1
        end
        function mouse2down()
            return (not inputStatePrev.mouseButton2) and inputStateCurr.mouseButton2
        end
        function mouse3down()
            return (not inputStatePrev.mouseButton3) and inputStateCurr.mouseButton3
        end
        function keydown(key)
            return not (inputStatePrev.keys[key + 1] > 0) and (inputStateCurr.keys[key + 1] > 0)
        end

        function saveScriptFunctions(index)
            scripts[index] = {}
            scripts[index].load = load
            scripts[index].update = update
            load = nil
            update = nil
        end

        function prepareEntity(id)
            entity = entities[id]
        end
        function runLoad(index)
            scripts[index].load()
        end
        function runUpdate(index)
            scripts[index].update()
        end
    )", "ScriptBridge");

    lua["entities"] = lua.create_table();
    lua["scripts"] = lua.create_table();

    lua.new_usertype<E4::Transform>(
        "Transform",
        "x", &E4::Transform::x,
        "y", &E4::Transform::y,
        "z", &E4::Transform::z,
        "rx", &E4::Transform::rx,
        "ry", &E4::Transform::ry,
        "rz", &E4::Transform::rz,
        "sx", &E4::Transform::sx,
        "sy", &E4::Transform::sy,
        "sz", &E4::Transform::sz
    );
    lua.new_usertype<E4::Drawable>(
        "Drawable",
        "visible", &E4::Drawable::visible,
        "mesh", &E4::Drawable::mesh,
        "material", &E4::Drawable::material
    );
//    lua.new_usertype<E4::Script>(
//        "Script"
//    );
//    lua.new_usertype<E4::Env>(
//        "Env",
//        "camera", &E4::Env::camera,
//        "light", &E4::Env::light
//    );
//    lua.new_usertype<E4::Camera>(
//        "Camera",
//        "enabled", &E4::Camera::enabled
//    );
//    lua.new_usertype<E4::Light>(
//        "Light",
//        "enabled", &E4::Light::enabled,
//        "type", &E4::Light::type
//    );

    lua["LightType"] = lua.create_table_with("POINT", LightType::POINT, "DIRECTIONAL", LightType::DIRECTIONAL);
    lua["CameraType"] = lua.create_table_with("ORTHO", CameraType::ORTHO, "PROJ", CameraType::PROJ);
}

void firstFrame(sol::state& lua, const E4::FrameState& frameState, E4::Ecs& ecs) {
    lua["inputStatePrev"] = lua.create_table();
    lua["inputStateCurr"] = lua.create_table();
}

void E4::ScriptBridge::update(App& app, sol::state& lua, Ecs& ecs, const E4::FrameState& frameState) {
    static bool loaded = false;
    if (!loaded) {
        firstFrame(lua, frameState, ecs);
        loaded = true;
    }

    lua["screenWidth"] = app.window.getWidth();
    lua["screenHeight"] = app.window.getHeight();

    lua["dt"] = static_cast<float>(frameState.deltatime) * 0.001f;
    lua["frameIndex"] = frameState.frameIndex;
    lua["inputStatePrev"]["wheelX"] = frameState.inputStatePrev.wheelX;
    lua["inputStatePrev"]["wheelY"] = frameState.inputStatePrev.wheelY;
    lua["inputStatePrev"]["mouseX"] = frameState.inputStatePrev.mouseX;
    lua["inputStatePrev"]["mouseY"] = frameState.inputStatePrev.mouseY;
    lua["inputStatePrev"]["mouseButton1"] = frameState.inputStatePrev.mouseButton1;
    lua["inputStatePrev"]["mouseButton2"] = frameState.inputStatePrev.mouseButton2;
    lua["inputStatePrev"]["mouseButton3"] = frameState.inputStatePrev.mouseButton3;
    lua["inputStatePrev"]["keys"] = std::ref(frameState.inputStatePrev.keys);
    lua["inputStateCurr"]["wheelX"] = frameState.inputStateCurr.wheelX;
    lua["inputStateCurr"]["wheelY"] = frameState.inputStateCurr.wheelY;
    lua["inputStateCurr"]["mouseX"] = frameState.inputStateCurr.mouseX;
    lua["inputStateCurr"]["mouseY"] = frameState.inputStateCurr.mouseY;
    lua["inputStateCurr"]["mouseButton1"] = frameState.inputStateCurr.mouseButton1;
    lua["inputStateCurr"]["mouseButton2"] = frameState.inputStateCurr.mouseButton2;
    lua["inputStateCurr"]["mouseButton3"] = frameState.inputStateCurr.mouseButton3;
    lua["inputStateCurr"]["keys"] = std::ref(frameState.inputStateCurr.keys);
}
