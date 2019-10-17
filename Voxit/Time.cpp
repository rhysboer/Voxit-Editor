#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::sinceLastUpdate = 0.0f;

void Time::Update() {
	deltaTime = glfwGetTime() - sinceLastUpdate;
	sinceLastUpdate = glfwGetTime();
}

float Time::DeltaTime() {
	return deltaTime;
}
