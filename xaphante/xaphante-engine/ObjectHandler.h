#pragma once
#include "IndexBuffer.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "VertexBuffer.h"
#include "WindowWrapper.h"
#include "_defines.h"

class SceneCollection;

class ObjectHandler {
	UInt32 NUM_VERTICES_ = 0;
	UInt32 NUM_INDICES_  = 0;

	VertexBuffer*   VERTEX_BUFFER_   = nullptr;
	IndexBuffer*    INDEX_BUFFER_    = nullptr;
	TextureHandler* TEXTURE_HANDLER_ = nullptr;
	bool isInit;

public:
	ObjectHandler();

	bool DeconstructObjectFile(std::string* object_location, void* vertices, void* indices);

	void Init(std::string* object_location, ShaderHandler* shader);

	void Draw(WindowWrapper* handler) const;

	void CleanUp(SceneCollection* node);

	void GameLoop(SceneCollection* scene_collection);
};
