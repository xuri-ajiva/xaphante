#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
protected:
	glm::mat4 VIEW {};
	glm::mat4 PROJ {};
	glm::mat4 VIEW_PROJ {};
	glm::vec3 POSITION {};
public:
	virtual ~Camera() = default;

	Camera(float fov, float aspect) {
		PROJ = glm::perspective(fov, aspect, 0.02f, 1000.0f);
		VIEW = glm::mat4(1.0f);
		Update();
	}

	virtual void Update() {
		VIEW_PROJ = PROJ * VIEW;
	}

	void Translate(glm::vec3 v) {
		POSITION += v;
		VIEW = translate(VIEW, v * -1.0f);
	}

	glm::mat4 GetViewProj() const {
		return VIEW_PROJ;
	}
};

class FreeCamera : public Camera {
public:
	FreeCamera(float fov, float aspect) : Camera(fov, aspect) {
		OnMouseMoved(0.0f, 0.0f);
	}

	void MoveForward(float amount) {
		Translate(LOCK_AT_ * amount);
	}

	void MoveSideways(float amount) {
		Translate(normalize(cross(LOCK_AT_, UP_)) * amount);
	}

	void MoveUp(float amount) {
		Translate(UP_ * amount);
	}

	void OnMouseMoved(float xRel, float yRel) {
		YAW_ += xRel * MOUSE_SENSITIVITY_;
		PITCH_ -= yRel * MOUSE_SENSITIVITY_;
		if (PITCH_ > PITCH_MIN_MAX) PITCH_ = PITCH_MIN_MAX;
		if (PITCH_ < -PITCH_MIN_MAX) PITCH_ = -PITCH_MIN_MAX;

		glm::vec3 front;
		front.x = cos(glm::radians(PITCH_)) * cos(glm::radians(YAW_));
		front.y = sin(glm::radians(PITCH_));
		front.z = cos(glm::radians(PITCH_)) * sin(glm::radians(YAW_));

		LOCK_AT_ = normalize(front);
	}

	void Update() override {
		VIEW = lookAt(POSITION, POSITION + LOCK_AT_, UP_);

		VIEW_PROJ = PROJ * VIEW;
	}

	float PITCH_MIN_MAX      = 89.0f;
	float MOUSE_SENSITIVITY_ = 0.3f;
protected:
	float PITCH_ = 0.0f;		//	x  
	float YAW_   = -90.0f;		//	y

	glm::vec3 LOCK_AT_ {};
	glm::vec3 UP_ = {0.0f, 1.0f, 0.0f};

};

class FpsCamera : public FreeCamera {
public:
	FpsCamera(float fov, float aspect) : FreeCamera(fov, aspect) { }

	void MoveFront(float amount) {
		Translate(normalize(NOT_UP_ * LOCK_AT_) * amount);
	}

private:
	glm::vec3 NOT_UP_ = {1.0f, 0.0f, 1.0f};
};
