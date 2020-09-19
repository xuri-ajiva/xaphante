#include "ResourceLoader.h"

#include <filesystem>
#include <fstream>

ShaderHandler* shaderTmp;

inline std::string ReadLengthOfString(std::ifstream* s, int l) {
	std::string str;
	str.resize(l);

	char tmp;

	for (int i = 0; i < l; ++i) {
		s->read(&tmp, 8);
		str.append(&tmp);
	}
	return str.c_str();
}

bool ResourceLoader::LoadObject(std::ifstream* input, SceneCollection* scene_collection) {
	auto* vertices = new std::vector<Vertex>();
	auto* indices  = new std::vector<UInt32>();

	auto obj = new ObjectHandler();

	char nl[2];
	nl[1] = '\0';

	input->read(nl, 1);

	std::vector<char> n;
	const auto        length = static_cast<int>(nl[0]);
	n.resize(length + 1);

	input->read(&n[0], length);
	n[length] = '\0';
	std::cout << "[NAME]: " << n.data() << std::endl;

	int size;
	input->read(reinterpret_cast<char*>(&size), sizeof(int)); //todo check

	input->read(reinterpret_cast<char*>(&obj->NUM_VERTICES_), size);

	for (unsigned int i = 0; i < obj->NUM_VERTICES_; ++i) {
		Vertex v {};
		input->read(reinterpret_cast<char*>(&v.pos), sizeof(float) * 3);
		input->read(reinterpret_cast<char*>(&v.normal), sizeof(float) * 3);
		//v.x += NUM_VERTICES_ / 100;

		//auto st = i % 6;
		//switch (st) {case 0:v.r = 1.0f;v.g = 0.0f;v.b = 0.0f;break;case 1:v.r = 0.0f;v.g = 1.0f;v.b = 0.0f;break;case 2:v.r = 0.0f;v.g = 0.0f;v.b = 1.0f;case 3:v.r = 0.0f;v.g = 1.0f;v.b = 1.0f;case 4:v.r = 1.0f;v.g = 1.0f;v.b = 0.0f;case 5:v.r = 1.0f;v.g = 0.0f;v.b = 1.0f;break;}
		//v.a = 1.0f;

		//std::cout << "{" << v.position.x << ", " << v.position.y << ", " << v.position.z << "}" << std::endl;
		vertices->push_back(v);
	}
		
	input->read(reinterpret_cast<char*>(&obj->NUM_INDICES_), size);

	for (int i = 0; i < obj->NUM_INDICES_; ++i) {
		UInt32 c;
		input->read(reinterpret_cast<char*>(&c), sizeof(UInt32));
		indices->push_back(c);
		//std::cout << c << std::endl;
	}	

	obj->Init(vertices->data(), indices->data(), shaderTmp);
	obj->isInit = true;
	scene_collection->objects->push_back(obj);
	return true;
}

bool ResourceLoader::DeconstructFile(std::string* object_location, SceneCollection* scene_collection) {
	auto input = std::ifstream(*object_location, std::ios::in | std::ios::binary);

	if (!input.is_open()) {
		std::cout << "[ERROR]: Reading File: " << object_location << std::endl;
		return false;
	}

	char l[6];				 //header
	l[5] = '\0';
	input.read(l, 5);

	if (std::strncmp(&l[0], "\4BOBF", 5) != 0) {
		std::cout << "[ERROR]: WRONG FILE FORMAT" << std::endl;
		return false;
	} else {
		return LoadObject(&input, scene_collection);
	}

	return true;
}

ResourceLoader::ResourceLoader(SceneHandler* handler, std::string* sceneLocation):
	HANDLER_(handler), SCENE_LOCATION_(sceneLocation) { }

void ResourceLoader::LoadCollection(SceneCollection* scene_collection, std::string* sceneCollectionLocation) {
	for (const auto& entry : std::filesystem::directory_iterator(*sceneCollectionLocation)) {
		auto location = entry.path().string();
		location      = location.replace(location.find('\\'), 1, "/");

		const auto last_slash_idx = location.find_last_of("\\/");
		if (std::string::npos != last_slash_idx) {
			if (location[last_slash_idx + 1] == '_') {
				continue;
			}
		}

		std::cout << "[LOADING]: " << location << std::endl;
		DeconstructFile(&location, scene_collection);
	}
}

void ResourceLoader::Load() {

	//load shaders
	auto vert = *SCENE_LOCATION_ + "shaders/basic.vert";
	auto frag = *SCENE_LOCATION_ + "shaders/basic.frag";

	shaderTmp = new ShaderHandler(&vert, &frag);
	shaderTmp->Bind();
	HANDLER_->shaderCollection->push_back(shaderTmp);

	//load textures
	//
	//load materials
	//
	//load objects

	const auto objectsLocation = *SCENE_LOCATION_ + "objects/";

	for (const auto& entry : std::filesystem::directory_iterator(objectsLocation)) {
		auto location = entry.path().string();

		const size_t last_slash_idx = location.find_last_of("\\/");
		if (std::string::npos != last_slash_idx) {
			if (location[last_slash_idx + 1] == '_') {
				continue;
			}
		}

		std::cout << "[LOADING]: " << location << std::endl;

		auto* obj = new SceneCollection(HANDLER_, shaderTmp);

		LoadCollection(obj, &location);

		obj->Init();

		HANDLER_->sceneCollection->push_back(obj);
	}
}
