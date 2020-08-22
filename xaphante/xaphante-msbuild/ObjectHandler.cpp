#include "ObjectHandler.h"
#include <GL/glew.h>

#include "Scene.h"

ObjectHandler::ObjectHandler() {
	VERTICES_[0] = Vertex {
		-0.5f, -0.5f, 0.0f,
		0, 0,
		1.0f, 0.0f, 0.0f, 1.0f
	};
	VERTICES_[1] = Vertex {
		0.5f, -0.5f, 0.0f,
		1, 0,
		0.0f, 0.0f, 1.0f, 1.0f
	};
	VERTICES_[3] = Vertex {
		0.5f, 0.5f, 0.0f,
		1, 1,
		0.0, 1.0f, 0.0f, 1.0f
	};
	VERTICES_[2] = Vertex {
		-0.5f, 0.5f, 0.0f,
		0, 1,
		0.0f, 0.0f, 1.0f, 1.0f
	};
}

void ObjectHandler::Init(std::string* sceneLocation, ShaderHandler* shader) {
	VERTEX_BUFFER_ = new VertexBuffer(VERTICES_, NUM_VERTICES_);
	VertexBuffer::Unbind();

	INDEX_BUFFER_ = new IndexBuffer(INDICES_, NUM_INDICES_, sizeof(UInt32));
	IndexBuffer::Unbind();

	TEXTURE_HANDLER_ = new TextureHandler(sceneLocation, shader);
	TextureHandler::Unbind();
}

void ObjectHandler::Draw(WindowWrapper* handler) const {
	VERTEX_BUFFER_->Bind();
	INDEX_BUFFER_->Bind();
	TEXTURE_HANDLER_->Bind();

	glDrawElements(GL_TRIANGLES, NUM_INDICES_,GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_);
	//VertexBuffer::Unbind();
	//IndexBuffer::Unbind();
	//TextureHandler::Unbind();
}
