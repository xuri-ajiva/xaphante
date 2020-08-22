#pragma once
#include "_defines.h"

#include "WindowWrapper.h"
#include "ObjectHandler.h"
#include <vector>

class SceneHandler;

class SceneCollection {
public:

	std::vector<ObjectHandler*>* objects {};

	void Init(std::string* vertexShader, std::string* fragmentShader);

	void Draw(SceneHandler* handler) const;

	SceneCollection(SceneHandler* handler);

private:
	int            colorUniform {};
	ShaderHandler* SHADER_ = nullptr;
	SceneHandler*  SCENE_HANDLER_ {};
};

class SceneHandler {

public:
	mutable std::string* SCENE_LOCATION_;

	std::vector<SceneCollection*>* sceneCollection;
	WindowWrapper*                 WINDOW_W_;

	SceneHandler(WindowWrapper* window);

	void Init(std::string* sceneLocation) const;

	void Draw(WindowWrapper* handler);
};

inline SceneCollection::SceneCollection(SceneHandler* handler) {
	SCENE_HANDLER_ = handler;

	objects = new std::vector<ObjectHandler*>();
}

inline SceneHandler::SceneHandler(WindowWrapper* window) {
	WINDOW_W_ = window;

	sceneCollection = new std::vector<SceneCollection*>();
}

inline void SceneHandler::Init(std::string* sceneLocation) const {
	SCENE_LOCATION_ = sceneLocation;
	auto* obj       = new SceneCollection(const_cast<SceneHandler*>(this));
	
	auto  vert      = new std::string(*sceneLocation + "shaders/basic.vert");
	auto  frag      = new std::string(*sceneLocation + "shaders/basic.frag");
	obj->Init(vert, frag);
	//free(vert);
	//free(frag);
	
	
	sceneCollection->push_back(obj);
}

inline void SceneCollection::Init(std::string* vertexShader, std::string* fragmentShader) {
	SHADER_ = new ShaderHandler(vertexShader, fragmentShader);
	SHADER_->Bind();

	auto* obj = new ObjectHandler();
	obj->Init(SCENE_HANDLER_->SCENE_LOCATION_, SHADER_);
	objects->push_back(obj);

	colorUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_color") GL_ERROR
}

inline void SceneHandler::Draw(WindowWrapper* handler) {
	for (auto* collection : *sceneCollection) {
		collection->Draw(this);
	}
}

inline void SceneCollection::Draw(SceneHandler* handler) const {
	SHADER_->Bind();
	glUniform4f(colorUniform, sinf(handler->WINDOW_W_->time), cosf(handler->WINDOW_W_->time), -sinf(handler->WINDOW_W_->time), 1.0f)GL_ERROR
	std::cout<< "Time: " << handler->WINDOW_W_->time << std::endl;

	for (auto collection : *objects) {
		collection->Draw(handler->WINDOW_W_);
	}
	SHADER_->Unbind();
}
