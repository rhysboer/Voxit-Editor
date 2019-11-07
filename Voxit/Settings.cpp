#include "Settings.h"

namespace Settings {
	const glm::vec3 SUN_DIRECTION = glm::vec3(0.58f, 0.58f, 0.58f);

	glm::vec3 backgroundColour = glm::vec3(0.25f, 0.25f, 0.25f);
	glm::vec3 sunDirection = SUN_DIRECTION;

	float angle = 0.0f; // Sun Rotation Angle (Degrees)
	float rotationSpeed = 30.0f;
	const float ROTATION_SPEED_MAX = 100.0f;
	const float ROTATION_SPEED_MIN = 1.0f;

	bool useVoxelOutline = true;
	bool useGrid = true;
	bool useLighting = true;
	bool useHighlighterSize = true;
	bool useAnimateLight = false;
}