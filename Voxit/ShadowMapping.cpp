#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(unsigned int shadowResolution) {
	this->shadowResolution = shadowResolution;

	for(int i = 0; i < CASCADE_AMOUNT; i++) {
		this->depthProjectionView.push_back(glm::mat4(0.0f));
		this->splits.push_back(0.0f);
	}

	// Distance between each cascade
	cascadeDistances.push_back(0.10f * 1.0f);
	cascadeDistances.push_back(0.25f * 1.0f);
	cascadeDistances.push_back(0.50f * 1.0f);
	cascadeDistances.push_back(1.00f * 1.0f);

	this->depthShader = ShaderManager::GetShader("depth");

	InitDepthBuffer();
}

ShadowMapping::~ShadowMapping() {}

void ShadowMapping::InitDepthBuffer() {
	glGenFramebuffers(CASCADE_AMOUNT, FBO);
	glGenTextures(CASCADE_AMOUNT, depthMap);

	float colour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	for(int i = 0; i < CASCADE_AMOUNT; i++) {
		// Texture settings
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);

		// Frame Buffer settings
		glBindFramebuffer(GL_FRAMEBUFFER, FBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void ShadowMapping::FrameBuffer_Start(const int& index) {
	glViewport(0, 0, shadowResolution, shadowResolution);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[index]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_CLAMP);

	SetShadowMapProjectionView(index);
	depthShader->SetMatrix4("_lightMatrix", depthProjectionView[index]);
}

void ShadowMapping::FrameBuffer_End() {
	glm::vec2 size = Window::GetSize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_CLAMP);
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMapping::ClearBuffer() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float ShadowMapping::GetCascadeSplit(const int& index) const {
	return splits[index];
}

glm::mat4 ShadowMapping::GetShadowProjectionView(const int& index) const {
	return depthProjectionView[index];
}

Shader* ShadowMapping::GetDepthShader() const {
	return depthShader;
}

void ShadowMapping::BindDepthTextures() const {
	for(int i = 0; i < CASCADE_AMOUNT; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
	}
}

void ShadowMapping::SetShadowMapProjectionView(const int& index) {
	if(index > CASCADE_AMOUNT) return;

	std::vector<glm::vec3> frustumPoints = Camera::ActiveCamera->GetFrustum()->points;

	float prevSplitDist = (index == 0) ? minCascadeDistance : cascadeDistances[index - 1];
	float splitDist = cascadeDistances[index];

	// Get the corners of the current cascade slice of the view frustum
	for(int i = 0; i < 4; i++) {
		glm::vec3 cornerRay = frustumPoints[i + 4] - frustumPoints[i];
		glm::vec3 nearCornerRay = cornerRay * prevSplitDist;
		glm::vec3 farCornerRay = cornerRay * splitDist;

		frustumPoints[i + 4] = frustumPoints[i] + farCornerRay;
		frustumPoints[i] = frustumPoints[i] + nearCornerRay;
	}

	glm::vec3 center = glm::vec3(0.0f);
	for(int i = 0; i < 8; i++)
		center += frustumPoints[i];
	center /= 8.0f;

	glm::vec3 minExtents;
	glm::vec3 maxExtents;

	// Calculate the radius of a bounding sphere sounding the frustum corners
	float sphereRadius = 0.0f;
	for(int i = 0; i < 8; i++) {
		float dist = glm::length(frustumPoints[i] - center);
		sphereRadius = glm::max(sphereRadius, dist);
	}

	sphereRadius = std::ceil(sphereRadius * 16.0f) / 16.0f;

	maxExtents = glm::vec3(sphereRadius, sphereRadius, sphereRadius);
	minExtents = -maxExtents;

	splits[index] = Camera::ActiveCamera->GetNear() + splitDist * (Camera::ActiveCamera->GetFar() - Camera::ActiveCamera->GetNear());

	glm::vec3 cascadeExtents = maxExtents - minExtents;

	// Get the position of the shadow camera
	glm::vec3 shadowCamPos = (center + Settings::sunDirection * -minExtents.z);

	// Come up with a new ortho Camera for the shadow caster
	glm::mat4 shadowCamProj = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, cascadeExtents.z);
	glm::mat4 shadowCamView = glm::lookAt(shadowCamPos, center, glm::vec3(0.0f, 1.0f, 0.0f));

	// STABILIZE CASCADE
	glm::mat4 shadowMatrix = shadowCamProj * shadowCamView;
	glm::vec4 shadowOrigin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	shadowOrigin = shadowMatrix * shadowOrigin;
	shadowOrigin = shadowOrigin * (shadowResolution / 2.0f);

	glm::vec4 roundOffset = glm::round(shadowOrigin) - shadowOrigin;
	roundOffset = roundOffset * 2.0f / (float)shadowResolution;
	roundOffset.z = 0.0f;
	roundOffset.w = 0.0f;

	shadowCamProj[3] += roundOffset;

	depthProjectionView[index] = (shadowCamProj * shadowCamView);
}
