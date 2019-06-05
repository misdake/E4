#pragma once

#include "opengl/Shaders.h"
#include "opengl/GlRenderer.h"
#include "../../core/Asset.h"
#include "../../components/Drawable.h"

namespace E4 {

    class Renderer {
    public:
        Renderer() = default;

        ShaderBasic shaderBasic = ShaderBasic(glRenderer);
        ShaderTexture shaderTexture = ShaderTexture(glRenderer);

        GlRenderer glRenderer;

        void init();
        void resize(int w, int h);

        void run(std::vector<Asset<Drawable>>& scene);
    };

}