#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Input;

class Window {
public:
	Window();
	~Window();

	bool Init(std::string title, int width, int height);

	GLFWwindow* GetWindow();
	static GLFWwindow* GetGLFWWindow();

	glm::vec2 Size();
	static glm::vec2 GetSize();

	void WindowUpdate();

private:
	void InitCallbacks();
	
	static GLFWwindow* window;
};


