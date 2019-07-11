#include "ScriptBridge.h"

#include "Lua.h"
#include "App.h"

void E4::ScriptBridge::load(App& app, sol::state& lua, Ecs& ecs) {
    lua["newEntity"] = [&app]() {
        return app.scene.newEntity().index;
    };
    lua["newEntityFromFile"] = [&app](std::string modelName) {
        return app.scene.newEntityFromFile(modelName).index;
    };

    lua["createTransform"] = [&app](uint32_t index) {
        return app.scene.createTransform(index);
    };
    lua["createDrawable"] = [&app](uint32_t index) {
        return app.scene.createDrawable(index);
    };
    lua["createScript"] = [&](uint32_t index, std::string scriptName) {
        return app.scene.createScript(index, scriptName);
    };
    lua["enableLight"] = [&](uint32_t index, LightType lightType, std::string ambient, std::string diffuse, std::string specular) {
        return app.scene.enableLight(index, lightType, ambient, diffuse, specular);
    };
    lua["disableLight"] = [&](uint32_t index) {
        return app.scene.disableLight(index);
    };
    lua["enableCamera"] = [&](uint32_t index, CameraType type, float fov) {
        return app.scene.enableCamera(index, type, fov);
    };
    lua["disableCamera"] = [&](uint32_t index) {
        return app.scene.disableCamera(index);
    };

    lua["newMaterialTexture"] = [&app](std::string textureName) {
        return app.scene.newMaterialTexture(textureName);
    };
    lua["newMaterialColor"] = [&app](std::string color) {
        return app.scene.newMaterialColor(color);
    };
    lua["newMaterialLight"] = [&app](std::string ambient, std::string diffuse) {
        return app.scene.newMaterialLight(ambient, diffuse);
    };
    lua["newMesh"] = [&app](std::string meshName) {
        return app.scene.newMesh(meshName);
    };

    lua.script(R"(
        function prepareEntity(id)
            entity = entities[id]
        end
        function runLoad(index)
            if scripts[index].load ~= nil then
                scripts[index].load()
            end
        end
        function runUpdate(index)
            if scripts[index].update ~= nil then
                scripts[index].update()
            end
        end
    )");

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
        "sz", &E4::Transform::sz,
        "parent", &E4::Transform::parent
    );
    lua.new_usertype<E4::Drawable>(
        "Drawable",
        "mesh", &E4::Drawable::mesh,
        "material", &E4::Drawable::material
    );
    lua.new_usertype<E4::Script>(
        "Script"
    );
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
    lua["inputStatePrev"] = lua.create_table_with(
        "mouseX", 0,
        "mouseY", 0,
        "mouseButton1", false,
        "mouseButton2", false,
        "mouseButton3", false,
        "keys", &frameState.inputStatePrev.keys
    );
    lua["inputStateCurr"] = lua.create_table_with(
        "mouseX", 0,
        "mouseY", 0,
        "mouseButton1", false,
        "mouseButton2", false,
        "mouseButton3", false,
        "keys", &frameState.inputStateCurr.keys
    );
}

void E4::ScriptBridge::update(App& app, sol::state& lua, Ecs& ecs, const E4::FrameState& frameState) {
    static bool loaded = false;
    if (!loaded) {
        firstFrame(lua, frameState, ecs);
        loaded = true;
    }

    lua["screenWidth"] = app.window.getWidth();
    lua["screenHeight"] = app.window.getHeight();

    lua["dt"] = frameState.deltatime * 0.001f;
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
