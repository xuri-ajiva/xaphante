#pragma once
#include "IndexBuffer.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "VertexBuffer.h"
#include "WindowWrapper.h"
#include "_defines.h"
#include "glm/glm.hpp"

class SceneCollection;

class ObjectHandler {
	Vertex VERTICES_[4];
	UInt32 NUM_VERTICES_ = 4;
	UInt32 INDICES_[6]   = {
		0, 1, 2,
		1, 2, 3
	};
	UInt32 NUM_INDICES_ = 6;

	VertexBuffer*   VERTEX_BUFFER_   = nullptr;
	IndexBuffer*    INDEX_BUFFER_    = nullptr;
	TextureHandler* TEXTURE_HANDLER_ = nullptr;

public:					  	
	ObjectHandler();

	void Init(std::string* sceneLocation, ShaderHandler* shader);

	void Draw(WindowWrapper* handler) const;

	void CleanUp(SceneCollection* node);

	void GameLoop(SceneCollection* scene_collection);
};
