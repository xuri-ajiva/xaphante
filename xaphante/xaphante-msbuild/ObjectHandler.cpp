#include "ObjectHandler.h"

ObjectHandler::ObjectHandler() {
	VERTICES_[0] = Vertex {
		-0.5f, -0.5f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f
	};
	VERTICES_[1] = Vertex {
		0.0f, 0.5f, 0.0f,
		0.0, 1.0f, 0.0f, 1.0f
	};
	VERTICES_[2] = Vertex {
		0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};
	NUM_VERTICES_ = 3;
}

void ObjectHandler::Init(const char* vertexShader, const char* fragmentShader) {
	VERTEX_BUFFER_ = new VertexBuffer(VERTICES_, NUM_VERTICES_);
	VertexBuffer::Unbind();

	shader = new ShaderHandler(vertexShader, fragmentShader);
	shader->Bind();
}

void ObjectHandler::Draw() const {
	VERTEX_BUFFER_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_);
	VertexBuffer::Unbind();
}
