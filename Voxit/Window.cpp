#include "Window.h"
#include "Input.h"

void _FramebufferResize(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* Window::window = nullptr;

Window::Window() {
	this->window = nullptr;
}

Window::~Window() {
	glfwDestroyWindow(window);
}

bool Window::Init(std::string title, int width, int height) {
	// Create Window
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (window == NULL) {
		printf("Failed to create window!");
		glfwTerminate();
		return false;
	}
	
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to load GLAD!");
		return false;
	}


	InitCallbacks();

	glViewport(0, 0, width, height);

	return true;
}

GLFWwindow* Window::GetWindow() {
	return window;	
}

GLFWwindow* Window::GetGLFWWindow() {
	return window;
}

void Window::WindowUpdate() {
	Input::Update();
}

void Window::InitCallbacks() {

	glfwSetFramebufferSizeCallback(this->window, _FramebufferResize);		// Resize
	glfwSetKeyCallback(this->window, Input::KeyCallback);					// Keyboard Input
	glfwSetMouseButtonCallback(this->window, Input::MouseButtonCallback);	// Mouse Buttons
	glfwSetCursorPosCallback(this->window, Input::MousePositionCallback);	// Mouse Position

}

glm::vec2 Window::Size() {
	return GetSize();
}

glm::vec2 Window::GetSize() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if(width == 0) 
		width = -1;
	if(height == 0) 
		height = -1;

	return glm::vec2(width, height);
}
