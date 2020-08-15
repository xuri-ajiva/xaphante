#pragma once
#include <vector>

#include "ObjectHandler.h"

class SceneHandler {
	std::vector<ObjectHandler*>* objects;

public:
	SceneHandler();

	void Init(const char* sceneLocation) const;

	void Draw() const;
};
