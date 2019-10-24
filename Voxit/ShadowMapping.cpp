#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(unsigned int width,unsigned int height) {
	this->depthMap = 0;
	this->FBO = 0;
	this->width = width;
	this->height = height;
	this->position = glm::vec3(-10.0f, 10.0f, 0.0f);
	this->depthPV = glm::mat4(0.0f);
	
	this->shader = ShaderManager::GetShader("depth");

	InitDepthBuffer();
}

ShadowMapping::~ShadowMapping() { }

void ShadowMapping::InitDepthBuffer() {
	glm::mat4 projection = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 20.0f);
	glm::mat4 view = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	depthPV = projection * view;

	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &depthMap);
	
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapping::FrameBuffer_Start() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	shader->SetMatrix4("_lightMatrix", depthPV);
}

void ShadowMapping::FrameBuffer_End() {
	glm::vec2 size = Window::GetSize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

Shader* ShadowMapping::GetShader() const {
	return shader;
}

unsigned int ShadowMapping::GetDepthMap() const {
	return depthMap;
}

glm::mat4 ShadowMapping::GetDepthPV() const {
	return depthPV;
}

void ShadowMapping::SetPosition(const glm::vec3& pos) {
	glm::vec3 lookVec = glm::normalize(pos - glm::vec3(0.0f));
	glm::vec3 rightVec = glm::cross(lookVec, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 upVec = glm::cross(rightVec, lookVec);

	glm::mat4 projection = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 40.0f);
	glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f), upVec);
	position = pos;

	depthPV = projection * view;
}

glm::vec3 ShadowMapping::GetPosition() const {
	return position;
}
