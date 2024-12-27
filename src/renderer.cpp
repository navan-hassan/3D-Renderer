#include <renderer.hpp>
#include <iostream>
#include <debug_utilities.hpp>

void Renderer::run() {
	debug_write("Starting Renderer...");

	window.init(800, 600);
	vulkanContext.init();
	window.mainLoop();
	terminate();
}

void Renderer::terminate() {
	vulkanContext.onDestroy();
	debug_write("Terminated Renderer");
}