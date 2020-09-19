#include "SceneHandler.h"
#include <string>
#include <filesystem>
#include <vector>

#include "ResourceLoader.h"

SceneHandler::SceneHandler(GameHandler* game, std::string* sceneLocation):
	resource(new ResourceLoader(this, sceneLocation)), SCENE_LOCATION_(sceneLocation), sceneCollection(new std::vector<SceneCollection*>),
	shaderCollection(new std::vector<ShaderHandler*>), textureCollection(new std::vector<TextureHandler*>), GAME_H_(game) {

	CAMERA = new FreeCamera(glm::radians(45.0f), GAME_H_->WINDOW_W_->Aspect());
}

void SceneHandler::Init() const {
	CAMERA->Translate({0, 0, 5});
	resource->Load();
}

void SceneHandler::Draw() const {
	for (auto* collection : *sceneCollection) {
		collection->Draw(GAME_H_->WINDOW_W_);
	}
}

void SceneHandler::CleanUp() {
	for (auto* collection : *sceneCollection) {
		collection->CleanUp(this);
		delete collection;
	}
	sceneCollection->clear();
	delete sceneCollection;
}

bool spd = false;

void SceneHandler::GameLoop(float delta) const {
	const auto speed = CAMERA_SPEED_ + (spd ? CAMERA_FAST_SPEED : CAMERA_DEFAULT_SPEED);

	auto set = GAME_H_->WINDOW_W_->KEY_DOWN_SET_;
	for (auto itr = set.begin(); itr != set.end(); ++itr) {
		const auto key = *itr;

		switch (key) {
			case SDLK_w:
				CAMERA->MoveForward(delta * speed);
				break;
			case SDLK_a:
				CAMERA->MoveSideways(delta * -speed);
				break;
			case SDLK_s:
				CAMERA->MoveForward(delta * -speed);
				break;
			case SDLK_d:
				CAMERA->MoveSideways(delta * speed);
				break;

			default:
				break;
		}
	}

	CAMERA->Update();

	for (auto* collection : *sceneCollection) {
		collection->GameLoop(delta);
	}
}

void SceneHandler::MouseMove(Sint32 xRel, Sint32 yRel) const {
	CAMERA->OnMouseMoved(xRel, yRel);
}

bool SceneHandler::KeyEvent(SDL_Keycode key, UInt16 mod, bool down) {
	switch (key) {
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
