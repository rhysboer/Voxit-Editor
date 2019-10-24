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
	glm::mat4 GetDepthPV() const;

	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;

private:
	void InitDepthBuffer();
	
	Shader* shader = nullptr;

	glm::vec3 position;
	glm::mat4 depthPV;

	unsigned int FBO;	// FRAME BUFFER OBJECT
	unsigned int depthMap;		// Texture
	unsigned int width, height;
};

