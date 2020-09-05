#include "VertexBuffer.h"

#include <cstddef>
#include <GL/glew.h>

VertexBuffer::VertexBuffer(void* data, UInt32 num_vertices) {
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &BUFFER_ID_)GL_ERROR
	glBindBuffer(GL_ARRAY_BUFFER, BUFFER_ID_)GL_ERROR
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), data, GL_STATIC_DRAW) GL_ERROR

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(struct Vertex, pos)))GL_ERROR
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(struct Vertex, normal)))GL_ERROR

	glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &BUFFER_ID_);
}

void VertexBuffer::Bind() const {
	glBindVertexArray(VAO_);
}

void VertexBuffer::Unbind() {
	glBindVertexArray(0);
}
