#include "ObjectHandler.h"

#include <GL/glew.h>
#include <fstream>
#include <vector>

ObjectHandler::ObjectHandler() {}


void ObjectHandler::Init(void* vertices, void* indices, ShaderHandler* shader) {
	VERTEX_BUFFER_ = new VertexBuffer(vertices, NUM_VERTICES_);
	VertexBuffer::Unbind();

	INDEX_BUFFER_ = new IndexBuffer(indices, NUM_INDICES_, sizeof(UInt32));
	IndexBuffer::Unbind();

	//TEXTURE_HANDLER_ = new TextureHandler(object_location, shader);
	//TextureHandler::Unbind();

}

void ObjectHandler::CleanUp(SceneCollection* node) const {
	if (!isInit) return;
	delete VERTEX_BUFFER_;
	delete INDEX_BUFFER_;
	//delete TEXTURE_HANDLER_;
}

void ObjectHandler::Draw(WindowWrapper* handler) const {
	if (!isInit) return;
	VERTEX_BUFFER_->Bind();
	INDEX_BUFFER_->Bind();
	//TEXTURE_HANDLER_->Bind();

	glDrawElements(GL_TRIANGLES, NUM_INDICES_,GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_);
	//VertexBuffer::Unbind();
	//IndexBuffer::Unbind();
	//TextureHandler::Unbind();
}

void ObjectHandler::GameLoop(float delta) {
	if (!isInit) return;
	
}
