#pragma once
#include <map>
#include <vector>

#include "glm/glm.hpp"
#include "Camera.h"
#include "VoxelBuilder.h"
#include "Voxel.h"

#define DOUBLE_MAX 1.79769e+308

#include "World.h"

class RaycastHit {
public:
	RaycastHit(bool hit, glm::vec3 position, Voxel* object) {
		this->hitObject = hit;
		this->position = position;
		this->object = object;
	}
	~RaycastHit() {}

	bool hitObject;
	glm::vec3 position;
	Voxel* object;
};


namespace Raycaster {
	// Returns a RaycastHit
	RaycastHit VoxelDetection(std::map<int, Voxel>* voxels);

	RaycastHit TEST_VoxelDetection();
}
