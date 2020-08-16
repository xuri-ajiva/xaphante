#pragma once
#include "IndexBuffer.h"
#include "ShaderHandler.h"
#include "VertexBuffer.h"
#include "_defines.h"

class ObjectHandler {
	Vertex VERTICES_[4];
	UInt32 NUM_VERTICES_ = 4; 
	UInt32 INDICES_[6] = {
		0,1,2,
		1,2,3
	};
	UInt32 NUM_INDICES_ = 6;

	VertexBuffer* VERTEX_BUFFER_ = nullptr;
	IndexBuffer* INDEX_BUFFER_ = nullptr;

	ShaderHandler* shader = nullptr;

public:
	ObjectHandler();

	void Init(const char* vertexShader, const char* fragmentShader);

	void Draw() const;
};
