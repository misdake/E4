#pragma once

#include <string>
#include <vector>

#include "DataType.h"
#include "FloatBuffer.h"
#include "AttributeSlot.h"
#include "UniformSlot.h"

#include <glbinding/gl/gl.h>

using namespace gl;

namespace E4 {

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
        void compile();
        void use();

    private:
        uint32_t programId = 0;
        uint32_t vsId = 0;
        uint32_t psId = 0;
        std::string vsMain = "";
        std::string psMain = "";
        std::string vsContent = "";
        std::string psContent = "";

    };


}