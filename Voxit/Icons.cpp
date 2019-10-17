#include "Icons.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

unsigned int Icons::iconData[(unsigned int)Icons::IconID::COUNT] = { -1 };

unsigned int Icons::GetIcon(const Icons::IconID id) {
	if(iconData[0] == -1) LoadIcons();
	return iconData[(unsigned int)id];
}

void Icons::LoadIcons() {
	int x, y, n;
	unsigned char* data;

	const char* paths[] = {
		"./data/icons/voxel.png",
		"./data/icons/eyedropper.png",
		"./data/icons/eraser.png",
		"./data/icons/gradient.png",
		"./data/icons/folder.png",
		"./data/icons/file.png",
		"./data/icons/error.png",
		"./data/icons/success.png"
	};

	for(int i = 0; i < (unsigned int)IconID::COUNT; i++) {
		data = stbi_load(paths[i], &x, &y, &n, 0);

		if(!data) {
			printf("Failed to load image at path: %s", paths[i]);
			continue;
		}

		glGenTextures(1, &iconData[i]);
		glBindTexture(GL_TEXTURE_2D, iconData[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
}
