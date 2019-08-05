#include "AttributeSlot.h"

#include <glbinding/gl/gl.h>
using namespace gl;

#include "FloatBuffer.h"

E4::AttributeSlot::AttributeSlot(const char* name, E4::ShaderDataType dataType) :
    name(name),
    dataType(dataType),
    location(0) {

}

E4::AttributeSlot& E4::AttributeSlot::operator<<(const E4::FloatBuffer& buffer) {
    bind(buffer);
    return *this;
}

void E4::AttributeSlot::bind(const E4::FloatBuffer& buffer) {
    glEnableVertexAttribArray(location);
    buffer.bind(location);
}

void E4::AttributeSlot::unbind() const {
    glDisableVertexAttribArray(location);
}
