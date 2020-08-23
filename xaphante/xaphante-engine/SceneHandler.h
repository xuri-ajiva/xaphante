#pragma once   
#include "_defines.h"

#include <vector>

#include "Cameras.h"
#include "GameHandler.h"

#define CAMERA_FAST_SPEED 8.0f
#define CAMERA_DEFAULT_SPEED 2.0f
						   
class GameHandler;

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
