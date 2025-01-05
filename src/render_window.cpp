#include <render_window.hpp>
#include <iostream>
#include <debug_utilities.hpp>

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

	debug_write("GLFW window successfully destroyed");
}