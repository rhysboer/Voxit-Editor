#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:

	Transform();
	~Transform();

	void SetPosition(glm::vec3 pos);
	void SetScale(glm::vec3 scale);
	void SetRotation(float angle, glm::vec3 axis);
	void Translate(glm::vec3 offset);
	
	glm::vec3 GetPosition();
	glm::mat4 ModelMatrix();

private:

	glm::mat4 model;
	glm::vec3 position;
};

