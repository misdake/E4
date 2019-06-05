#pragma once

#include "Shader.h"
#include "GlRenderer.h"

namespace E4 {

    class Renderer;

    class ShaderBasic : public Shader {
    public:
        explicit ShaderBasic(GlRenderer& renderer);
    };

    class ShaderTexture : public Shader {
    public:
        explicit ShaderTexture(GlRenderer& renderer);
    };

}
