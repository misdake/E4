#include "Shader.h"

#include <iostream>
#include <sstream>
#include <glbinding/gl/gl.h>
using namespace gl;

#include "AttributeSlot.h"
#include "UniformSlot.h"

uint32_t compileShader(GLenum shaderType, const std::string& content) {
    uint32_t shaderId = glCreateShader(shaderType);
    const char* strFileData = content.c_str();
    glShaderSource(shaderId, 1, &strFileData, nullptr);

    glCompileShader(shaderId);

    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == int(GL_FALSE)) {
        GLint infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

        auto* strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, strInfoLog);

        const char* strShaderType = nullptr;
        switch (shaderType) {
            case GL_VERTEX_SHADER:
                strShaderType = "vertex";
                break;
            case GL_GEOMETRY_SHADER:
                strShaderType = "geometry";
                break;
            case GL_FRAGMENT_SHADER:
                strShaderType = "fragment";
                break;
            default:
                break;
        }

        printf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }
    return shaderId;
}

std::string composeVs(
    const std::string& vsMain,
    const std::vector<std::pair<E4::AttributeSlot*, uint32_t>>& attributes,
    const std::vector<std::pair<E4::UniformSlot*, uint32_t>>& uniforms,
    const std::vector<std::pair<std::string, E4::ShaderDataType>>& varyings
) {
    std::stringstream stream;

    stream << "#version 330\n\n";

    for (auto& pair : uniforms) {
        E4::UniformSlot* uniform = pair.first;
        const char* name = uniform->name;
        const char* typeName = dataTypeName(uniform->dataType);
        stream << "uniform " << typeName << " " << name << ";\n";
    }
    for (auto& pair : attributes) {
        E4::AttributeSlot* attribute = pair.first;
        const char* name = attribute->name;
        const char* typeName = dataTypeName(attribute->dataType);
        stream << "in " << typeName << " " << name << ";\n";
    }
    for (auto& pair : varyings) {
        const std::string& name = pair.first;
        const char* typeName = dataTypeName(pair.second);
        stream << "out " << typeName << " " << name << ";\n";
    }
    stream << "\n" << vsMain << "\n";

    return stream.str();
}

std::string composePs(
    const std::string& psMain,
    const std::vector<std::pair<std::string, E4::ShaderDataType>>& varyings,
    const std::vector<std::pair<E4::UniformSlot*, uint32_t>>& uniforms
) {
    std::stringstream stream;

    stream << "#version 330\n\n";

    for (auto& pair : uniforms) {
        E4::UniformSlot* uniform = pair.first;
        const char* name = uniform->name;
        const char* typeName = dataTypeName(uniform->dataType);
        stream << "uniform " << typeName << " " << name << ";\n";
    }
    for (auto& pair : varyings) {
        const std::string& name = pair.first;
        const char* typeName = dataTypeName(pair.second);
        stream << "in " << typeName << " " << name << ";\n";
    }
    stream << "\n" << psMain << "\n";

    return stream.str();
}

E4::Shader::Shader(std::string vsMain, std::string psMain) :
    vsMain(std::move(vsMain)),
    psMain(std::move(psMain)) {
}

void E4::Shader::compile() {
    programId = glCreateProgram();

    vsContent = composeVs(vsMain, vertexAttributes, vertexUniforms, varyings);
//    std::cout << vsContent << std::endl;
    uint32_t vsId = compileShader(GL_VERTEX_SHADER, vsContent);

    psContent = composePs(psMain, varyings, pixelUniforms);
//    std::cout << psContent << std::endl;
    uint32_t psId = compileShader(GL_FRAGMENT_SHADER, psContent);

    glAttachShader(programId, vsId);
    glAttachShader(programId, psId);

    glLinkProgram(programId);

    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == int(GL_FALSE)) {
        GLint infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

        auto* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(programId, infoLogLength, nullptr, strInfoLog);
        printf("Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        return;
    }

    glDetachShader(programId, vsId);
    glDetachShader(programId, psId);
    glDeleteShader(vsId);
    glDeleteShader(psId);

    for (auto& attributePair : vertexAttributes) {
        AttributeSlot* slot = attributePair.first;
        uint32_t& location = attributePair.second;
        location = glGetAttribLocation(programId, slot->name);
    }
    for (auto& uniformPair : vertexUniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        location = glGetUniformLocation(programId, slot->name);
    }
    for (auto& uniformPair : pixelUniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        location = glGetUniformLocation(programId, slot->name);
    }
}

void E4::Shader::use() {
    if (!compiled) {
        compile();
        compiled = true;
    }

    glUseProgram(programId);
    for (auto& attributePair : vertexAttributes) {
        AttributeSlot* slot = attributePair.first;
        uint32_t& location = attributePair.second;
        slot->location = location;
    }
    for (auto& uniformPair : vertexUniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        slot->location = location;
    }
    for (auto& uniformPair : pixelUniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        slot->location = location;
    }
}
