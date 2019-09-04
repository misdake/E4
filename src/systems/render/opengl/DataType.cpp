#include "DataType.h"

const char* E4::dataTypeName(E4::ShaderDataType type) {
    switch (type) {
        case E4::ShaderDataType::FLOAT: return "float";
        case E4::ShaderDataType::VEC2: return "vec2";
        case E4::ShaderDataType::VEC3: return "vec3";
        case E4::ShaderDataType::VEC4: return "vec4";
        case E4::ShaderDataType::TEXTURE: return "sampler2D";
        case E4::ShaderDataType::MAT4: return "mat4";
        default: return "";
    }
}