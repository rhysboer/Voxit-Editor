#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(unsigned int shadowResolution) {
	this->shadowResolution = shadowResolution;
	//this->depthMap = 0;// [CASCADE_AMOUNT] = { 0 };
	//this->FBO = 0;// [CASCADE_AMOUNT] = { 0 };
	this->position = glm::vec3(-10.0f, 10.0f, 0.0f); // REMOVE

	for(int i = 0; i < CASCADE_AMOUNT; i++) {
		this->depthProjectionView.push_back(glm::mat4(0.0f));
		this->cascadeSplitsTEST.push_back(0.0f);
	}
	
	// Distance between each cascade
	cascadeSplits.push_back(minCascadeDistance + 0.50f * maxCascadeDistance);	// 0.05f
	cascadeSplits.push_back(minCascadeDistance + 1.50f * maxCascadeDistance);	// 0.15f
	cascadeSplits.push_back(minCascadeDistance + 5.00f * maxCascadeDistance);	// 0.50f

	this->shader = ShaderManager::GetShader("depth");

	InitDepthBuffer();
}

ShadowMapping::~ShadowMapping() { }

void ShadowMapping::InitDepthBuffer() {
	//glm::vec3 lightDirection = glm::vec3(10.0f, -10.0f, -3.0f);
	//float offset = 20;
	//
	//float cameraFar = Camera::ActiveCamera->GetFar();
	//Camera::ActiveCamera->SetFar(50.0f);
	//std::vector<glm::vec3> points = Camera::ActiveCamera->GetFrustum()->points;
	//Camera::ActiveCamera->SetFar(cameraFar);
	//
	//glm::vec3 forward = glm::normalize(Settings::sunDirection);
	//glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	//glm::vec3 up = glm::normalize(glm::cross(right, forward));
	//
	//glm::mat4 lightMatrix = glm::lookAt(glm::vec3(0.0f), forward, up);
	//glm::mat4 lightMatrix_inv = glm::inverse(lightMatrix);
	//
	//for(int i = 0; i < 8; i++) {
	//	points[i] = glm::vec3(lightMatrix * glm::vec4(points[i], 1.0f));
	//}
	//
	//glm::vec3 min = points[0];
	//glm::vec3 max = points[0];
	//
	//for(int i = 1; i < 8; i++) {
	//	min.x = glm::min(min.x, points[i].x);
	//	min.y = glm::min(min.y, points[i].y);
	//	min.z = glm::min(min.z, points[i].z);
	//
	//	max.x = glm::max(max.x, points[i].x);
	//	max.y = glm::max(max.y, points[i].y);
	//	max.z = glm::max(max.z, points[i].z);
	//}
	//
	//float width = abs(max[0] - min[0]);
	//float height = abs(max[1] - min[1]);
	//float far = abs(max[2] - min[2]);
	//
	//glm::vec3 position = glm::vec3(lightMatrix_inv * glm::vec4(0.5f * (min + max), 1.0f));
	//
	//glm::mat4 view = glm::lookAt(position, position + Settings::sunDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 projection = glm::ortho(-0.5f * width - offset, 0.5f * width + offset,
	//								  -0.5f * height - offset, 0.5f * height + offset,
	//								  -0.5f * far - offset, 0.5f * far + offset);

	
	glGenFramebuffers(CASCADE_AMOUNT, FBO);
	glGenTextures(CASCADE_AMOUNT, depthMap);
	
	float colour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	for(int i = 0; i < CASCADE_AMOUNT; i++) {
		// Texture settings
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);

		// Frame Buffer settings
		glBindFramebuffer(GL_FRAMEBUFFER, FBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void ShadowMapping::FrameBuffer_Start(int index) {
	glViewport(0, 0, shadowResolution, shadowResolution);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[index]);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	SetShadowMapProjectionView(index);
	shader->SetMatrix4("_lightMatrix", depthProjectionView[index]);
}

void ShadowMapping::FrameBuffer_End(int index) {
	glm::vec2 size = Window::GetSize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float ShadowMapping::GetCascadeSplit(const int& index) const {
	return cascadeSplitsTEST[index];
}

void ShadowMapping::BindDepthTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthMap[2]);
}

Shader* ShadowMapping::GetShader() const {
	return shader;
}

unsigned int ShadowMapping::GetDepthMap(int index) const {
	return depthMap[0];
}

glm::mat4 ShadowMapping::GetDepthPV(int index) const {
	return depthProjectionView[index];
}

