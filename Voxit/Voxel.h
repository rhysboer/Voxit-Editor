#pragma once
#include "glm/glm.hpp"

struct Voxel {
	Voxel() : colour(1.0f), position(0.0f) { }
	Voxel(const Voxel& vox) : colour(vox.colour), position(vox.position) {}
	glm::vec3 colour;
	glm::vec3 position;
};