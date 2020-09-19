#include "SceneCollection.h"

#include <filesystem>

SceneCollection::SceneCollection(SceneHandler* handler, ShaderHandler* shader):
	SHADER_(shader), SCENE_HANDLER_(handler) {

	objects = new std::vector<ObjectHandler*>();

	MODEL = glm::mat4(1.0f);

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;
}

void SceneCollection::Init() {
	modelUniform        = glGetUniformLocation(SHADER_->GetShaderId(), "u_modelViewProje") GL_ERROR
	modelViewUniform    = glGetUniformLocation(SHADER_->GetShaderId(), "u_modelView") GL_ERROR
	invModelViewUniform = glGetUniformLocation(SHADER_->GetShaderId(), "u_invModelView") GL_ERROR
}

void SceneCollection::Draw(WindowWrapper* window) {

	MODEL_VIEW_PROJE = SCENE_HANDLER_->CAMERA->GetViewProj() * MODEL;

	glm::mat4 modelView    = SCENE_HANDLER_->CAMERA->GetView() * MODEL;
	glm::mat4 invModelView = transpose(inverse(modelView));

	SHADER_->Bind();

	glUniformMatrix4fv(modelUniform, 1, false, &MODEL_VIEW_PROJE[0][0])GL_ERROR
	glUniformMatrix4fv(modelViewUniform, 1, false, &modelView[0][0])GL_ERROR
	glUniformMatrix4fv(invModelViewUniform, 1, false, &invModelView[0][0])GL_ERROR

	for (auto* collection : *objects) {
		collection->Draw(window);
	}
	SHADER_->Unbind();
}

void SceneCollection::CleanUp(SceneHandler* scene_handler) {
	for (auto* collection : *objects) {
		collection->CleanUp(this);
		delete collection;
	}
	objects->clear();
	delete objects;
	delete SHADER_;
}

void SceneCollection::GameLoop(float delta) {
	for (auto* collection : *objects) {
		collection->GameLoop(delta);
	}

	MODEL = rotate(MODEL, 1.0f * delta, glm::vec3(0, 1, 0));
	//MODEL = glm::translate(MODEL, glm::vec3(0, 0, sinf(delta->WINDOW_W_->time) * delta->WINDOW_W_->delta));
}
