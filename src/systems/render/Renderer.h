#pragma once

#include "../../core/ECS.h"
#include "opengl/Shaders.h"
#include "opengl/GlRenderer.h"
#include "../../util/FrameState.h"

namespace E4 {

    class App;

    class Renderer {
    public:
        Renderer() = default;

        ShaderBasic shaderBasic = ShaderBasic(glRenderer);
        ShaderTexture shaderTexture = ShaderTexture(glRenderer);
        ShaderLight shaderLight = ShaderLight(glRenderer);

        GlRenderer glRenderer;

        void init();
        void resize(int w, int h);

        void run(Ecs& ecs, const E4::FrameState& state, const E4::Environment& environment);
    };

}