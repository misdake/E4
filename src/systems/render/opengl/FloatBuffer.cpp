#include "FloatBuffer.h"

#include <glbinding/gl/gl.h>
using namespace gl;

#include "../../../util/Log.h"

E4::FloatBuffer::FloatBuffer(std::vector<float> array, uint32_t floatPerVertex, uint32_t countVertex) :
    array(std::move(array)),
    bufferId(0),
    floatPerVertex(floatPerVertex),
    countVertex(countVertex) {
}
E4::FloatBuffer::FloatBuffer() :
    array(),
    bufferId(0),
    floatPerVertex(0),
    countVertex(0) {
}
E4::FloatBuffer& E4::FloatBuffer::set(std::vector<float> narray, uint32_t nfloatPerVertex, uint32_t ncountVertex) {
    if (bufferId > 0) {
        Log::error("abandoning uploaded buffer");
    }
    array = std::move(narray);
    bufferId = 0;
    floatPerVertex = nfloatPerVertex;
    countVertex = ncountVertex;
    return *this;
}

void E4::FloatBuffer::upload() {
    if (bufferId > 0) {
        Log::error("abandoning uploaded buffer");
    }
    glGenVertexArrays(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, countVertex * floatPerVertex * 4, &this->array[0], GL_STATIC_DRAW);
}

void E4::FloatBuffer::bind(uint32_t attributeIndex) const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glVertexAttribPointer(
        attributeIndex,     // attribute
        floatPerVertex,     // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        floatPerVertex * 4, // stride
        nullptr // array buffer offset
    );
}