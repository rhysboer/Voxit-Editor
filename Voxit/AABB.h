#pragma once
#include <vector>
#include "glm/matrix.hpp"

#define FLOAT_MAX 3.40282e+038f
#define FLOAT_MIN 1.17549e-038f

class AABB {
public:

	AABB() {};
	AABB(glm::mat4 model, const float vertices[], const unsigned int size);
	~AABB() {};

	void CreateAABB(glm::mat4 model, const float vertices[], const unsigned int size);
	std::vector<glm::vec3> GetAABB();

private:

	std::vector<glm::vec3> points = std::vector<glm::vec3>();
};

