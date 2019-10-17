#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "StringHelper.h"

class ShaderManager;

class Shader {
public:
	Shader();
	~Shader();

	void UseProgram();

	void SetFloat(const char* name, float value);
	void SetInt(const char* name, int value);
	void SetMatrix4(const char* name, glm::mat4 value);
	void SetVector4(const char* name, glm::vec4 value);
	void SetVector3(const char* name, glm::vec3 value);

protected:
	friend ShaderManager;
	bool LoadShader(std::string vertSource, std::string fragSource);
private:

	bool ErrorHandler(unsigned int toTest, unsigned int statusType);

	int shaderProgram;
};

