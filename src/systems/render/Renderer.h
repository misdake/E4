#pragma once

#include "opengl/Shaders.h"
#include "opengl/GlRenderer.h"
#include "../../util/FrameState.h"

namespace E4 {

    class EcsCore;

    class Renderer {
    public:
        Renderer() = default;

        ShaderBasic shaderBasic = ShaderBasic(glRenderer);
        ShaderTexture shaderTexture = ShaderTexture(glRenderer);

        GlRenderer glRenderer;

        void init();
        void resize(int w, int h);

        void run(EcsCore& ecs, const E4::FrameState& state);
    };

}