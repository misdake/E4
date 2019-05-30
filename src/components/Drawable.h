#pragma once

#include <string>
#include "../systems/render/Texture.h"
#include "../systems/render/opengl/AttributeSlot.h"
#include "../systems/render/opengl/UniformSlot.h"
#include "../systems/render/opengl/GlRenderer.h"
#include "../systems/render/opengl/Shader.h"
#include "../core/Asset.h"

namespace E4 {

    class Material {
    public:
        Program* program;
    };

    class Mesh {
    public:
        std::vector<std::pair<AttributeSlot*, FloatBuffer*>> attributes;
        std::vector<std::pair<UniformSlot*, ShaderData*>> uniforms;

        int vertexCount;

        void addAttribute(AttributeSlot& slot, FloatBuffer& floatBuffer) {
            attributes.emplace_back(&slot, &floatBuffer);
        }
        void addUniform(UniformSlot& slot, ShaderData& shaderData) {
            uniforms.emplace_back(&slot, &shaderData);
        }
    };

    struct Drawable {
    public:
        AssetPointer<Mesh> mesh;
        AssetPointer<Material> material;
    };

}