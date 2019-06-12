#pragma once

#include "../util/Window.h"
#include "ECS.h"

#include "../systems/render/Renderer.h"
#include "../systems/transform/Transformer.h"
#include "../systems/script/ScriptRunner.h"

#include "Asset.h"
#include "../components/Drawable.h"
#include "../systems/script/ScriptFile.h"

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

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
