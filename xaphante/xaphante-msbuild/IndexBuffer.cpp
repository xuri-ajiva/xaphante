#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* data, UInt32 numIndices, UInt8 elementSize) {
glGenBuffers(1, &BUFFER_ID_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BUFFER_ID_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &BUFFER_ID_);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BUFFER_ID_);
}

void IndexBuffer::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
