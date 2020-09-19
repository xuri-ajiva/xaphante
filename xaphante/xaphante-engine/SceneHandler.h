#pragma once
#include "_defines.h"

#include <vector>

#include "Cameras.h"
#include "GameHandler.h"
#include "ResourceLoader.h"
#include "SceneHandler.h"
#include "SceneCollection.h"

#define CAMERA_FAST_SPEED 8.0f
#define CAMERA_DEFAULT_SPEED 2.0f

class SceneHandler {

public:
	mutable std::string* SCENE_LOCATION_;

	std::vector<SceneCollection*>* sceneCollection;
	std::vector<ShaderHandler*>*   shaderCollection;
	std::vector<TextureHandler*>*  textureCollection;

	GameHandler*    GAME_H_;
	ResourceLoader* resource;

	FreeCamera* CAMERA;
	float       CAMERA_SPEED_ = CAMERA_DEFAULT_SPEED;

	SceneHandler(GameHandler* game, std::string* sceneLocation);

	void Init() const;

	void Draw() const;

	void CleanUp();

	void GameLoop(float delta) const;

	void MouseMove(Sint32 xRel, Sint32 yRel) const;

	bool KeyEvent(SDL_Keycode key, UInt16 mod, bool down);
};
