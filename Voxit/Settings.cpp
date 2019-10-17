#include "Settings.h"

namespace Settings {
	glm::vec3 backgroundColour = glm::vec3(0.25f, 0.25f, 0.25f);
	glm::vec3 sunDirection = glm::vec3(0.0f, -1.0f, 0.0f);

	bool useVoxelOutline = true;
	bool useShadow = true;
	bool useGrid = true;
	bool useSun = true;
	bool useHighlighterSize = true;
}