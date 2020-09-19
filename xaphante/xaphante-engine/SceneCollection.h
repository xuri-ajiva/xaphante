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

	void Init();

	void Draw(WindowWrapper* window);

	void CleanUp(SceneHandler* scene_handler);

	void GameLoop(float delta);

	SceneCollection(SceneHandler* handler, ShaderHandler* shader);

private:
	glm::mat4 MODEL {};
	glm::mat4 MODEL_VIEW_PROJE {};

	int modelUniform {};
	int modelViewUniform {};
	int invModelViewUniform {};

	ShaderHandler* SHADER_ = nullptr;
	SceneHandler*  SCENE_HANDLER_ {};
};
