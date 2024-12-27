#include <render_window.hpp>
#include <iostream>

void Window::init(int width, int height) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
}

void Window::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	onDestroy();
}

void Window::onDestroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Releasing all GLFW resources..." << std::endl;
}