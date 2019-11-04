#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
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

	Shader* GetShader() const;
	unsigned int GetDepthMap(int index = 0) const;
	glm::mat4 GetDepthPV(int index = 0) const;

	glm::vec3 GetPosition() const;

	static const unsigned int CASCADE_AMOUNT = 3;
private:
	void SetShadowMapProjectionView(const int& index);
	void InitDepthBuffer();
	
	Shader* shader = nullptr;

	glm::vec3 position;
	std::vector<glm::mat4> depthProjectionView = std::vector<glm::mat4>();
	std::vector<float> cascadeSplits = std::vector<float>();

	std::vector<float> cascadeSplitsTEST = std::vector<float>();


	unsigned int FBO[CASCADE_AMOUNT];								// FRAME BUFFER OBJECT
	unsigned int depthMap[CASCADE_AMOUNT] ;							// Texture
	unsigned int shadowResolution;


	// Test
	const float minCascadeDistance = 0.0f;
	const float maxCascadeDistance = 1.0f;
};

