#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "VoxelBuilder.h"
#include "ShaderManager.h"
#include "Time.h"

#include "imgui.h"

#include "Input.h"
#include "Window.h"

#include "Camera.h"

#include <string>


//DEBUG
#include "Renderer.h"

class Application {
public:
	Application(Window* window);
	~Application();
	
	void OnUpdate();
	void OnDraw();

private:
	void OnStart();

	VoxelBuilder voxelBuilder = VoxelBuilder();

	Shader* shader = nullptr;

	Shader* gridShader = nullptr;

	Camera* camera = nullptr;
};

