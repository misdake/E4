#include "Attribute.h"

#include <glbinding/gl/gl.h>

using namespace gl;

E4::Attribute::Attribute(E4::DataType dataType, std::string name) :
    dataType(dataType),
    name(std::move(name)) {

}

void E4::Attribute::bind(E4::FloatBuffer& buffer) {
    glEnableVertexAttribArray(attributeIndex);
    buffer.bind(attributeIndex);
}

void E4::Attribute::unbind() {
    glDisableVertexAttribArray(attributeIndex);
}