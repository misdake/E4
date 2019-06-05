#pragma once

#include <string>
#include "../systems/render/opengl/AttributeSlot.h"
#include "../systems/render/opengl/UniformSlot.h"
#include "../core/Asset.h"
#include "../systems/render/Texture.h"

namespace E4 {

    class Shader;

    class Material {
    public:
        Shader* shader;
        Texture texture;
    };

    class Mesh {
    public:
        int vertexCount;
        FloatBuffer position;
        FloatBuffer texcoord;
        FloatBuffer color;
        ShaderData offset;

//        std::vector<std::pair<AttributeSlot*, FloatBuffer*>> attributes;
//        std::vector<std::pair<UniformSlot*, ShaderData*>> uniforms;
//
//        void addAttribute(AttributeSlot& slot, FloatBuffer& floatBuffer) {
//            attributes.emplace_back(&slot, &floatBuffer);
//        }
//        void addUniform(UniformSlot& slot, ShaderData& shaderData) {
//            uniforms.emplace_back(&slot, &shaderData);
//        }
    };

    struct Drawable {
    public:
        Asset<Mesh> mesh;
        Asset<Material> material;
    };

}