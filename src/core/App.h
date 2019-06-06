#pragma once

#include "../util/Window.h"
#include "ECS.h"

#include "../systems/render/Renderer.h"
#include "../systems/transform/Transformer.h"

#include "Asset.h"
#include "../components/Drawable.h"

namespace E4 {

    class App {
    public:
        Window window;

        //ecs
        ECS ecs;

        //systems
        Transformer transform;
        Renderer renderer;

        //resources
        AssetPool<Mesh> meshes;
        AssetPool<Material> materials;

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
