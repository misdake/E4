#include "AttributeSlot.h"

#include <glbinding/gl/gl.h>

using namespace gl;

E4::AttributeSlot::AttributeSlot(const char* name, E4::ShaderDataType dataType) :
    name(name),
    dataType(dataType){

}

void E4::AttributeSlot::bind(E4::FloatBuffer& buffer) const {
    glEnableVertexAttribArray(location);
    buffer.bind(location);
}

void E4::AttributeSlot::unbind() const {
    glDisableVertexAttribArray(location);
}