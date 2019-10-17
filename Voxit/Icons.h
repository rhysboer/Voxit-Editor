#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Icons {
public:
	static enum class IconID {
		PEN,
		EYEDROPPER,
		ERASER,
		GRADIENT,
		FOLDER,
		FILE,
		ERROR_,
		SUCCESS,

		// LAST
		COUNT
	};

	static unsigned int GetIcon(const Icons::IconID id);

private:
	static void LoadIcons();

	static unsigned int iconData[];
};

