#include "SceneHandler.h"
#include <string>
#include <filesystem>
#include <vector>

SceneHandler::SceneHandler(GameHandler* game): SCENE_LOCATION_(nullptr) {
	GAME_H_ = game;

	sceneCollection = new std::vector<SceneCollection*>();

	CAMERA = new FreeCamera(glm::radians(45.0f), GAME_H_->WINDOW_W_->Aspect());
}

void SceneHandler::Init(std::string* sceneLocation) {
	SCENE_LOCATION_ = sceneLocation;

	CAMERA->Translate({0, 0, 5});

	auto vert = *sceneLocation + "shaders/basic.vert";
	auto frag = *sceneLocation + "shaders/basic.frag";

	const auto objectsLocation = *sceneLocation + "objects/";

	for (const auto& entry : std::filesystem::directory_iterator(objectsLocation)) {
		auto location = entry.path().string();

		const size_t last_slash_idx = location.find_last_of("\\/");
		if (std::string::npos != last_slash_idx) {
			if(location[last_slash_idx + 1] == '_') {
				continue;
			}
		}

		std::cout << "[LOADING]: " << location << std::endl;

		auto* obj = new SceneCollection(this, &location);

		obj->Init(&vert, &frag);

		sceneCollection->push_back(obj);
	}
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

bool dw  = false;
bool da  = false;
bool ds  = false;
bool dd  = false;
bool spd = false;

void SceneHandler::GameLoop(float delta) {
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

void SceneHandler::MouseMove(Sint32 xRel, Sint32 yRel) const {
	CAMERA->OnMouseMoved(xRel, yRel);
}

bool SceneHandler::KeyEvent(SDL_Keycode key, UInt16 mod, bool down) {
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
