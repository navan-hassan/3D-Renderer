#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <constants.hpp>

class Window {
public:
	void init(int width, int height);

	void mainLoop();

	void onDestroy();

private:
	GLFWwindow* window = nullptr;
};

#endif