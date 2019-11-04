#include "Application.h"


/*
	
	MODEL = Object Transform (aka world space)
	VIEW = Camera Transform
	PROJECTION = Camera Lens

*/


Application::Application(Window* window) {
	OnStart();
}

Application::~Application() {
	delete voxelBuilder;
	delete camera;
}

void Application::OnUpdate() {
	Time::Update();

	camera->InputHandler();
	voxelBuilder->OnUpdate();
}

void Application::OnDraw() {
	voxelBuilder->Render(camera);

	Renderer::Render();
}

void Application::OnStart() {
	camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, glm::radians(40.0f), 0.01f, 1000.0f);
	voxelBuilder = new VoxelBuilder();
}
