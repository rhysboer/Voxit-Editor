#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Time.h"
#include "Input.h"

#define CAMERA_SENSITIVITY 0.3f
#define CAMERA_SPEED 13.0f

class Frustum;
class Camera {
public:
	Camera(float pos_x, float pos_y, float pos_z, float worldUp_x, float worldUp_y, float worldUp_z, float yaw, float pitch, float fov, float near, float far);
	Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float fov, float near, float far);
	~Camera();

	void InputHandler();
	void Rotate(float x, float y);
	void SetFar(const float& value);
	void SetDirty();

	std::vector<glm::vec3> FrustumPoints();

	glm::mat4 View();
	glm::mat4 Projection();

	glm::vec3 Position() { return position; }
	Frustum* GetFrustum() const;
	float GetFar() const;
	float GetNear() const;
	bool IsDirty() const;

	static Camera* ActiveCamera;

	friend class Frustum;
private:

	void UpdateCamera();
	void LimitRotation();

	Frustum* frustum;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float fov;
	float _near;
	float _far;

	bool isDirty;
};

class Frustum {
public:
	std::vector<glm::vec3> points = std::vector<glm::vec3>();
	glm::vec3 center = glm::vec3(0.0f);

	friend class Camera;
private:
	void UpdateFrustum(const Camera& camera);
};