void ShadowMapping::SetShadowMapProjectionView(const int& index) {
	if(index > CASCADE_AMOUNT) return;


	Camera::ActiveCamera->SetFar(100.0f);

	std::vector<glm::vec3> frustumPoints = Camera::ActiveCamera->GetFrustum()->points;
	glm::vec3 frustumCenter = Camera::ActiveCamera->GetFrustum()->center;

	Camera::ActiveCamera->SetFar(1000.0f);

	float prevSplitDist = (index == 0) ? minCascadeDistance : cascadeSplits[index - 1];
	float splitDist = cascadeSplits[index];
	
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
	center *= 1.0f / 8.0f;

	glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 minExtents;
	glm::vec3 maxExtents;

	// Calculate the radius of a bounding sphere sounding the frustum corners
	float sphereRadius = 0.0f;
	for(int i = 0; i < 8; i++) {
		float dist = glm::length(frustumPoints[i] - center);
		sphereRadius = glm::max(sphereRadius, dist);
	}

	sphereRadius = std::ceil(sphereRadius * 16.0f) / 16.0f;
	
	maxExtents = glm::vec3(sphereRadius);
	minExtents = -maxExtents;

	// DEBUG / TEST
	cascadeSplitsTEST[index] = Camera::ActiveCamera->GetNear() + splitDist * (100.0f - Camera::ActiveCamera->GetNear());

	glm::vec3 cascadeExtents = maxExtents - minExtents;

	// Get the position of the shadow camera
	glm::vec3 shadowCamPos = center + Settings::sunDirection * -minExtents.z;


	// Come up with a new ortho Camera for the shadow caster
	glm::mat4 shadowCamProj = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y,
									 0.0f, cascadeExtents.z);
	glm::mat4 shadowCamView = glm::lookAt(shadowCamPos, center, upDir);


	// STABILIZE CASCADE
	glm::mat4 shadowMatrix = shadowCamProj * shadowCamView;
	glm::vec4 shadowOrigin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	shadowOrigin = shadowMatrix * shadowOrigin;
	shadowOrigin = shadowOrigin * (shadowResolution / 2.0f);

	glm::vec4 roundedOrigin = glm::round(shadowOrigin);
	glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
	roundOffset = roundOffset * 2.0f / (float)shadowResolution;
	roundOffset.z = 0.0f;
	roundOffset.w = 0.0f;

	// Fix this
	glm::mat4 shadowProj = shadowCamProj;// *shadowCamView;
	shadowProj[3] += roundOffset;
	shadowCamProj = shadowProj; // MAYBE???

	depthProjectionView[index] = (shadowCamProj * shadowCamView);
}

glm::vec3 ShadowMapping::GetPosition() const {
	return position;
}

/*





	float offset = 20;

	float cameraFar = Camera::ActiveCamera->GetFar();
	Camera::ActiveCamera->SetFar(50.0f);

	std::vector<glm::vec3> points = Camera::ActiveCamera->GetFrustum()->points;

	Camera::ActiveCamera->SetFar(cameraFar);

	//glm::vec3 sunForward = glm::vec3(Settings::sunMatrix[2]);
	//glm::vec3 sunUp = glm::vec3(Settings::sunMatrix[1]);

	glm::vec3 lightDirection = glm::vec3(10.0f, -10.0f, -3.0f);

	glm::vec3 forward = glm::normalize(lightDirection);
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	glm::mat4 lightMatrix = glm::lookAt(glm::vec3(0), forward, up); //Settings::sunMatrix;
	glm::mat4 lightMatrix_inv = glm::inverse(lightMatrix);

	for(int i = 0; i < 8; i++) {
		points[i] = glm::vec3(lightMatrix * glm::vec4(points[i], 1.0f));
	}

	glm::vec3 min = points[0];
	glm::vec3 max = points[0];

	for(int i = 1; i < 8; i++) {
		min.x = glm::min(min.x, points[i].x);
		min.y = glm::min(min.y, points[i].y);
		min.z = glm::min(min.z, points[i].z);

		max.x = glm::max(max.x, points[i].x);
		max.y = glm::max(max.y, points[i].y);
		max.z = glm::max(max.z, points[i].z);
	}

	float pointSize = glm::length(points[0] - points[7]);
	float worldUnitsPerTexel = pointSize / texWidth;

	min /= worldUnitsPerTexel;
	min = glm::floor(min);
	min *= worldUnitsPerTexel;

	max /= worldUnitsPerTexel;
	max = glm::floor(max);
	max *= worldUnitsPerTexel;

	float width = abs(max.x - min.x);
	float height = abs(max.y - min.y);
	float far = abs(max.z - min.z);

	glm::vec3 position = glm::vec3(lightMatrix_inv * glm::vec4(0.5f * (min + max), 1.0f));

	glm::mat4 view = glm::lookAt(position, position + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::ortho(-0.5f * width - offset, 0.5f * width + offset,
										-0.5f * height - offset, 0.5f * height + offset,
										-0.5f * far - offset, 0.5f * far + offset);

	depthPV = projection * view;








*/