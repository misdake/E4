#pragma once

#include <string>
#include <vector>

#include "FloatBuffer.h"
#include "Uniform.h"
#include "AttributeSlot.h"

#include <glbinding/gl/gl.h>

using namespace gl;

namespace E4 {

    class Shader {
    public:
        uint32_t shaderId = 0;
        std::string content;
        GLenum shaderType;

        Shader(std::string content, GLenum shaderType);
        void compile();
    };

    class VertexShader : public Shader {
    public:
        std::vector<std::pair<AttributeSlot*, uint32_t>> attributes;
        std::vector<Uniform> uniforms;
        explicit VertexShader(const std::string& content);
        void addAttribute(AttributeSlot& slot) {
            attributes.emplace_back(&slot, 0);
        }
    };

    class PixelShader : public Shader {
    public:
        std::vector<Uniform> uniforms;
        explicit PixelShader(const std::string& content);
    };

    class Program {
    public:
        uint32_t programId = 0;
        VertexShader vertexShader;
        PixelShader pixelShader;

        Program(const std::string& vs, const std::string& ps);
        void compile();
        void use();
    };


}