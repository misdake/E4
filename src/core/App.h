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
        EcsCore ecs;

        //systems
        Transformer transform;
        Renderer renderer;
        ScriptRunner scriptRunner;

        //resources
        AssetPool<Mesh> meshes;
        AssetPool<Material> materials;
        AssetLoader<Texture> textures;
        AssetLoader<ScriptFile> scripts;
        BuiltinMeshLoader meshLoader;

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
