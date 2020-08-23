#pragma once
#include "_defines.h"

#include "WindowWrapper.h"
#include "ObjectHandler.h"
#include <vector>

#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GameHandler.h"

	#define CAMERA_FAST_SPEED 8.0f
	#define CAMERA_DEFAULT_SPEED 2.0f

class SceneHandler;

class SceneCollection {
public:

	std::vector<ObjectHandler*>* objects {};

	void Init(std::string* vertexShader, std::string* fragmentShader);

	void Draw(WindowWrapper* window) const;

	void CleanUp(SceneHandler* scene_handler);

	void GameLoop(SceneHandler* scene_handler);

	SceneCollection(SceneHandler* handler);

private:
	glm::mat4      MODEL;
	glm::mat4      MODEL_VIEW_PROJE;
	int            colorUniform {};
	int            modelUniform {};
	ShaderHandler* SHADER_ = nullptr;
	SceneHandler*  SCENE_HANDLER_ {};
};

class SceneHandler {

public:
	mutable std::string* SCENE_LOCATION_;

	std::vector<SceneCollection*>* sceneCollection;

	GameHandler* GAME_H_;

	FreeCamera* CAMERA;
	float       CAMERA_SPEED_ = CAMERA_DEFAULT_SPEED;

	SceneHandler(GameHandler* game);

	void Init(std::string* sceneLocation);

	void Draw() const;

	void CleanUp();

	void GameLoop(float delta);

	void MouseMove(Sint32 xRel, Sint32 yRel) const;

	bool KeyEvent(SDL_Keycode key, UInt16 mod, bool down);
};

inline SceneCollection::SceneCollection(SceneHandler* handler) {
	SCENE_HANDLER_ = handler;

	objects = new std::vector<ObjectHandler*>();

	MODEL = glm::mat4(1.0f);

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;
}

inline SceneHandler::SceneHandler(GameHandler* game): SCENE_LOCATION_(nullptr) {
	GAME_H_ = game;

	sceneCollection = new std::vector<SceneCollection*>();

	CAMERA = new FreeCamera(glm::radians(45.0f), GAME_H_->WINDOW_W_->Aspect());
}

inline void SceneHandler::Init(std::string* sceneLocation) {
	SCENE_LOCATION_ = sceneLocation;

	CAMERA->Translate({0, 0, 5});

	auto* obj  = new SceneCollection(const_cast<SceneHandler*>(this));
	auto  vert = new std::string(*sceneLocation + "shaders/basic.vert");
	auto  frag = new std::string(*sceneLocation + "shaders/basic.frag");
	obj->Init(vert, frag);
	delete vert;
	delete frag;

	sceneCollection->push_back(obj);
}

inline void SceneCollection::Init(std::string* vertexShader, std::string* fragmentShader) {
	SHADER_ = new ShaderHandler(vertexShader, fragmentShader);
	SHADER_->Bind();

	auto* obj = new ObjectHandler();
	obj->Init(SCENE_HANDLER_->SCENE_LOCATION_, SHADER_);
	objects->push_back(obj);

	modelUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_modelViewProje") GL_ERROR
	colorUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_color") GL_ERROR
}

inline void SceneHandler::Draw() const {
	for (auto* collection : *sceneCollection) {
		collection->Draw(GAME_H_->WINDOW_W_);
	}
}

inline void SceneCollection::Draw(WindowWrapper* window) const {
	SHADER_->Bind();
	glUniform4f(colorUniform,
	            sinf(window->time),
	            cosf(window->time),
	            -sinf(window->time),
	            1.0f)GL_ERROR

	glUniformMatrix4fv(modelUniform, 1, false, &MODEL_VIEW_PROJE[0][0])GL_ERROR

	for (auto collection : *objects) {
		collection->Draw(window);
	}
	SHADER_->Unbind();
}

inline void SceneCollection::CleanUp(SceneHandler* scene_handler) {
	for (auto collection : *objects) {
		collection->CleanUp(this);
		delete collection;
	}
	objects->clear();
	delete objects;
	delete SHADER_;
}

inline void SceneHandler::CleanUp() {
	for (auto* collection : *sceneCollection) {
		collection->CleanUp(this);
		delete collection;
	}
	sceneCollection->clear();
	delete sceneCollection;
}

inline bool dw  = false;
inline bool da  = false;
inline bool ds  = false;
inline bool dd  = false;
inline bool spd = false;

inline void SceneHandler::GameLoop(float delta) {
	auto speed = CAMERA_SPEED_ + (spd ? CAMERA_FAST_SPEED : CAMERA_DEFAULT_SPEED);

	if (dw) CAMERA->MoveForward(delta * speed);
	if (ds) CAMERA->MoveForward(delta * -speed);
	if (da) CAMERA->MoveSideways(delta * -speed);
	if (dd) CAMERA->MoveSideways(delta * speed);

	CAMERA->Update();

	for (auto* collection : *sceneCollection) {
		collection->GameLoop(this);
	}
}

inline void SceneCollection::GameLoop(SceneHandler* scene_handler) {
	for (auto collection : *objects) {
		collection->GameLoop(this);
	}

	//MODEL = rotate(MODEL, 1.0f * scene_handler->WINDOW_W_->delta, glm::vec3(0, 1, 0));
	//MODEL = glm::translate(MODEL, glm::vec3(0, 0, sinf(scene_handler->WINDOW_W_->time) * scene_handler->WINDOW_W_->delta));

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;

	//std::cout << "Time: " << scene_handler->WINDOW_W_->time << std::endl;
}

inline void SceneHandler::MouseMove(Sint32 xRel, Sint32 yRel) const {
	CAMERA->OnMouseMoved(xRel, yRel);
}

inline bool SceneHandler::KeyEvent(SDL_Keycode key, UInt16 mod, bool down) {
	switch (key) {
		case SDLK_w:
			dw = down;
			break;
		case SDLK_a:
			da = down;
			break;
		case SDLK_s:
			ds = down;
			break;
		case SDLK_d:
			dd = down;
			break;

		case SDLK_ESCAPE:
			GAME_H_->RUNNING = false;
			return false;
		case SDLK_KP_PLUS:
			CAMERA_SPEED_ += CAMERA_DEFAULT_SPEED;
			break;
		case SDLK_KP_MINUS:
			CAMERA_SPEED_ -= CAMERA_DEFAULT_SPEED;
			break;
		case SDLK_LSHIFT:
			spd = down;
			break;

		default:
			break;
	}
	return true;
}
