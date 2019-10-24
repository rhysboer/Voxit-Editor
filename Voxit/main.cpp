#include <glad/glad.h>	// OpenGL Function Pointers
#include <GLFW/glfw3.h> // OpenGL Window API

//#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Application.h"
#include "ShaderManager.h"

#include "Settings.h"

void SetupImGui(GLFWwindow* window);

int main() {

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window* window = new Window();

	// Main Loop
	if (window->Init("Voxit Editor", 1280, 800)) {
		ShaderManager::InitShaders();
		Application app = Application(window);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		SetupImGui(window->GetWindow());

		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::GetStyle().WindowBorderSize = 0.0f;

		while (!glfwWindowShouldClose(window->GetWindow())) {
			// INPUT
			// -------
			if (glfwGetKey(window->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window->GetWindow(), true);

			// UPDATE
			// -------

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//ImGui::ShowDemoWindow();


			app.OnUpdate();

			// Update Window Components
			window->WindowUpdate();

			// CLEAR
			// -------
			glClearColor(Settings::backgroundColour.r, Settings::backgroundColour.g, Settings::backgroundColour.b, 1.0f); // 0.25f, 0.25f, 0.25f, 1.0f
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// RENDER
			// -------
			app.OnDraw();

			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// SWAP BUFFERS
			// -------
			glfwSwapBuffers(window->GetWindow());
			glfwPollEvents();
		}
	}
	
	delete window;
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}


void SetupImGui(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
}