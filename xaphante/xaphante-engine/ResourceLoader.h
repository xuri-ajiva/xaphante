#pragma once
#include "_defines.h"

#include "GameHandler.h"

class ResourceLoader {
	SceneHandler* HANDLER_;
	std::string*  SCENE_LOCATION_;
public:

	static bool LoadObject(std::ifstream* input, SceneCollection* scene_collection);

	static bool DeconstructFile(std::string* object_location, SceneCollection* scene_collection);

	ResourceLoader(SceneHandler* handler, std::string* sceneLocation);

	void LoadCollection(SceneCollection* scene_collection, std::string* sceneCollectionLocation);


	void Load();

};
