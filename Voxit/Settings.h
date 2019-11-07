#pragma once
#include "glm/glm.hpp"

namespace Settings {
	const extern glm::vec3 SUN_DIRECTION;

	extern glm::vec3 backgroundColour;
	extern glm::vec3 sunDirection;

	extern float angle;
	extern float rotationSpeed;

	const extern float ROTATION_SPEED_MIN;
	const extern float ROTATION_SPEED_MAX;

	extern bool useVoxelOutline;
	extern bool useGrid;
	extern bool useLighting;
	extern bool useHighlighterSize;
	extern bool useAnimateLight;
}