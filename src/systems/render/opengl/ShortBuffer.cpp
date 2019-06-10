#include "ShortBuffer.h"

#include <iostream> //TODO
#include <glbinding/gl/gl.h>
using namespace gl;

E4::ShortBuffer::ShortBuffer(std::vector<uint16_t> array, uint32_t countIndices) :
    array(std::move(array)),
    bufferId(0),
    countIndices(countIndices) {
}
E4::ShortBuffer::ShortBuffer() :
    array(),
    bufferId(0),
    countIndices(0) {
}
E4::ShortBuffer& E4::ShortBuffer::set(std::vector<uint16_t> narray, uint32_t ncountIndices) {
    if (bufferId > 0) { //TODO
        std::cout << "abandoning uploaded buffer" << std::endl;
    }
    array = std::move(narray);
    bufferId = 0;
    countIndices = ncountIndices;
    return *this;
}

void E4::ShortBuffer::upload() {
    if (bufferId > 0) { //TODO
        std::cout << "abandoning uploaded buffer" << std::endl;
    }
    glGenVertexArrays(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, countIndices * 2, &this->array[0], GL_STATIC_DRAW);
}

void E4::ShortBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
}