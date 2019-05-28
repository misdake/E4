#include "Uniform.h"

#include <glbinding/gl/gl.h>

using namespace gl;

E4::Uniform::Uniform(E4::ShaderDataType dataType, std::string name) :
    dataType(dataType),
    name(std::move(name)) {

}

void E4::Uniform::bind(ShaderData shaderData) {
    switch (dataType) {
        case ShaderDataType::FLOAT:
            glUniform1f(location, shaderData.numbers.x);
            break;
        case ShaderDataType::VEC2:
            glUniform2f(location, shaderData.numbers.x, shaderData.numbers.y);
            break;
        case ShaderDataType::VEC3:
            glUniform3f(location, shaderData.numbers.x, shaderData.numbers.y, shaderData.numbers.z);
            break;
        case ShaderDataType::VEC4:
            glUniform4f(location, shaderData.numbers.x, shaderData.numbers.y, shaderData.numbers.z, shaderData.numbers.w);
            break;
        case ShaderDataType::TEXTURE:
            break;
        case ShaderDataType::MATRIX:
            break;
    }
}