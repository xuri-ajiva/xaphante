#include "SceneCollection.h"

#include <filesystem>

SceneCollection::SceneCollection(SceneHandler* handler, std::string* sceneCollectionLocation): SCENE_COLLECTION_LOCATION_(sceneCollectionLocation),
                                                                                               SCENE_HANDLER_(handler) {
	objects = new std::vector<ObjectHandler*>();

	MODEL = glm::mat4(1.0f);

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;
}

void SceneCollection::Init(std::string* vertexShader, std::string* fragmentShader) {
	SHADER_ = new ShaderHandler(vertexShader, fragmentShader);
	SHADER_->Bind();

	for (const auto& entry : std::filesystem::directory_iterator(*SCENE_COLLECTION_LOCATION_)) {
		auto location = entry.path().string();
		location      = location.replace(location.find('\\'), 1, "/");

		const size_t last_slash_idx = location.find_last_of("\\/");
		if (std::string::npos != last_slash_idx) {
			if (location[last_slash_idx + 1] == '_') {
				continue;
			}
		}

		std::cout << "[LOADING]: " << location << std::endl;

		auto* obj = new ObjectHandler();
		obj->Init(&location, SHADER_);
		objects->push_back(obj);
	}

	modelUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_modelViewProje") GL_ERROR
	colorUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_color") GL_ERROR
}

void SceneCollection::Draw(WindowWrapper* window) const {
	SHADER_->Bind();
	glUniform4f(colorUniform,
	            sinf( window->time / 10.0f),
	            cosf( window->time / 10.0f),
	            -sinf(window->time / 10.0f),
	            1.0f)GL_ERROR

	glUniformMatrix4fv(modelUniform, 1, false, &MODEL_VIEW_PROJE[0][0])GL_ERROR

	for (auto collection : *objects) {
		collection->Draw(window);
	}
	SHADER_->Unbind();
}

void SceneCollection::CleanUp(SceneHandler* scene_handler) {
	for (auto collection : *objects) {
		collection->CleanUp(this);
		delete collection;
	}
	objects->clear();
	delete objects;
	delete SHADER_;
}

void SceneCollection::GameLoop(SceneHandler* scene_handler) {
	for (auto collection : *objects) {
		collection->GameLoop(this);
	}

	//MODEL = rotate(MODEL, 1.0f * scene_handler->WINDOW_W_->delta, glm::vec3(0, 1, 0));
	//MODEL = glm::translate(MODEL, glm::vec3(0, 0, sinf(scene_handler->WINDOW_W_->time) * scene_handler->WINDOW_W_->delta));

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;

	//std::cout << "Time: " << scene_handler->WINDOW_W_->time << std::endl;
}
