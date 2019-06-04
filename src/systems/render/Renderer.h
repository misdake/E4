#pragma once

#include "opengl/UniformSlot.h"
#include "opengl/AttributeSlot.h"
#include "opengl/Shaders.h"
#include "opengl/GlRenderer.h"
#include "../../core/Asset.h"
#include "../../components/Drawable.h"

namespace E4 {

    class Renderer {
    public:
        Renderer() = default;

        UniformSlots uniformSlots;
        AttributeSlots attributeSlots;

        ShaderBasic shaderBasic = ShaderBasic(*this);

        GlRenderer glRenderer;

        void init();
        void resize(int w, int h);

        void run(std::vector<Asset<Drawable>>& scene);
    };

}