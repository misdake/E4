#include "Uniform.h"

#include <glbinding/gl/gl.h>

using namespace gl;

E4::Uniform::Uniform(E4::DataType dataType, std::string name) :
    dataType(dataType),
    name(std::move(name)) {

}

void E4::Uniform::bind(float x, float y, float z) {
    glUniform3f(location, x,y,z);
}