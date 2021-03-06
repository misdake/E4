#include "UniformSlot.h"

#include <glbinding/gl/gl.h>
using namespace gl;

E4::UniformSlot::UniformSlot(const char* name, E4::ShaderDataType dataType) :
    name(name),
    dataType(dataType),
    location(0) {

}

E4::UniformSlot& E4::UniformSlot::operator<<(const E4::ShaderData& shaderData) {
    bind(shaderData);
    return *this;
}

void E4::UniformSlot::bind(const ShaderData& shaderData) {
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
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, shaderData.textureId);
            glUniform1i(location, 0);
            break;
        case ShaderDataType::MAT4:
            glUniformMatrix4fv(location, 1, false, shaderData.mat4->array());
            break;
    }
}
