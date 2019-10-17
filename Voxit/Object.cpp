#include "Object.h"

void Object::OnDraw(Camera* const camera) {
	if(!shader) return;

	shader->SetMatrix4("_model", transform.ModelMatrix());
	shader->SetMatrix4("_view", camera->View());
	shader->SetMatrix4("_projection", camera->Projection());

	shader->UseProgram();
}

Transform& Object::GetTransform() {
	return transform;
}

Shader& Object::GetShader() {
	return *shader;
}

void Object::SetShader(Shader* shader) {
	this->shader = shader;
}

void Object::SetActive(bool value) {
	this->isActive = value;
}

Object::Object() {
	shader = nullptr;
}
Object::~Object() {}
