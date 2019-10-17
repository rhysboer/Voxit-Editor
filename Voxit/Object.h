#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.h"
#include "Transform.h"

#include "Camera.h"

class Object {
public:
	~Object();

	virtual void OnDraw(Camera* const camera);

	Transform& GetTransform();
	Shader& GetShader();
	void SetShader(Shader* shader);

	void SetActive(bool value);

protected:
	Object();

	Transform transform = Transform();
	Shader* shader;

	bool isActive = true;
	unsigned int vao = 0;
	unsigned int vbo = 0;
	
};

