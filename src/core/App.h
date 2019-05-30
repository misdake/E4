#pragma once

#include "../systems/render/Renderer.h"
#include "../systems/render/opengl/GlRenderer.h"
#include "Asset.h"
#include "../util/Window.h"

namespace E4 {

    class App {
    public:
        Window window;

        Renderer renderer;
        GlRenderer glRenderer;

        std::vector<AssetPointer<Drawable>> scene;

        AssetPool<Material> materials;
        AssetPool<Mesh> meshes;
        AssetPool<Drawable> drawables;

        void load(const std::function<void(App&)>& onLoaded) {
            int width = 640;
            int height = 480;
            window.create(
                {
                    .title = std::string("abc"),
                    .width = width,
                    .height = height,
                    .borderless = false,
                    .mouseTrap = false,
                }
            );
            glRenderer.init();
            glRenderer.resize(width, height);

            onLoaded(*this);
        }

        void enterLoop(const std::function<void(App&, const FrameState&)>& onFrame) {
            window.enterEventLoop([&onFrame, this](const FrameState& frameState) -> void {
                onFrame(*this, frameState);

                glRenderer.clear();

                for(auto& drawable: scene) {
                    glRenderer.draw(drawable.get());
                }
            });
        }

    };

}
