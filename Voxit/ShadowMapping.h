#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "ShaderManager.h"
#include "Settings.h"
#include "Window.h"
#include "Camera.h"

class ShadowMapping {
public:
	ShadowMapping(unsigned int shadowResolution);
	~ShadowMapping();

	void FrameBuffer_Start(int index = 0);
	void FrameBuffer_End(int index = 0);
	
	float GetCascadeSplit(const int& index) const;

	void BindDepthTextures();

	static const unsigned int CASCADE_AMOUNT = 4;
private:
	void SetShadowMapProjectionView(const int& index);
	void InitDepthBuffer();

	std::vector<glm::mat4> depthProjectionView = std::vector<glm::mat4>();
	std::vector<float> cascadeDistances = std::vector<float>();
	std::vector<float> splits = std::vector<float>();

	Shader* depthShader = nullptr;

	unsigned int FBO[CASCADE_AMOUNT]; // Frame buffer object
	unsigned int depthMap[CASCADE_AMOUNT]; // Depth map textures
	unsigned int shadowResolution;

	const float minCascadeDistance = 0.0f;
	const float maxCascadeDistance = 1.0f;
};

