#pragma once	
#include "_defines.h"

#include <vector>

#include "ObjectHandler.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GameHandler.h"

class SceneCollection {
public:

	std::vector<ObjectHandler*>* objects {};
	std::string* SCENE_COLLECTION_LOCATION_;

	void Init(std::string* vertexShader, std::string* fragmentShader);

	void Draw(WindowWrapper* window) const;

	void CleanUp(SceneHandler* scene_handler);

	void GameLoop(SceneHandler* scene_handler);

	SceneCollection(SceneHandler* handler, std::string* sceneCollectionLocation);

private:
	glm::mat4      MODEL {};
	glm::mat4      MODEL_VIEW_PROJE {};
	int            colorUniform {};
	int            modelUniform {};
	ShaderHandler* SHADER_ = nullptr;
	SceneHandler*  SCENE_HANDLER_ {};
};
