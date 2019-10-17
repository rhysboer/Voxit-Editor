#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Time.h"
#include "Input.h"
#include "imgui.h"

#define CAMERA_SENSITIVITY 0.3f
#define CAMERA_SPEED 13.0f

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float fov, float near, float far);
	~Camera();

	void InputHandler();
	void Rotate(float x, float y);

	glm::mat4 View();
	glm::mat4 Projection();

	glm::vec3 Position() { return position; }

	static Camera* ActiveCamera;

private:

	void UpdateCamera();
	void LimitRotation();

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float fov;
	float _near;
	float _far;
};

