#include "VertexBuffer.h"	  

#include <cstddef>
#include <GL/glew.h>

VertexBuffer::VertexBuffer(void* data, UInt32 num_vertices) {
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &BUFFER_ID_);
	glBindBuffer(GL_ARRAY_BUFFER, BUFFER_ID_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, r));

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
