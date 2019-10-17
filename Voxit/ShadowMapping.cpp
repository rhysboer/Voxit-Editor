#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(unsigned int width,unsigned int height) {
	this->depthMap = 0;
	this->depthMapFBO = 0;
	this->width = width;
	this->height = height;

	this->shader = ShaderManager::GetShader("voxelBasic");

	InitDepthBuffer();
}

ShadowMapping::~ShadowMapping() { }

Shader* ShadowMapping::GetShader() const {
	return shader;
}

unsigned int ShadowMapping::GetDepthMap() const {
	return depthMap;
}

void ShadowMapping::InitDepthBuffer() {
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(-10.0f, 10.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	lightMatrix = projection * view;

	glGenFramebuffers(1, &depthMapFBO);
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapping::FrameBuffer_Start() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	shader->SetMatrix4("_lightMatrix", lightMatrix);
}

void ShadowMapping::FrameBuffer_End() {
	glm::vec2 size = Window::GetSize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
