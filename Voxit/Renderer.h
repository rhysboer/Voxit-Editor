#pragma once
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "ShaderManager.h"

struct Line {
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 colour;

	unsigned int vao;
	unsigned int vbo;
};

class Renderer {
public:

	static void AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(0.5f));
	static void Render();
	static void Clear();

private:

	static std::vector<Line> lines;
};

