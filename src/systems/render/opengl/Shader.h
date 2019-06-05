#pragma once

#include <string>
#include <vector>
#include <glbinding/gl/gl.h>
using namespace gl;

#include "DataType.h"
#include "FloatBuffer.h"

namespace E4 {
    class AttributeSlot;
    class UniformSlot;
    class GlRenderer;
    class Drawable;

    class Shader {
    public:
        std::vector<std::pair<AttributeSlot*, uint32_t>> vertexAttributes;
        std::vector<std::pair<UniformSlot*, uint32_t>> vertexUniforms;
        std::vector<std::pair<std::string, ShaderDataType>> varyings;
        std::vector<std::pair<UniformSlot*, uint32_t>> pixelUniforms;

        void addVertexAttribute(AttributeSlot& slot) {
            vertexAttributes.emplace_back(&slot, 0);
        }
        void addVertexUniform(UniformSlot& slot) {
            vertexUniforms.emplace_back(&slot, 0);
        }
        void addVarying(const std::string& name, ShaderDataType type) {
            varyings.emplace_back(name, type);
        }
        void addPixelUniform(UniformSlot& slot) {
            pixelUniforms.emplace_back(&slot, 0);
        }

        Shader(std::string vsMain, std::string psMain);
        void use();
        virtual void bind(GlRenderer& renderer, const Drawable& drawable) = 0;

    private:
        void compile();

        bool compiled = false;
        uint32_t programId = 0;
        std::string vsMain = "";
        std::string psMain = "";
        std::string vsContent = "";
        std::string psContent = "";
    };
}