#include "SceneHandler.h"

SceneHandler::SceneHandler() {
	objects = new std::vector<ObjectHandler*>();
}

void SceneHandler::Init(const char* sceneLocation) const {
	auto* obj = new ObjectHandler();
	obj->Init("shaders/basic.vert", "shaders/basic.frag");
	objects->push_back(obj);
}

void SceneHandler::Draw() const {
	for (auto object : *objects) {
		object->Draw();
	}
}
