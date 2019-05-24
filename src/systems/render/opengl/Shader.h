#pragma once

#include <string>
#include <vector>

#include "FloatBuffer.h"
#include "Attribute.h"

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
        std::vector<Attribute> attributes;
        explicit VertexShader(const std::string& content);
    };

    class PixelShader : public Shader {
    public:
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