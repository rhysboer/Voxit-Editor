#include "Camera.h"

Camera* Camera::ActiveCamera;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float fov, float near, float far) {
	this->position = position;
	this->worldUp = worldUp;
	this->yaw = yaw;
	this->pitch = pitch;
	this->fov = fov;
	this->_near = near;
	this->_far = far;

	ActiveCamera = this;

	UpdateCamera();
}

Camera::~Camera() {
}

void Camera::InputHandler() {
	glm::vec3 translate = glm::vec3(0.0f);

	if(Input::IsKeyDown(GLFW_KEY_W)) //input.IsKeyDown(GLFW_KEY_W)) // FORWARD
		translate = this->front;
	if(Input::IsKeyDown(GLFW_KEY_S)) // BACKWARD
		translate = -this->front;
	if(Input::IsKeyDown(GLFW_KEY_D)) // RIGHT
		translate = this->right;
	if(Input::IsKeyDown(GLFW_KEY_A)) // LEFT
		translate = -this->right;
	if(Input::IsKeyDown(GLFW_KEY_E)) // UP
		translate = this->up;
	if(Input::IsKeyDown(GLFW_KEY_Q)) // DOWN
		translate = -this->up;

	if(Input::IsMouseKeyDown(GLFW_MOUSE_BUTTON_2)) { // RIGHT CLICK
		glm::vec2 deltaPos = Input::MouseDeltaPosition();

		yaw += deltaPos.x * CAMERA_SENSITIVITY;
		pitch -= deltaPos.y * CAMERA_SENSITIVITY;

		LimitRotation();
	}

	position += (translate * Time::DeltaTime() * CAMERA_SPEED);
	UpdateCamera();
}

void Camera::Rotate(float x, float y) {
	yaw += x * CAMERA_SENSITIVITY;
	pitch -= y * CAMERA_SENSITIVITY;

	LimitRotation();
	UpdateCamera();
}


glm::mat4 Camera::View() {
	return view;
}

glm::mat4 Camera::Projection() {
	return projection;
}

void Camera::UpdateCamera() {
	glm::vec3 temp = glm::vec3(0.0f);
	temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch));
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(temp);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	view = glm::lookAt(position, position + front, up);
	
	glm::vec2 size = Window::GetSize();
	projection = glm::perspective(fov, (size.y == 0) ? 0.0f : size.x / size.y, _near, _far);
}

void Camera::LimitRotation() {
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
}
