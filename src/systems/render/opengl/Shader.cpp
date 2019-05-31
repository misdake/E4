#include "Shader.h"

#include <iostream>
#include <sstream>

E4::Shader::Shader(std::string mainMethod, GLenum shaderType) :
    mainMethod(std::move(mainMethod)),
    shaderType(shaderType),
    content() {

}

void E4::Shader::compile() {
    shaderId = glCreateShader(shaderType);
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
}

E4::VertexShader::VertexShader(const std::string& content) :
    Shader(content, GL_VERTEX_SHADER) {

}
void E4::VertexShader::compose() {
    std::stringstream stream;

    stream << "#version 330\n\n";

    for (auto& pair : attributes) {
        AttributeSlot* attribute = pair.first;
        const char* name = attribute->name;
        const char* typeName = dataTypeName(attribute->dataType);
        stream << "in " << typeName << " " << name << ";\n";
    }
    for (auto& pair : uniforms) {
        UniformSlot* uniform = pair.first;
        const char* name = uniform->name;
        const char* typeName = dataTypeName(uniform->dataType);
        stream << "uniform " << typeName << " " << name << ";\n";
    }
    stream << "\n" << mainMethod << "\n";

    content = stream.str();
}

E4::PixelShader::PixelShader(const std::string& content) :
    Shader(content, GL_FRAGMENT_SHADER) {

}

void E4::PixelShader::compose() {
    std::stringstream stream;

    stream << "#version 330\n\n";

    for (auto& pair : uniforms) {
        UniformSlot* uniform = pair.first;
        const char* name = uniform->name;
        const char* typeName = dataTypeName(uniform->dataType);
        stream << "uniform " << typeName << " " << name << ";\n";
    }
    stream << "\n" << mainMethod << "\n";

    content = stream.str();
}

E4::Program::Program(const std::string& vs, const std::string& ps) :
    vertexShader(vs),
    pixelShader(ps) {
}

void E4::Program::compile() {
    programId = glCreateProgram();

    vertexShader.compose();
    std::cout << vertexShader.content << std::endl;
    vertexShader.compile();

    pixelShader.compose();
    std::cout << pixelShader.content << std::endl;
    pixelShader.compile();

    glAttachShader(programId, vertexShader.shaderId);
    glAttachShader(programId, pixelShader.shaderId);

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
    }

    glDetachShader(programId, vertexShader.shaderId);
    glDetachShader(programId, pixelShader.shaderId);
    glDeleteShader(vertexShader.shaderId);
    glDeleteShader(pixelShader.shaderId);

    for (auto& attributePair : vertexShader.attributes) {
        AttributeSlot* slot = attributePair.first;
        uint32_t& location = attributePair.second;
        location = glGetAttribLocation(programId, slot->name);
    }
    for (auto& uniformPair : vertexShader.uniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        location = glGetUniformLocation(programId, slot->name);
    }
    for (auto& uniformPair : pixelShader.uniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        location = glGetUniformLocation(programId, slot->name);
    }
}

void E4::Program::use() {
    glUseProgram(programId);
    for (auto& attributePair : vertexShader.attributes) {
        AttributeSlot* slot = attributePair.first;
        uint32_t& location = attributePair.second;
        slot->location = location;
    }
    for (auto& uniformPair : vertexShader.uniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        slot->location = location;
    }
    for (auto& uniformPair : pixelShader.uniforms) {
        UniformSlot* slot = uniformPair.first;
        uint32_t& location = uniformPair.second;
        slot->location = location;
    }
}
