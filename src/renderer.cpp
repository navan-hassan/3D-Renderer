#include <renderer.hpp>
#include <iostream>

void Renderer::run() {
	std::cout << "Running Renderer" << std::endl; 
	window.init(800, 600);
	window.mainLoop();
}