#pragma once

#include "ECS.h"

#include "Asset.h"
#include "../components/Drawable.h"
#include "../systems/script/ScriptFile.h"

#include "../systems/render/Renderer.h"
#include "../systems/transform/Transformer.h"
#include "../systems/script/ScriptRunner.h"

#include "../util/Window.h"
#include "assets/BuiltinMesh.h"

namespace E4 {

    class App {
    public:
        Window window;

        //ecs
        Ecs ecs;

        //systems
        Transformer transform;
        Renderer renderer;
        ScriptRunner scriptRunner;

        //resources
        AssetPool<Mat4> mat4;
        AssetPool<Mesh> meshes;
        AssetPool<Material> materials;
        AssetLoader<Texture> textures;
        AssetLoader<ScriptFile> scripts;
        BuiltinMeshLoader meshLoader;

        uint16_t width;
        uint16_t height;
        std::string folder;

        explicit App(uint16_t width, uint16_t height, const std::string& folder);

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
