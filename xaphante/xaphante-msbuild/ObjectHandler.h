#pragma once
#include "ShaderHandler.h"
#include "VertexBuffer.h"
#include "_defines.h"

class ObjectHandler {
	Vertex VERTICES_[3];
	UInt32 NUM_VERTICES_;

	VertexBuffer* VERTEX_BUFFER_ = nullptr;

	ShaderHandler* shader = nullptr;

public:
	ObjectHandler();

	void Init(const char* vertexShader, const char* fragmentShader);

	void Draw() const;
};
