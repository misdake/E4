#include <utility>

#include <utility>

#include "Shader.h"

E4::Shader::Shader(std::string content, GLenum shaderType) :
    content(std::move(content)),
    shaderType(shaderType) {

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

E4::PixelShader::PixelShader(const std::string& content) :
    Shader(content, GL_FRAGMENT_SHADER) {

}

E4::Program::Program(const std::string& vs, const std::string& ps) :
    vertexShader(vs),
    pixelShader(ps) {
    vertexShader.compile();
    pixelShader.compile();
}

void E4::Program::compile() {
    programId = glCreateProgram();

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
}

void E4::Program::use() {
    glUseProgram(programId);
}
