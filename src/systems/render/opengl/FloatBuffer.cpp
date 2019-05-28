#include "FloatBuffer.h"

#include <glbinding/gl/gl.h>

using namespace gl;

E4::FloatBuffer::FloatBuffer(std::vector<float> array, uint32_t floatPerVertex, uint32_t countVertex) :
    array(std::move(array)),
    floatPerVertex(floatPerVertex),
    countVertex(countVertex) {
}

void E4::FloatBuffer::upload() {
    glGenVertexArrays(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, countVertex * floatPerVertex * 4, &this->array[0], GL_STATIC_DRAW);
}

void E4::FloatBuffer::bind(uint32_t attributeIndex) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glVertexAttribPointer(
        attributeIndex,     // attribute
        countVertex,        // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        floatPerVertex * 4, // stride
        reinterpret_cast<void*> (0) // array buffer offset
    );
}