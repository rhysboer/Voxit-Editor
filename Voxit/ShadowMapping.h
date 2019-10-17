#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderManager.h"

#include "Window.h"

class ShadowMapping {
public:

	ShadowMapping(unsigned int width, unsigned int height);
	~ShadowMapping();

	void FrameBuffer_Start();
	void FrameBuffer_End();


	Shader* GetShader() const;
	unsigned int GetDepthMap() const;
private:
	void InitDepthBuffer();
	
	Shader* shader = nullptr;

	glm::mat4 lightMatrix;

	unsigned int depthMapFBO;	// FRAME BUFFER OBJECT
	unsigned int depthMap;		// Texture
	unsigned int width, height;
};

