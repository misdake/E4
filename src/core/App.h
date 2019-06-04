#pragma once

#include "../util/Window.h"
#include "../systems/render/Renderer.h"
#include "Asset.h"
#include "../components/Drawable.h"

namespace E4 {

    class App {
    public:
        Window window;

        Renderer renderer;

        std::vector<Asset<Drawable>> scene;

        AssetPool<Material> materials;
        AssetPool<Mesh> meshes;
        AssetPool<Drawable> drawables;

        void load(const std::function<void(App&)>& onLoaded);

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame);

    };

}
