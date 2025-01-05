#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <GLFW/glfw3.h>

// Wrapper class for interfacing with a GLFW window
class Window {
public:
	void init(int width, int height);

	void mainLoop();

	void onDestroy();
	GLFWwindow* window = nullptr;
};

#endif