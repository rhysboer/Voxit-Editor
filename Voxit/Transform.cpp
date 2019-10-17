#include "Transform.h"

Transform::Transform() {
	this->model = glm::mat4(1.0f);
	this->position = glm::vec3(0.0f);
}

Transform::~Transform() {}

void Transform::SetPosition(glm::vec3 pos) {
	position = pos;
}

void Transform::SetScale(glm::vec3 scale) {
	model = glm::scale(model, scale);
}

void Transform::SetRotation(float angle, glm::vec3 axis) {
	model = glm::rotate(model, angle, axis);
}

void Transform::Translate(glm::vec3 offset) {
	position += offset;
}

glm::vec3 Transform::GetPosition() {
	return this->position;
}

glm::mat4 Transform::ModelMatrix() {
	return glm::translate(model, position);
}
