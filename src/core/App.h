#pragma once

#include "ECS.h"
#include "Scene.h"

#include "Asset.h"
#include "../components/Drawable.h"
#include "../systems/script/ScriptFile.h"

#include "../systems/render/Renderer.h"
#include "../systems/transform/Transformer.h"
#include "../systems/script/ScriptRunner.h"

#include "../util/Window.h"
#include "../assets/MeshLoader_builtin.h"
#include "../assets/MeshLoader_obj.h"
#include "../systems/env/EnvBuilder.h"
#include "../systems/recycler/Recycler.h"
#include "../util/StringPool.h"

namespace E4 {

    class App {
    public:
        FrameState frameState;
        Window window;

        //ecs
        Ecs ecs;
        Scene scene;

        uint16_t width;
        uint16_t height;
        std::string folder;

        //systems
        Recycler recycler;
        EnvBuilder envBuilder;
        Transformer transform;
        Renderer renderer;
        ScriptRunner scriptRunner;

        //resources
        StringPool stringPool;
        AssetPool<Mat4> mat4;
        AssetPool<Mesh> meshes;
        AssetPool<Material> materials;
        AssetLoader<Texture> textures;
        AssetLoader<ScriptFile> scripts;
        MeshLoader_builtin meshLoader_builtin;
        MeshLoader_obj meshLoader_obj;

        explicit App(uint16_t width, uint16_t height, const std::string& folder);

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
