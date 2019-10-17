#include "AABB.h"
#include "Voxel.h"

AABB::AABB(glm::mat4 model, const float vertices[], const unsigned int size) {
	CreateAABB(model, vertices, size);
}

void AABB::CreateAABB(glm::mat4 model, const float vertices[], const unsigned int size) {
	glm::vec4 vector = glm::vec4(0);

	float min_x = FLOAT_MAX; float max_x = FLOAT_MIN;
	float min_y = FLOAT_MAX; float max_y = FLOAT_MIN;
	float min_z = FLOAT_MAX; float max_z = FLOAT_MIN;

	for(unsigned int i = 0; i < size; i += 3) {
		vector = glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1) * model;

		if(vector.x < min_x)
			min_x = vector.x;
		if(vector.x > max_x)
			max_x = vector.x;
		if(vector.y < min_y)
			min_y = vector.y;
		if(vector.y > max_y)
			max_y = vector.y;
		if(vector.z < min_z)
			min_z = vector.z;
		if(vector.z > max_z)
			max_z = vector.z;
	}

	// TOP
	points.push_back(glm::vec3(min_x, max_y, min_z)); // BACK LEFT 
	points.push_back(glm::vec3(max_x, max_y, min_z)); // BACK RIGHT
	points.push_back(glm::vec3(max_x, max_y, max_z)); // FRONT RIGHT
	points.push_back(glm::vec3(min_x, max_y, max_z)); // FRONT LEFT

	// BOT
	points.push_back(glm::vec3(min_x, min_y, min_z)); // BACK LEFT
	points.push_back(glm::vec3(max_x, min_y, min_z)); // BACK RIGHT
	points.push_back(glm::vec3(max_x, min_y, max_z)); // FRONT RIGHT
	points.push_back(glm::vec3(min_x, min_y, max_z)); // FRONT LEFT
}

std::vector<glm::vec3> AABB::GetAABB() {
	return points;
}
