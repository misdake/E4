#pragma once

#include "Shader.h"

namespace E4 {

    class GlRenderer;

    class ShaderBasic : public Shader {
    public:
        explicit ShaderBasic(GlRenderer& renderer);
        void bind(GlRenderer& renderer, const Transform& Transform, const Drawable& drawable, const E4::Environment& environment) override;
    };

    class ShaderTexture : public Shader {
    public:
        explicit ShaderTexture(GlRenderer& renderer);
        void bind(GlRenderer& renderer, const Transform& Transform, const Drawable& drawable, const E4::Environment& environment) override;
    };

}
